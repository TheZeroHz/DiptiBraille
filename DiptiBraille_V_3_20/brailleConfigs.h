#ifndef BRAILLECONFIGS_H
#define BRAILLECONFIGS_H
#include<Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Keypad.h>
#include <WiFi.h>
#include "Audio.h"
#include "esp_random.h"
#include <Update.h>
#include "AudioTools.h"
#include "BleKeyboard.h"
bool KeyBoard_Connected=false;
BleKeyboard bleKeyboard;

#define DEBUG_AUDIO false
#define DEBUG_WEBSHARE true
#define braille_version "3.20"
#define version_type " beta"
String update_url="https://drive.google.com/uc?id=1F_yV9xp4O4s3-oyEsIUqHSSqvUNJXbRn&export=download";
String latest_update_loc="/C/firmware/current/update.bin";
String failsafe_update_loc="/C/firmware/failsafe/update.bin";
String previous_update_loc="/C/firmware/previous/update.bin";

#define BUTTON_PIN_BITMASK 0x100 // 2^8 in hex
// SD CARD
#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12
// I2S Amplifier
#define I2S_DOUT 6
#define I2S_BCLK 5
#define I2S_LRC 4

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

#endif