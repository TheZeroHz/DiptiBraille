#include <WiFi.h>
#include "Display.h"
#include "Audio.h"
#include <FS.h>
#include <SD.h>
#include <LittleFS.h>
#include <SD_MMC.h>
#include <FFat.h>
#include <SPI.h>
#include <ESPFMfGK.h>

#define app_core 1
#define utility_core 0
#define display_stack 2000
#define audio_stack 5000
#define webshare_stack 10000
#define remoteshell_stack 5000


bool WebShareEnabled=false;




TaskHandle_t display_handle = NULL;
TaskHandle_t audio_handle = NULL;
TaskHandle_t webshare_handle = NULL;



Display disp;
Audio audio;
const word filemanagerport = 8080;
ESPFMfGK filemgr(filemanagerport);  



//****************************************************************************************
//                                    Display  _ T A S K                                 *
//****************************************************************************************

struct dispMessage {
  uint8_t cmd;
  String txt;
  uint32_t value;
  uint32_t ret;
} dispTxMessage, dispRxMessage;

enum : uint8_t {
  SET_FREQ,
  GET_FREQ,
  DISP_PRINT,
  DISP_DISABLE,
  DISP_ENABLE
};

QueueHandle_t dispSetQueue = NULL;
QueueHandle_t dispGetQueue = NULL;

void dispCreateQueues() {
  dispSetQueue = xQueueCreate(10, sizeof(struct dispMessage));
  dispGetQueue = xQueueCreate(10, sizeof(struct dispMessage));
}

void dispTask(void* parameter) {
  dispCreateQueues();
  if (!dispSetQueue || !dispGetQueue) {
    log_e("queues are not initialized");
    while (true) { ; }  // endless loop
  }

  struct dispMessage dispRxTaskMessage;
  struct dispMessage dispTxTaskMessage;

  disp.setPins(15, 16, 17, 1);
  disp.initDisplay(14);

  while (true) {
    //Serial.println("Hello Boss");
    if (xQueueReceive(dispSetQueue, &dispRxTaskMessage, 1) == pdPASS) {
      if (dispRxTaskMessage.cmd == SET_FREQ) {
        dispTxTaskMessage.cmd = SET_FREQ;
        disp.setFrequency(dispRxTaskMessage.value);
        dispTxTaskMessage.ret = 1;
        xQueueSend(dispGetQueue, &dispTxTaskMessage, portMAX_DELAY);
      } else if (dispRxTaskMessage.cmd == DISP_PRINT) {
        dispTxTaskMessage.cmd = DISP_PRINT;
        disp.writeBinStr(dispRxTaskMessage.txt);
        xQueueSend(dispGetQueue, &dispTxTaskMessage, portMAX_DELAY);
      } else if (dispRxTaskMessage.cmd == GET_FREQ) {
        dispTxTaskMessage.cmd = GET_FREQ;
        dispTxTaskMessage.ret = disp.getFrequency();
        xQueueSend(dispGetQueue, &dispTxTaskMessage, portMAX_DELAY);
      } else if (dispRxTaskMessage.cmd == DISP_ENABLE) {
        dispTxTaskMessage.cmd = DISP_ENABLE;
        disp.enable();
        xQueueSend(dispGetQueue, &dispTxTaskMessage, portMAX_DELAY);
      } else if (dispRxTaskMessage.cmd == DISP_DISABLE) {
        dispTxTaskMessage.cmd = DISP_DISABLE;
        disp.disable();
        xQueueSend(dispGetQueue, &dispTxTaskMessage, portMAX_DELAY);
      } else {
        log_i("error");
      }
    }
    disp.loop();
    if (!disp.isEnabled()) {
      sleep(1);
    }
    // audio.loop();
    // if (!audio.isRunning()) {
    // sleep(1);
    // }
  }
}

void displayInit() {
  xTaskCreatePinnedToCore(
    dispTask,              /* Function to implement the task */
    "display",             /* Name of the task */
    display_stack,         /* Stack size in words */
    NULL,                  /* Task input parameter */
    2 | portPRIVILEGE_BIT, /* Priority of the task */
    &display_handle,       /* Task handle. */
    app_core               /* Core where the task should run */
  );
}

struct dispMessage transmitReceive(dispMessage msg) {
  xQueueSend(dispSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(dispGetQueue, &dispRxMessage, portMAX_DELAY) == pdPASS) {
    if (msg.cmd != dispRxMessage.cmd) {
      log_e("wrong reply from message queue");
    }
  }
  return dispRxMessage;
}

void dispSetFreq(uint8_t freq) {
  dispTxMessage.cmd = SET_FREQ;
  dispTxMessage.value = freq;
  dispMessage RX = transmitReceive(dispTxMessage);
}

uint8_t dispGetFreq() {
  dispTxMessage.cmd = GET_FREQ;
  dispMessage RX = transmitReceive(dispTxMessage);
  return RX.ret;
}

void dispEnable() {
  dispTxMessage.cmd = DISP_ENABLE;
  dispMessage RX = transmitReceive(dispTxMessage);
}

void dispDisable() {
  dispTxMessage.cmd = DISP_DISABLE;
  dispMessage RX = transmitReceive(dispTxMessage);
}


void dispPrint(String msg) {
  dispTxMessage.cmd = DISP_PRINT;
  dispTxMessage.txt = msg;
  dispMessage RX = transmitReceive(dispTxMessage);
}

//****************************************************************************************
//                                  END OF DISPLAY PORTION                               *
//****************************************************************************************






//****************************************************************************************
//                                   A U D I O _ T A S K                                 *
//****************************************************************************************
struct audioMessage {
  uint8_t cmd;
  const char* txt;
  uint32_t value;
  uint32_t ret;
  const char* lang;
} audioTxMessage, audioRxMessage;

enum : uint8_t { SET_VOLUME,
                 GET_VOLUME,
                 CONNECTTOHOST,
                 CONNECTTOSD,
                 CONNECTTOSPEECH,
                 GET_AUDIOSTATUS };

QueueHandle_t audioSetQueue = NULL;
QueueHandle_t audioGetQueue = NULL;

void CreateQueues() {
  audioSetQueue = xQueueCreate(10, sizeof(struct audioMessage));
  audioGetQueue = xQueueCreate(10, sizeof(struct audioMessage));
}

void audioTask(void* parameter) {
  CreateQueues();
  if (!audioSetQueue || !audioGetQueue) {
    log_e("queues are not initialized");
    while (true) { ; }  // endless loop
  }

  struct audioMessage audioRxTaskMessage;
  struct audioMessage audioTxTaskMessage;

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(20);  // 0...21

  while (true) {
    if (xQueueReceive(audioSetQueue, &audioRxTaskMessage, 1) == pdPASS) {
      if (audioRxTaskMessage.cmd == SET_VOLUME) {
        audioTxTaskMessage.cmd = SET_VOLUME;
        audio.setVolume(audioRxTaskMessage.value);
        audioTxTaskMessage.ret = 1;
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else if (audioRxTaskMessage.cmd == CONNECTTOHOST) {
        audioTxTaskMessage.cmd = CONNECTTOHOST;
        audioTxTaskMessage.ret = audio.connecttohost(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else if (audioRxTaskMessage.cmd == CONNECTTOSD) {
        audioTxTaskMessage.cmd = CONNECTTOSD;
        audioTxTaskMessage.ret = audio.connecttoSD(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else if (audioRxTaskMessage.cmd == GET_VOLUME) {
        audioTxTaskMessage.cmd = GET_VOLUME;
        audioTxTaskMessage.ret = audio.getVolume();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else if (audioRxTaskMessage.cmd == GET_AUDIOSTATUS) {
        audioTxTaskMessage.cmd = GET_AUDIOSTATUS;
        audioTxTaskMessage.ret = audio.isRunning();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else if (audioRxTaskMessage.cmd == CONNECTTOSPEECH) {
        audioTxTaskMessage.cmd = CONNECTTOSPEECH;
        audioTxTaskMessage.ret = audio.connecttospeech(audioRxTaskMessage.txt, audioRxTaskMessage.lang);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      } else {
        log_i("error");
      }
    }
    audio.loop();
    if (!audio.isRunning()) {
      sleep(1);
    }
  }
}

void audioInit() {
  xTaskCreatePinnedToCore(
    audioTask,             /* Function to implement the task */
    "audioplay",           /* Name of the task */
    5000,                  /* Stack size in words */
    NULL,                  /* Task input parameter */
    2 | portPRIVILEGE_BIT, /* Priority of the task */
    NULL,                  /* Task handle. */
    1                      /* Core where the task should run */
  );
}
struct audioMessage transmitReceive(audioMessage msg) {
  xQueueSend(audioSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(audioGetQueue, &audioRxMessage, portMAX_DELAY) == pdPASS) {
    if (msg.cmd != audioRxMessage.cmd) {
      log_e("wrong reply from message queue");
    }
  }
  return audioRxMessage;
}

void audioSetVolume(uint8_t vol) {
  audioTxMessage.cmd = SET_VOLUME;
  audioTxMessage.value = vol;
  audioMessage RX = transmitReceive(audioTxMessage);
}

uint8_t audioGetVolume() {
  audioTxMessage.cmd = GET_VOLUME;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

bool audioConnecttohost(const char* host) {
  audioTxMessage.cmd = CONNECTTOHOST;
  audioTxMessage.txt = host;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

bool audioConnecttoSD(const char* filename) {
  audioTxMessage.cmd = CONNECTTOSD;
  audioTxMessage.txt = filename;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}
void audio_info(const char* info) {
  //Serial.print("info        ");
  //Serial.println(info);
}
bool audioConnecttoSpeech(const char* speech, const char* language) {
  audioTxMessage.cmd = CONNECTTOSPEECH;
  audioTxMessage.txt = speech;
  audioTxMessage.lang = language;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

bool audiostatus() {
  audioTxMessage.cmd = GET_AUDIOSTATUS;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

//****************************************************************************************
//                                  END OF AUDIO PORTION                                 *
//****************************************************************************************




//****************************************************************************************
//                                   WebShare  _ T A S K                                 *
//****************************************************************************************

struct webshareMessage {
  uint8_t cmd;
  String txt;
  bool status;
  uint32_t value;
  uint32_t ret;
} webshareTxMessage, webshareRxMessage;

enum : uint8_t {
  GETWEBSHARE_ADDRESS,
  WEBSHARE_STATE,
  WEBSHARE_DISABLE,
  WEBSHARE_ENABLE
};

QueueHandle_t webshareSetQueue = NULL;
QueueHandle_t webshareGetQueue = NULL;

void webshareCreateQueues() {
  webshareSetQueue = xQueueCreate(10, sizeof(struct webshareMessage));
  webshareGetQueue = xQueueCreate(10, sizeof(struct webshareMessage));
}

void webshareTask(void* parameter) {
  webshareCreateQueues();
  if (!webshareSetQueue || !webshareGetQueue) {
    log_e("queues are not initialized");
    while (true) { ; }  // endless loop
  }

  struct webshareMessage webshareRxTaskMessage;
  struct webshareMessage webshareTxTaskMessage;
  addFileSystems();
  setupFilemanager();
  while (true) {
    
    if (xQueueReceive(webshareSetQueue, &webshareRxTaskMessage, 1) == pdPASS) {
    if (webshareRxTaskMessage.cmd == WEBSHARE_ENABLE) {
        webshareTxTaskMessage.cmd = WEBSHARE_ENABLE;
        WebShareEnabled=true;
        xQueueSend(webshareGetQueue, &webshareTxTaskMessage, portMAX_DELAY);
      } else if (webshareRxTaskMessage.cmd == WEBSHARE_DISABLE) {
        webshareTxTaskMessage.cmd = WEBSHARE_DISABLE;
        WebShareEnabled=false;
        xQueueSend(webshareGetQueue, &webshareTxTaskMessage, portMAX_DELAY);
      } else {
        log_i("error");
      }
    }
    if(WebShareEnabled){filemgr.handleClient();}
    else sleep(2);
  }
}

void webshareInit() {
  xTaskCreatePinnedToCore(
    webshareTask,              /* Function to implement the task */
    "webshare",             /* Name of the task */
    webshare_stack,         /* Stack size in words */
    NULL,                  /* Task input parameter */
    2 | portPRIVILEGE_BIT, /* Priority of the task */
    &webshare_handle,       /* Task handle. */
    app_core               /* Core where the task should run */
  );
}

struct webshareMessage transmitReceive(webshareMessage msg) {
  xQueueSend(webshareSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(webshareGetQueue, &webshareRxMessage, portMAX_DELAY) == pdPASS) {
    if (msg.cmd != webshareRxMessage.cmd) {
      log_e("wrong reply from message queue");
    }
  }
  return webshareRxMessage;
}




void webshareEnable() {
  webshareTxMessage.cmd = WEBSHARE_ENABLE;
  webshareMessage RX = transmitReceive(webshareTxMessage);
}

void webshareDisable() {
  webshareTxMessage.cmd = WEBSHARE_DISABLE;
  webshareMessage RX = transmitReceive(webshareTxMessage);
}

//****************************************************************************************
//                                  END OF WebShare PORTION                              *
//****************************************************************************************








