#include "ExternalLibrary.h"
#include "brailleConfigs.h"
#include <TFT_eSPI.h>
#include "TFT_GUI.h"
#include "qrcode.h"
#include "EchoPad.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define ITEM_HEIGHT 40
#define ANIMATION_DURATION 500
WiFiManager wm;
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI library
TFT_eSPI tft2=TFT_eSPI();
QRcode qrcode (&tft);
EchoPad echopad(tft2);
TFT_GUI menu(tft, SCREEN_WIDTH, SCREEN_HEIGHT, ITEM_HEIGHT, ANIMATION_DURATION);

String rootmenuItems[] = { "Write", "Read", "Audio Book", "Calculator", "Wireless", "Sim", "Settings" };
//write
String writemenuItems[] = { "Docs writing", "Practice writing", "Learn writing" };
String practiceWritingItems[]={"alphabet","number","special character"};
String learnWritingItems[]={"alphabet","number","special character"};
//read
String readmenuItems[] = { "Docs reading", "Practice reading", "Learn reading" };
String practiceReadingItems[]={"alphabet","number","special character"};
String learnReadingItems[]={"alphabet","number","special character"};
//audiobook
String audiobookItems[] = { "Bengali Book", "English Book", "Math Book" };
//calculator
String calculatorItems[] = { "Addition", "Subtraction", "Multiplication", "Division" };
//wireless
String wirelessmenuItems[] = {"WiFi","Bluetooth","WebShare","RemoteShell","BLE Keyboard"};
//wifi
String wifimenuItems[]={"Enter SSID","Enter Password","Join Network"};
//bluetooth
String bluetoothItems[]={"Turn On","Turn Off"};
//webshare
String websharemenuItems[]={"Enable WebShare","Disable WebShare"};
//remoteshell
String remoteshellItems[]={"Enable remoteshell","Disable remoteshell"};
//sim
String simItems[]={"Call","Message"};
//settings
String settingmenuItems[] = { "Language", "Voice output", "Braille output", "Touch pen", "Update firmware", "Factory Reset", "Power" };
String languagemenuItems[] = { "English", "Bengali" };
String voicemenuItems[] = { "Enable", "Disable" };
String brailleOutputItems[] = { "Enable", "Disable" };
String touchpenItems[] = { "Enable", "Disable" };
String powermenuItems[] = { "Power Off", "Restart" };

const byte CS = 10;
uint64_t sdTotalSize = 0;
uint64_t sdUsedSpace = 0;
const char* sdType = "Unknown";
bool sdMounted = false;


bool disp_state = true;
bool voice_state = true;
bool control_key_hold = false;
bool space_key_hold = false;
int frequency=0;
uint8_t volume = 10;
int brightness = 100;
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'c' },
  { 'e', '6', '5', '4' },
  { 'n', '*', '#', 'b' },
  { 'd', 'u', 's', 'C' }
};

byte rowPins[ROWS] = { 47, 48, 1, 38 }; //45(instant fry your board) should not be used otherwise you'll get fried board
byte colPins[COLS] = { 39, 40, 41, 42 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


struct language {
  char userdata;
  String kerneldata;
};

language eng[] = {
  { ' ', "E000000" },
  { '!', "E011101" },
  { '\"', "E000010" },
  { '#', "E001111" },
  { '$', "E110101" },
  { '%', "E100101" },
  { '&', "E111101" },
  { '\'', "E001000" },
  { '(', "E111011" },
  { ')', "E011111" },
  { '*', "E100001" },
  { '+', "E001101" },
  { ',', "E000001" },
  { '-', "E001001" },
  { '.', "E000101" },
  { '/', "E001100" },
  { '0', "E001011" },
  { '1', "E010000" },
  { '2', "E011000" },
  { '3', "E010010" },
  { '4', "E010011" },
  { '5', "E010001" },
  { '6', "E011010" },
  { '7', "E011011" },
  { '8', "E011001" },
  { '9', "E001010" },
  { ':', "E100011" },
  { ';', "E000011" },
  { '<', "E110001" },
  { '=', "E111111" },
  { '>', "E001110" },
  { '?', "E100111" },
  { '@', "E000100" },
  { 'A', "E100000" },
  { 'B', "E110000" },
  { 'C', "E100100" },
  { 'D', "E100110" },
  { 'E', "E100010" },
  { 'F', "E110100" },
  { 'G', "E110110" },
  { 'H', "E110010" },
  { 'I', "E010100" },
  { 'J', "E010110" },
  { 'K', "E101000" },
  { 'L', "E111000" },
  { 'M', "E101100" },
  { 'N', "E101010" },
  { 'P', "E111100" },
  { 'Q', "E111110" },
  { 'R', "E111010" },
  { 'S', "E011100" },
  { 'T', "E011110" },
  { 'U', "E101001" },
  { 'V', "E111001" },
  { 'W', "E010111" },
  { 'X', "E101101" },
  { 'Y', "E101111" },
  { 'Z', "E101011" },
  { '[', "E010101" },
  { '\\', "E110011" },
  { ']', "E110111" },
  { '^', "E000110" },
  { '_', "E000111" }
};
#define root_menu_cnt 7
#define write_f_cnt 3
#define practice_writing_cnt 3
#define learn_writing_cnt 3
#define read_f_cnt 3
#define practice_reading_cnt 3
#define learn_reading_cnt 3
#define audio_book_cnt 3
#define calculator_cnt 4
#define wireless_cnt 5
#define settings_cnt 7
String set_data = "E000000", s;
String dataa, buff, phone_number;
const char* ssid = "Rakib";
const char* password = "rakib@2024";
bool capslock = false;
bool numlock = false;
bool btn_cancel_isdown = false;
bool btn_up_isdown = false;
bool btn_down_isdown = false;
bool btn_accept_isdown = false;
bool btn_backspace_isdown = false;
bool ctrl = false;
enum pagetype { root_menu,
                write_f,
                read_f,
                audio_book,
                calculator,
                wireless,
                sim,
                settings,
                doc_writing,
                practice_writing,
                learn_writing,
                practice_alphabet_writing,
                practice_number_writing,
                practice_specialcharecter_writing,
                learn_alphabet_writing,
                learn_number_writing,
                learn_specialcharecter_writing,
                doc_reading,
                practice_reading,
                learn_reading,
                practice_alphabet_reading,
                practice_number_reading,
                practice_specialcharecter_reading,
                learn_alphabet_reading,
                learn_number_reading,
                learn_specialcharecter_reading,
                bengali_book,
                english_book,
                math_book,
                addition,
                subtraction,
                multiplication,
                division,
                wifi,
                bluetooth,
                web_share,
                remote_shell,
                ble_keyboard,
                call,
                message,
                language,
                voice_output,
                braille_output,
                touch_pen,
                power,
                update_firmware,
                init_failsafe_protocol };
enum pagetype c_page = root_menu;

void setup() {
  analogWrite(TFT_BL,0);
  Serial.begin(115200);
  tft.init();
  tft.setRotation(4);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_8, 1);

  
  qrcode.init();
  webshareInit();
  audioInit();
  displayInit();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("=");
    delay(10);
  }
  frequency=dispGetFreq();
  audioSetVolume(15);
  log_i("current display frequency is: %d", dispGetFreq());
  log_i("current volume is: %d", audioGetVolume());
  if (!SD.exists("/")) {
    menu.displayHardwareInfo(sdMounted, sdTotalSize, sdUsedSpace, sdType);
    menu.setBright(40);
  }
  else{
  audioConnecttoSD(boot_sound);
  menu.bootUP();
  menu.displayHardwareInfo(sdMounted, sdTotalSize, sdUsedSpace, sdType);
  menu.fadeUP();
  delay(5000);}
  
}

void loop() {
  switch (c_page) {
    
    case root_menu: root_menu_page(); break;
    case write_f: write_f_page(); break;
    case read_f: read_f_page(); break;
    case audio_book: audio_book_page(); break;
    case calculator: calculator_page(); break;
    case wireless: wireless_page(); break;
    case sim: sim_page(); break;
    case settings: settings_page(); break;
    //write
    case doc_writing: doc_writing_page(); break;  //write to text file
    case practice_writing: practice_writing_page(); break;
    case learn_writing: learn_writing_page(); break;
    //practice write
    case practice_alphabet_writing: practice_alphabet_writing_page(); break;
    case practice_number_writing: practice_number_writing_page(); break;
    case practice_specialcharecter_writing: practice_specialcharecter_writing_page(); break;
    //learn write
    case learn_alphabet_writing: learn_alphabet_writing_page(); break;
    case learn_number_writing: learn_number_writing_page(); break;
    case learn_specialcharecter_writing: learn_specialcharecter_writing_page(); break;
    //read
    case doc_reading: doc_reading_page(); break;  //read to text file
    case practice_reading: practice_reading_page(); break;
    case learn_reading: learn_reading_page(); break;
    //practice read
    case practice_alphabet_reading: practice_alphabet_reading_page(); break;
    case practice_number_reading: practice_number_reading_page(); break;
    case practice_specialcharecter_reading: practice_specialcharecter_reading_page(); break;
    //learn read
    case learn_alphabet_reading: learn_alphabet_reading_page(); break;
    case learn_number_reading: learn_number_reading_page(); break;
    case learn_specialcharecter_reading: learn_specialcharecter_reading_page(); break;
    //audio book
    case bengali_book: bengali_book_page(); break;
    case english_book: english_book_page(); break;
    case math_book: math_book_page(); break;
    //calculator
    case addition: addition_page(); break;
    case subtraction: subtraction_page(); break;
    case multiplication: multiplication_page(); break;
    case division: division_page(); break;
    //wireless
    case wifi: wifi_page(); break;
    case bluetooth: bluetooth_page(); break;
    case web_share: web_share_page(); break;
    case remote_shell: remote_shell_page(); break;
    case ble_keyboard:ble_keyboard_page();break;
    //sim
    case call: call_page(); break;
    case message: message_page(); break;
    //settings
    case language: language_page(); break;
    case voice_output: voice_output_page(); break;
    case braille_output: braille_output_page(); break;
    case touch_pen: touch_pen_page(); break;
    case power: power_page(); break;
    case update_firmware: update_firmware_page(); break;
    case init_failsafe_protocol: init_failsafe_protocol_page(); break;
  }
}

//root menu
void root_menu_page(void) {
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  uint8_t prev_sub_pos = 1;  // Store previous selected menu item
  int itemCount = sizeof(rootmenuItems) / sizeof(rootmenuItems[0]);
  menu.setItems(rootmenuItems, itemCount);

  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.print(F("           welcome to root menu  Version-"));
      Serial.print(braille_version);
      Serial.println(version_type);
      //audioConnecttoSD("voice_over/root_menu.mp3");
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("write"));
      print_selected(2, sub_pos);
      Serial.println(F("read"));
      print_selected(3, sub_pos);
      Serial.println(F("audio book"));
      print_selected(4, sub_pos);
      Serial.println(F("calculator"));
      print_selected(5, sub_pos);
      Serial.println(F("wireless"));
      print_selected(6, sub_pos);
      Serial.println(F("sim"));
      print_selected(7, sub_pos);
      Serial.println(F("settings"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/write.mp3");
      } else if (menu_selected(2, sub_pos)) {
        audioConnecttoSD("C/menu/eng/read.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/audio_book.mp3");
      }
      if (menu_selected(4, sub_pos)) {
        audioConnecttoSD("C/menu/eng/calculator.mp3");
      } else if (menu_selected(5, sub_pos)) {
        audioConnecttoSD("C/menu/eng/wireless.mp3");
      } else if (menu_selected(6, sub_pos)) {
        audioConnecttoSD("C/menu/eng/sim.mp3");
      } else if (menu_selected(7, sub_pos)) {
        audioConnecttoSD("C/menu/eng/settings.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      prev_sub_pos = sub_pos;  // Store previous selection
      if (sub_pos == root_menu_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      prev_sub_pos = sub_pos;  // Store previous selection
      if (sub_pos == 1) {
        sub_pos = root_menu_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_accept_isdown) {
      prev_sub_pos = sub_pos;  // Store previous selection before switching page
      switch (sub_pos) {
        case 1: c_page = write_f; return;
        case 2: c_page = read_f; return;
        case 3: c_page = audio_book; return;
        case 4: c_page = calculator; return;
        case 5: c_page = wireless; return;
        case 6: c_page = sim; return;
        case 7: c_page = settings; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}


//write section
void write_f_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(writemenuItems) / sizeof(writemenuItems[0]);
  menu.setItems(writemenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F(" write "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("doc writing"));
      print_selected(2, sub_pos);
      Serial.println(F("practice writing"));
      print_selected(3, sub_pos);
      Serial.println(F("learn writing"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/document_writing.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/practice_writing.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_writing.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == write_f_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = write_f_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }

    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = doc_writing; return;
        case 2: c_page = practice_writing; return;
        case 3: c_page = learn_writing; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}


// doc writing section
void doc_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  tft.fillScreen(TFT_BLACK);
  echopad.drawTitleBar();        // Draw the title bar
  echopad.drawTextEditor();      // Draw the text editor interface
  while (true) {
    echopad.blinkCursor();
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        doc writing "));
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String st = braille_input();
      char udata = kerneldata_to_userdata(st);
      if (numlock && numlock_f(udata)) appendFile(SD, "/text.txt", (String)(udata));
      if (!numlock) {
        if (!capslock && (int)udata > 64 && (int)udata < 92) {
          udata = (int)udata + 32;
        }
        appendFile(SD, "/text.txt", (String)(udata));
      }
      Serial.println(udata);
      echopad.handleUserInput(udata);
      btn_accept_isdown = false;
    }
    if (btn_backspace_isdown) {
      read_for_backspace(SD, "/text.txt");
      int length = s.length();
      s[length - 1] = '\0';
      Serial.println(s);
      writeFile(SD, "/text.txt", s);
      btn_backspace_isdown = false;
    }
    if (btn_cancel_isdown) {
      c_page = write_f;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice writing section
void practice_writing_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(practiceWritingItems) / sizeof(practiceWritingItems[0]);
  menu.setItems(practiceWritingItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        practice writing "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("practice alphabet writing"));
      print_selected(2, sub_pos);
      Serial.println(F("practice number writing"));
      print_selected(3, sub_pos);
      Serial.println(F("practice special charecter writing"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/practice_alphabet_writing.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/practice_number_writing.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/practice_special_character_writing.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == practice_writing_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = practice_writing_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = write_f;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = practice_alphabet_writing; return;
        case 2: c_page = practice_number_writing; return;
        case 3: c_page = practice_specialcharecter_writing; return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice alphabet writing
void practice_alphabet_writing_page(void) {
  const char* ch;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"practice alphabet writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice alphabet writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "alphabet");
      Serial.print("random char ");
      Serial.println(iv_str);
      String ch_name = "C/en/alp/" + (String)iv_str + ".mp3";
      ch = ch_name.c_str();
      /**
      audioConnecttoSD("C/menu/eng/write.mp3");
code_112:
      if (audiostatus()) {
        goto code_112;
      }
     **/
      audioConnecttoSD(ch);
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice number writing
void practice_number_writing_page(void) {
  const char* ch;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
    menu.setItems(new String[1]{"practice number writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice number writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "number");
      Serial.print("random char ");
      Serial.println(iv_str);
      String ch_name = "C/en/num/" + (String)iv_str + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD("C/menu/eng/write.mp3");
/*code_112:
      if (audiostatus()) {
        goto code_112;
      }
      */
      audioConnecttoSD(ch);
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice special charecter writing
void practice_specialcharecter_writing_page(void) {
  const char* ch;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"practice special charecter writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice special charecter writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "special");
      Serial.print("random char ");
      Serial.println(iv_str);
      String ch_name = "C/en/special_char/" + (String)iv_str + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD("C/menu/eng/write.mp3");
/*
code_112:
      if (audiostatus()) {
        goto code_112;
      }*/
      audioConnecttoSD(ch);
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn writing section
void learn_writing_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
   int itemCount = sizeof(learnWritingItems) / sizeof(learnWritingItems[0]);
  menu.setItems(learnWritingItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        learn writing "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("learn alphabet writing"));
      print_selected(2, sub_pos);
      Serial.println(F("learn number writing"));
      print_selected(3, sub_pos);
      Serial.println(F("learn special charecter writing"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_alphabet_writing.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/learn_number_writing.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_special_character_writing.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == learn_writing_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = learn_writing_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = write_f;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = learn_alphabet_writing; return;
        case 2: c_page = learn_number_writing; return;
        case 3: c_page = learn_specialcharecter_writing; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn alphabet writing
void learn_alphabet_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 65;
   menu.setItems(new String[1]{"learn alphabet writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn alphabet writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data);
      String bi_str = data.substring(1);
      String ch_name = "C/en/alp/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      /*
CODE_110:
      if (audiostatus()) {
        goto CODE_110;
      }*/
      Serial.println("Write correct alphabet shown in display:");
      audioConnecttoSD("C/menu/eng/write_displayed_alphabet.mp3");
      String st = braille_input();
      char alp = kerneldata_to_userdata(st);
      if (str_data.compareTo((String)alp) == 0) audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    if (btn_down_isdown) {
      if (sub_pos == 90) {
        sub_pos = 65;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 65) {
        sub_pos = 90;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispDisable();
      c_page = learn_writing;
      return;
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn number writing
void learn_number_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 48;
 menu.setItems(new String[1]{"learn number writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn number writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data);
      String bi_str = data.substring(1);
      String ch_name = "C/en/num/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      /*
CODE_110:
      if (audiostatus()) {
        goto CODE_110;
      }*/
      Serial.println("Write correct number shown in display:");
      audioConnecttoSD("C/menu/eng/write_displayed_number.mp3");
      String st = braille_input();
      char alp = kerneldata_to_userdata(st);
      if (str_data.compareTo((String)alp) == 0) audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    if (btn_down_isdown) {
      if (sub_pos == 57) {
        sub_pos = 48;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 48) {
        sub_pos = 57;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispDisable();
      c_page = learn_writing;
      return;
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn special char writing
void learn_specialcharecter_writing_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 33;
  menu.setItems(new String[1]{"learn special charecter writing"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn special charecter writing"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data);
      String bi_str = data.substring(1);
      String ch_name = "C/en/special_char/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      /*
CODE_110:
      if (audiostatus()) {
        goto CODE_110;
      }
      */
      Serial.println("Write correct special character shown in display:");
      audioConnecttoSD("C/menu/eng/write_displayed_special_char.mp3");
      String st = braille_input();
      char alp = kerneldata_to_userdata(st);
      if (str_data.compareTo((String)alp) == 0) audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    if (btn_down_isdown) {
      if (sub_pos == 47) {
        sub_pos = 33;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 33) {
        sub_pos = 47;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispDisable();
      c_page = learn_writing;
      return;
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//read
void read_f_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(readmenuItems) / sizeof(readmenuItems[0]);
  menu.setItems(readmenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("reading "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("doc reading"));
      print_selected(2, sub_pos);
      Serial.println(F("practice reading"));
      print_selected(3, sub_pos);
      Serial.println(F("learn reading"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/document_reading.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/practice_reading.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_reading.mp3");
      }
    }

    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == read_f_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = read_f_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = doc_reading; return;
        case 2: c_page = practice_reading; return;
        case 3: c_page = learn_reading; return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
// doc reading section
void doc_reading_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"read to text file"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        doc reading "));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("read to text file"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = read_f;
      return;
    }
    if (btn_accept_isdown) {
      readFile(SD, "/text.txt");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice reading section
void practice_reading_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
      int itemCount = sizeof(practiceReadingItems) / sizeof(practiceReadingItems[0]);
  menu.setItems(practiceReadingItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        practice reading "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("practice alphabet reading"));
      print_selected(2, sub_pos);
      Serial.println(F("practice number reading"));
      print_selected(3, sub_pos);
      Serial.println(F("practice special charecter reading"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos-1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/practice_alphabet_reading.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/practice_number_reading.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/practice_special_character_reading.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == practice_reading_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = practice_reading_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = read_f;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = practice_alphabet_reading; return;
        case 2: c_page = practice_number_reading; return;
        case 3: c_page = practice_specialcharecter_reading; return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice alphabet reading
void practice_alphabet_reading_page(void) {
  btn_down_isdown = false;
  btn_up_isdown = false;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  const char* ch;
  uint32_t loopstart;
   menu.setItems(new String[1]{"practice alphabet reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice alphabet reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      dispClear();
      dispDisable();
      c_page = practice_reading;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "alphabet");
      Serial.print("random char ");
      Serial.println(iv_str);
      String random_kerneldata = userdata_to_kerneldata((String)iv_str);
      random_kerneldata = random_kerneldata.substring(1);
      if (disp_state) {
        dispEnable();
        dispPrint(random_kerneldata);
      }
      audioConnecttoSD("C/menu/eng/guess_alphabet.mp3");
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice number reading
void practice_number_reading_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"practice number reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice number reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      dispClear();
      dispDisable();
      c_page = practice_reading;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "number");
      Serial.print("random char ");
      Serial.println(iv_str);
      String random_kerneldata = userdata_to_kerneldata((String)iv_str);
      random_kerneldata = random_kerneldata.substring(1);
      if (disp_state) {
        dispEnable();
        dispPrint(random_kerneldata);
      }
      audioConnecttoSD("C/menu/eng/guess_number.mp3");
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice special charecter reading
void practice_specialcharecter_reading_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
    menu.setItems(new String[1]{"practice special charecter reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("practice special charecter reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      dispClear();
      dispDisable();
      c_page = practice_reading;
      return;
    }
    if (btn_accept_isdown) {
      char iv_str[2] = { 0 };
      getRandomStr(iv_str, 1, "special");
      Serial.print("random char ");
      Serial.println(iv_str);
      String random_kerneldata = userdata_to_kerneldata((String)iv_str);
      random_kerneldata = random_kerneldata.substring(1);
      if (disp_state) {
        dispEnable();
        dispPrint(random_kerneldata);
      }
      audioConnecttoSD("C/menu/eng/guess_special_char.mp3");
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if (((String)iv_str).compareTo((String)(ch)) == 0)
        audioConnecttoSD("C/sounds/correct.mp3");
      else audioConnecttoSD("C/sounds/wrong.mp3");
      btn_accept_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn reading section
void learn_reading_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(learnReadingItems) / sizeof(learnReadingItems[0]);
  menu.setItems(learnReadingItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        learn reading "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("learn alphabet reading"));
      print_selected(2, sub_pos);
      Serial.println(F("learn number reading"));
      print_selected(3, sub_pos);
      Serial.println(F("learn special charecter reading"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_alphabet_reading.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/learn_number_reading.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/learn_special_character_reading.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == learn_reading_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = learn_reading_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = read_f;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = learn_alphabet_reading; return;
        case 2: c_page = learn_number_reading; return;
        case 3: c_page = learn_specialcharecter_reading; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn alphabet reading
void learn_alphabet_reading_page(void) {
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 65;
  menu.setItems(new String[1]{"learn alphabet reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn alphabet reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data.c_str());
      String bi_str = data.substring(1);
      String ch_name = "C/en/alp/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      btn_accept_isdown = false;
    }

    if (btn_down_isdown) {
      if (sub_pos == 90) {
        sub_pos = 65;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 65) {
        sub_pos = 90;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispDisable();
      c_page = learn_reading;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn number reading
void learn_number_reading_page(void) {
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 48;
  menu.setItems(new String[1]{"learn number reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn number reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data.c_str());
      String bi_str = data.substring(1);
      String ch_name = "C/en/num/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      btn_accept_isdown = false;
    }

    if (btn_down_isdown) {
      if (sub_pos == 57) {
        sub_pos = 48;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 48) {
        sub_pos = 57;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispClear();
      dispDisable();
      c_page = learn_reading;
      return;
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn special charecter reading
void learn_specialcharecter_reading_page(void) {
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 33;
   menu.setItems(new String[1]{"learn special charecter reading"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      print_divider();
      print_selected(1, 1);
      Serial.println(F("learn special charecter reading"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String str_data = (String)((char)(sub_pos));
      String data = userdata_to_kerneldata(str_data.c_str());
      String bi_str = data.substring(1);
      String ch_name = "C/en/special_char/" + str_data + ".mp3";
      ch = ch_name.c_str();
      audioConnecttoSD(ch);
      Serial.println(bi_str);
      if (disp_state) {
        dispEnable();
        dispPrint(bi_str.c_str());
      }
      btn_accept_isdown = false;
    }

    if (btn_down_isdown) {
      if (sub_pos == 47) {
        sub_pos = 33;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 33) {
        sub_pos = 47;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    if (btn_cancel_isdown) {
      dispDisable();
      c_page = learn_reading;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//audio_book
void audio_book_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
   int itemCount = sizeof(audiobookItems) / sizeof(audiobookItems[0]);
  menu.setItems(audiobookItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("listen audio book "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("bengali book"));
      print_selected(2, sub_pos);
      Serial.println(F("english book"));
      print_selected(3, sub_pos);
      Serial.println(F("math book"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos-1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/bengali_book.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/english_book.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/math_book.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == audio_book_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = audio_book_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }

    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = bengali_book; return;
        case 2: c_page = english_book; return;
        case 3: c_page = math_book; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void bengali_book_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
 menu.setItems(new String[1]{"bengali book"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        bengali book"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("play bengali.wav"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = audio_book;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void english_book_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  bool update_display = true;
  int sub_pos = 1;
  uint32_t loopstart;
  const char* filename;
  const char* page_num;
  audioConnecttoSD("C/menu/eng/play_english_for_kids.mp3");
   menu.setItems(new String[1]{"english book"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        english book"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("play English for kids"));
      Serial.println();
      print_divider();
       menu.setSelectedItem(0);
    }
    String page_name = "C/en/num/" + (String)(sub_pos) + ".mp3";
    page_num = page_name.c_str();

    keypress_detect();
    if (btn_cancel_isdown) {
      audioConnecttoSD("C/menu/eng/stop.mp3");
      c_page = audio_book;
      return;
    }
    if (btn_accept_isdown) {
      String fname = "D/books/english_ for_kids/" + (String)(sub_pos) + ".mp3";
      filename = fname.c_str();
      audioConnecttoSD(filename);
      btn_accept_isdown = false;
    }
    if (btn_down_isdown) {
      audioConnecttoSD(page_num);
      if (sub_pos == 4) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      audioConnecttoSD(page_num);
      if (sub_pos == 1) {
        sub_pos = 4;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void math_book_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"math book"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        math book"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("play math.wav"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = audio_book;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}

void calculator_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
   int itemCount = sizeof(calculatorItems) / sizeof(calculatorItems[0]);
  menu.setItems(calculatorItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to calculator "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("addition"));
      print_selected(2, sub_pos);
      Serial.println(F("subtraction"));
      print_selected(3, sub_pos);
      Serial.println(F("multiplication"));
      print_selected(4, sub_pos);
      Serial.println(F("divison"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos-1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/addition.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/subtraction.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/multiplication.mp3");
      } else if (menu_selected(4, sub_pos)) {
        audioConnecttoSD("C/menu/eng/division.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == calculator_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = calculator_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }

    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = addition; return;
        case 2: c_page = subtraction; return;
        case 3: c_page = multiplication; return;
        case 4: c_page = division; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void addition_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"addition"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        addition"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("A+B"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = calculator;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void subtraction_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"subtraction"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        subtraction"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("A-B"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = calculator;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void multiplication_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"multiplication"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        multiplication"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("A*B"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = calculator;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void division_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"Division"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        division"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("A/B"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = calculator;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}


void wireless_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(wirelessmenuItems) / sizeof(wirelessmenuItems[0]);
  menu.setItems(wirelessmenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to wireless section "));
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      print_selected(1, sub_pos);
      Serial.println(F("wifi"));
      print_selected(2, sub_pos);
      Serial.println(F("bluetooth"));
      print_selected(3, sub_pos);
      Serial.println(F("web_share"));
      print_selected(4, sub_pos);
      Serial.println(F("remote_shell"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos-1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/wifi.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/bluetooth.mp3");
      } else if (menu_selected(3, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/web_share.mp3");
      } else if (menu_selected(4, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/remote_shell.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == wireless_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = wireless_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = wifi; return;
        case 2: c_page = bluetooth; return;
        case 3: c_page = web_share; return;
        case 4: c_page = remote_shell; return;
        case 5: c_page = ble_keyboard;return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void wifi_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(wifimenuItems) / sizeof(wifimenuItems[0]);
  menu.setItems(wifimenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to wifi section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("Turn Off"));
      print_selected(2, sub_pos);
      Serial.println(F("Turn On"));
      print_selected(3, sub_pos);
      Serial.println(F("Join Network"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/wifi_ssid.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/wifi_password.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 3) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 3;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = wireless;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1:
          Serial.println(F("                                  enter ssid"));
          while (Serial.available() == 0) {
            String ssid = Serial.readString();

            if (ssid != NULL) {
              Serial.println(ssid);
              return;
            }

            if (btn_cancel_isdown) break;
          }

        case 2:
          Serial.println(F("                                  enter password"));
        case 3:
          tft.fillScreen(TFT_BLACK);
          qrcode.create("WIFI:S:DiptiWIFI;T:WPA2;P:USER0011@;;");
          tft.setTextColor(TFT_GREEN,TFT_BLACK);
          tft.setCursor(30,160, 2);
          tft.setTextFont(4);
          tft.println("WiFi Manager");
          wm.startConfigPortal("DiptiWIFI","USER0011@");

      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void bluetooth_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
   int itemCount = sizeof(bluetoothItems) / sizeof(bluetoothItems[0]);
  menu.setItems(bluetoothItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to bluetooth section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("turn on bluetooth"));
      print_selected(2, sub_pos);
      Serial.println(F("turn off bluetooth"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/bluetooth_on.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/bluetooth_off.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = wireless;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: Serial.println(F("                                  *turned on bluetooth")); return;
        case 2: Serial.println(F("                                  *turned off bluetooth")); return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//webshare section
void web_share_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(websharemenuItems) / sizeof(websharemenuItems[0]);
  menu.setItems(websharemenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to webshare section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("webshare enable"));
      print_selected(2, sub_pos);
      Serial.println(F("webshare disable"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/web_share_enable.mp3");
      } else if (menu_selected(2, sub_pos)) {
        audioConnecttoSD("C/menu/eng/web_share_disable.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = wireless;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1:
          webshareEnable();
          tft.fillScreen(TFT_BLACK);
          qrcode.create("http://"+WiFi.localIP().toString()+":8080/");
          tft.setCursor(30,165, 2);
          tft.setTextFont(2);
          tft.setTextColor(TFT_WHITE,TFT_BLACK);
          tft.println("http://"+WiFi.localIP().toString()+":8080/");
          tft.setTextColor(TFT_GREEN,TFT_BLACK);
          tft.setCursor(50,180, 2);
          tft.setTextFont(4);
          tft.println("WebShare");


          Serial.println("Started at port8080 ip");
          Serial.println();
          Serial.println(F("                                  webshare enabled"));
          delay(30000);
          return;
        case 2:
          webshareDisable();
          Serial.println(F("                                webshare disabled"));
          return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//remote shell section
void remote_shell_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(remoteshellItems) / sizeof(remoteshellItems[0]);
  menu.setItems(remoteshellItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to remote shell section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("remote shell enable"));
      print_selected(2, sub_pos);
      Serial.println(F("remote shell disable"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/remote_shell_enable.mp3");
      } else if (menu_selected(2, sub_pos)) {
        audioConnecttoSD("C/menu/eng/remote_shell_disable.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = wireless;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: Serial.println(F("                                 remote shell enabled")); return;
        case 2: Serial.println(F("                               remote shell disabled")); return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}

//BLE_KEYBOARD SECTION
void ble_keyboard_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"[..Starting..]"},1);
  bleKeyboard.begin();
  unsigned long ble_timer=millis();
  while (true) {
    if(millis()-ble_timer>2000 && bleKeyboard.isConnected())menu.setItems(new String[1]{"Connected :-)"},1);
    if(millis()-ble_timer>2000 && !bleKeyboard.isConnected())menu.setItems(new String[1]{"disConnected :-("},1);
    loopstart = millis();
    if (update_display) {
      update_display = false;
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String st = braille_input();
      char udata = kerneldata_to_userdata(st);
      if (numlock && numlock_f(udata))bleKeyboard.write(udata);
      if (!numlock) {
        if (!capslock && (int)udata > 64 && (int)udata < 92) {
          udata = (int)udata + 32;
        }
        bleKeyboard.write(udata);
      }
      Serial.println(udata);
      btn_accept_isdown = false;
    }
    if (btn_backspace_isdown) {
      Serial.println("bkspc");
      bleKeyboard.write(KEY_BACKSPACE);
      btn_backspace_isdown = false;
    }
    if (btn_cancel_isdown) {
      bleKeyboard.end();
      c_page = wireless;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}









//sim section
void sim_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(simItems) / sizeof(simItems[0]);
  menu.setItems(simItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to sim section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("call"));
      print_selected(2, sub_pos);
      Serial.println(F("message"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/call.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/message.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = call; return;
        case 2: c_page = message; return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void call_page() {
  btn_accept_isdown = false;
  btn_cancel_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* num;
  audioConnecttoSD("C/menu/eng/input_number.mp3");
 menu.setItems(new String[1]{"Call"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        call"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("call to x"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      numlock = true;
      String st = braille_input();
      char digit = kerneldata_to_userdata(st);
      if (numlock && numlock_f(digit)) phone_number += (String)(digit);
      Serial.println(phone_number);
      String num_name = "C/en/num/" + (String)(digit) + ".mp3";
      num = num_name.c_str();
      audioConnecttoSD(num);
      btn_accept_isdown = false;
    }
    if (btn_backspace_isdown) {
      int length = phone_number.length();
      phone_number[length - 1] = '\0';
      Serial.println("Backspace pressed. phone num:" + phone_number);
      btn_backspace_isdown = false;
    }
    if (phone_number.length() == 11)
      audioConnecttoSD("C/menu/eng/make_call.mp3");
    if (ctrl) {
      make_call(phone_number);
      btn_accept_isdown = false;
      phone_number = "";
      btn_accept_isdown = false;
      ctrl = false;
    }
    if (btn_cancel_isdown) {
      c_page = sim;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
void message_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
   menu.setItems(new String[1]{"Message"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        message"));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("message to x"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = sim;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//settings
void settings_page(void) {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(settingmenuItems) / sizeof(settingmenuItems[0]);
  menu.setItems(settingmenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to settings "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("language"));
      print_selected(2, sub_pos);
      Serial.println(F("voice output"));
      print_selected(3, sub_pos);
      Serial.println(F("braille output"));
      print_selected(4, sub_pos);
      Serial.println(F("touch pen"));
      print_selected(5, sub_pos);
      Serial.println(F("update firmware"));
      print_selected(6, sub_pos);
      Serial.println(F("initiate failsafe protocol"));
      print_selected(7, sub_pos);
      Serial.println(F("power"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/language.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/voice_output.mp3");
      } else if (menu_selected(3, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/braille_output.mp3");
      } else if (menu_selected(4, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/touch_pen.mp3");
      } else if (menu_selected(5, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/update_firmware.mp3");
      } else if (menu_selected(6, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/initiate_failsafe_protocol.mp3");
      } else if (menu_selected(7, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/power.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == settings_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = settings_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = root_menu;
      return;
      ;
    }

    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: c_page = language; return;
        case 2: c_page = voice_output; return;
        case 3: c_page = braille_output; return;
        case 4: c_page = touch_pen; return;
        case 5: c_page = update_firmware; return;
        case 6: c_page = init_failsafe_protocol; return;
        case 7: c_page = power; return;
      }
    }

    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//language
void language_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(languagemenuItems) / sizeof(languagemenuItems[0]);
  menu.setItems(languagemenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to language section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("english language"));
      print_selected(2, sub_pos);
      Serial.println(F("bangla language"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/english_language.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/bengali_language.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: set_data = "E000000"; return;
        case 2: set_data = "B000000"; return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//voice output
void voice_output_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(voicemenuItems) / sizeof(voicemenuItems[0]);
  menu.setItems(voicemenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to voice output section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F(" voice output enable"));
      print_selected(2, sub_pos);
      Serial.println(F("voice output disable"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/voice_enable.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/voice_disable.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1:
          voice_state = true;
          Serial.println(F("                                  *voice output enabled"));
          return;
        case 2:
          voice_state = false;
          Serial.println(F("                                  *voice output disabled"));
          return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//braille output
void braille_output_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(brailleOutputItems) / sizeof(brailleOutputItems[0]);
  menu.setItems(brailleOutputItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to braille output section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F(" braille display output enable"));
      print_selected(2, sub_pos);
      Serial.println(F("braille display output disable"));
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/display_enable.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/display_disable.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1:
          disp_state = true;
          Serial.println(F("                                  *braille output enabled"));
          return;
        case 2:
          disp_state = false;
          Serial.println(F("                                  *braille output disabled"));
          return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//touch pen section
void touch_pen_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
    int itemCount = sizeof(touchpenItems) / sizeof(touchpenItems[0]);
  menu.setItems(touchpenItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to touch pen section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F(" touch pen enable"));
      print_selected(2, sub_pos);
      Serial.println(F("touch pen disable"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/touch_pen_enable.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/touch_pen_disable.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1: Serial.println(F("                                  *pen input enabled"));
        case 2: Serial.println(F("                                  *pen input disabled"));
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//update_firmware section
void update_firmware_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
menu.setItems(new String[1]{"update firmware"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        update firmware"));
      print_divider();
      audioConnecttoSD("C/menu/eng/activate_update_firmware.mp3");
      Serial.println();
      print_divider();
      menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      downloadUpdate();
      updateFromFS(SD, latest_update_loc);
      btn_accept_isdown = false;
    }
    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//failsafe section
void init_failsafe_protocol_page() {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  menu.setItems(new String[1]{"failsafe protocol"}, 1);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        failsafe protocol"));
      print_divider();
      audioConnecttoSD("C/menu/eng/activate_failsafe_protocol.mp3");
      Serial.println();
      print_divider();
        menu.setSelectedItem(0);
    }
    keypress_detect();
    if (btn_accept_isdown) {
      updateFromFS(SD, failsafe_update_loc);
      btn_accept_isdown = false;
    }
    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//power
void power_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
  int itemCount = sizeof(powermenuItems) / sizeof(powermenuItems[0]);
  menu.setItems(powermenuItems, itemCount);
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome power section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F(" power off"));
      print_selected(2, sub_pos);
      Serial.println(F("restart"));
      Serial.println();
      Serial.println();
      print_divider();
      menu.setSelectedItem(sub_pos - 1);
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/power_off.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/restart.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == 2) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = 2;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_cancel_isdown) {
      c_page = settings;
      return;
    }
    if (btn_accept_isdown) {
      switch (sub_pos) {
        case 1:
          Serial.println(F("Shutting Down..."));
        audioConnecttoSD(shutdown_sound);
        menu.drawSdJpeg("/boot1.jpg",0,0);
        delay(4000);
        menu.fadeDown();
        menu.off();
        delay(5);
          esp_deep_sleep_start();
          return;
        case 2:
        Serial.println(F("Restarting..."));
        audioConnecttoSD(shutdown_sound);
        menu.drawSdJpeg("/boot1.jpg",0,0);
        delay(4000);
        menu.fadeDown();
        menu.off();
        delay(5);
        ESP.restart();
         return;
      }
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}

void print_selected(uint8_t x, uint8_t y) {
  if (x == y) Serial.print(F("->  "));
  else Serial.print(F("    "));
}
bool menu_selected(uint8_t x, uint8_t y) {
  if (x == y) return true;
  else return false;
}
void clear_screen() {
  for (uint8_t i = 0; i < 100; i++) Serial.println();
}
void print_divider() {
  for (uint8_t i = 0; i < 20; i++) Serial.print("--");
  Serial.println();
}
String braille_input() {
  String data = set_data;
  for (;;) {
    if (customKeypad.getKeys()) {
      for (int i = 0; i < LIST_MAX; i++)  // Scan the whole key list.
      {
        if (customKeypad.key[i].stateChanged)  // Only find keys that have changed state.
        {
          switch (customKeypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
            case PRESSED:
              char ch = customKeypad.key[i].kchar;
              if (ch > 48 && ch < 55)
                data[((String)ch).toInt()] = '1';
              else if (customKeypad.key[i].kchar == 'e') {
                return data;
              }
              break;
          }
        }
      }
    }
  }
}
char kerneldata_to_userdata(String data) {
  for (int i = 0; i < sizeof(eng) / sizeof(eng[0]); i++) {
    if (data.compareTo(eng[i].kerneldata) == 0) {
      //if(eng[i].userdata!="B000000")
      return eng[i].userdata;
    }
  }
}
String userdata_to_kerneldata(String data) {
  for (int i = 0; i < sizeof(eng) / sizeof(eng[0]); i++) {
    if (data.compareTo((String)eng[i].userdata) == 0) {
      return eng[i].kerneldata;
    }
  }
}
void getRandomStr(char* output, int len, String type) {
  char* eligible_chars;
  if (type.compareTo("alphabet") == 0) {
    eligible_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  } else if (type.compareTo("number") == 0) {
    eligible_chars = "0123456789";
  } else if (type.compareTo("special") == 0) {
    eligible_chars = "@$#%^&*()_-+=?><:,;\/";
  }

  for (int i = 0; i < len; i++) {
    uint8_t random_index = random(0, strlen(eligible_chars));
    output[i] = eligible_chars[random_index];
  }
  //Serial.print("Random String: "); Serial.println(output);
}

void readFile(fs::FS& fs, const char* path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  s = "";  //audiostatus()
  int cnt = 0;
  while (file.available()) {
    char charRead = file.read();
    s += (String)charRead;
    if (charRead == ' ') cnt++;
    if (charRead == '.' || cnt == 25 || !file.available()) {
      audioConnecttoSpeech(s.c_str(), "en");
      s = "";
      cnt = 0;
      while (audiostatus()) { Serial.println("audio playing"); }
    }
  }
  Serial.println(s);
  file.close();
}

void writeFile(fs::FS& fs, const char* path, String message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }

  file.close();
}

void appendFile(fs::FS& fs, const char* path, String message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void keypress_detect() {
  char customKey;
  int cnt;
  if (customKeypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++)  // Scan the whole key list.
    {
      if (customKeypad.key[i].stateChanged)  // Only find keys that have changed state.
      {
        switch (customKeypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            cnt = 0;
            customKey = customKeypad.key[i].kchar;
            if (customKey == 'e') {
              btn_accept_isdown = true;
            } else if (customKey == 'u') {
              if (control_key_hold&&!space_key_hold) {
                volume = volume + 1;
                if (volume > 21) volume = 21;
                audioSetVolume(volume);
                if(!audiostatus())audioConnecttoSD("/C/sounds/beep.mp3");
                Serial.println(audioGetVolume());
              }
              else if(control_key_hold&&space_key_hold){
                frequency=frequency+5;
                dispSetFreq((uint8_t)(frequency));
                 Serial.println(dispGetFreq());
              }
               else if (space_key_hold&&!control_key_hold) {
                brightness = brightness + 20;
                if (brightness > 100) brightness = 100;
                Serial.println(brightness);
                menu.setBright(brightness);
              } else if (!control_key_hold || !space_key_hold) btn_up_isdown = true;
            } else if (customKey == 'd') {
              if (control_key_hold&&!space_key_hold) {
                volume = volume - 1;
                if (volume < 1) volume = 1;
                audioSetVolume(volume);
                if(!audiostatus())audioConnecttoSD("/C/sounds/beep.mp3");
                Serial.println(audioGetVolume());
              }
              else if(control_key_hold&&space_key_hold){
                frequency=frequency-5;
                dispSetFreq((uint8_t)(frequency));
                Serial.println(dispGetFreq());
              }
              else if (space_key_hold&&!control_key_hold) {
                brightness = brightness - 20;
                if (brightness < 0) brightness =0;
                Serial.println(brightness);
                menu.setBright(brightness);
              } else if (!control_key_hold) btn_down_isdown = true;
            } else if (customKey == 'c') {
              btn_cancel_isdown = true;
            } else if (customKey == 'b' && (c_page == doc_writing||c_page == ble_keyboard || c_page == call)) {
              Serial.println("backspace");
              btn_backspace_isdown = true;
            } else if (customKey == '*') {
              ctrl = true;
            } else if (customKey == 'C') {
              capslock = !capslock;
              if (capslock) {
                if(!audiostatus())audioConnecttoSD("/C/sounds/beep.mp3");
                Serial.println("capslock on");
              }
              if (!capslock) {
                if(!audiostatus())audioConnecttoSD("/C/sounds/beep.mp3");
                Serial.println("capslock off");
              }
            } else if (customKey == 'n') {
              numlock = !numlock;
              if (numlock) {
                Serial.println("numlock on");
              }
              if (!numlock) {
                Serial.println("numlock off");
              }
            }
            break;
          case HOLD:
            customKey = customKeypad.key[i].kchar;
            if (customKey == '*') control_key_hold = true;
            if (customKey == 's') space_key_hold = true;
            break;
          case RELEASED:
            customKey = customKeypad.key[i].kchar;
            if (customKey == '*') control_key_hold = false;
            if (customKey == 's') space_key_hold = false;
            break;
        }
      }
    }
  }
}
//****************************************************************************************
//                                          GSM
//****************************************************************************************
void send_sms(String phn_num, String msg) {

  Serial2.println("AT+CMGF=1");
  waitForResponse();

  Serial2.println("AT+CNMI=1,2,0,0,0");

  waitForResponse();
  String msg_mode = "AT+CMGS=\"" + phn_num + "\"\r";
  Serial2.print(msg_mode);
  waitForResponse();
  Serial2.print(msg);
  Serial2.write(0x1A);
  waitForResponse();
}


void make_call(String phn_num) {
  phn_num = "ATD" + phn_num + ";";
  Serial.println("calling..." + phn_num);
  Serial2.println(phn_num);
  delay(40000);
  waitForResponse();
}

void waitForResponse() {
  delay(1000);
  while (Serial2.available()) {
    buff = Serial2.readString();
    Serial.print(buff);
  }
  //Serial2.read();
}
//****************************************************************************************
//                        numlock & backspace                                            *
//****************************************************************************************
bool numlock_f(char udata) {
  if (isDigit(udata))
    return true;
  else {
    if(!audiostatus())audioConnecttoSD("/C/sounds/beep.mp3");
    return false;
  }
}
void read_for_backspace(fs::FS& fs, const char* path) {
  File file = fs.open(path);
  Serial.print("Read from file: ");
  s = "";
  while (file.available()) {
    char charRead = file.read();
    s += (String)charRead;
    //Serial.write(file.read());
  }
  Serial.println(s);
  file.close();
}
