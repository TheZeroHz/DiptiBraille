#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Keypad.h>
#include <WiFi.h>
#include "Audio.h"
#include "esp_random.h"


// SD CARD
#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12
// I2S Amplifier
#define I2S_DOUT 6
#define I2S_BCLK 5
#define I2S_LRC 4

bool disp_state=true;
bool voice_state=true;

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '4', 's', 'e' },
  { '2', '5', 'n', 'u' },
  { '3', '6', 'C', 'c' },
  { '*', 'b', '#', 'd' }
};
byte rowPins[ROWS] = { 35, 36, 37, 38 };
byte colPins[COLS] = { 39, 40, 41, 42 };  // 18-sck, 5-cs, 23-mosi, 19-miso for sd
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


struct language {
  char userdata;
  String kerneldata;
};
language eng[] = {
  { ' ', "E000000" },
  { '!', "E011101" },
  { '\"',"E000010" },
  { '#', "E001111" },
  { '$', "E110101" },
  { '%', "E100101" },
  { '&', "E111101" },
  { '\'',"E001000" },
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
  { '\\',"E110011" },
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
#define wireless_cnt 4
#define settings_cnt 5
String set_data = "E000000", s;
String dataa, buff, phone_number;
const char* ssid = "rakib";
const char* password = "rakib@2023";
bool capslock = false;
bool numlock = false;
bool btn_cancel_isdown = false;
bool btn_up_isdown = false;
bool btn_down_isdown = false;
bool btn_accept_isdown = false;
bool btn_backspace_isdown = false;
bool ctrl= false;

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
                call,
                message,
                language,
                voice_output,
                braille_output,
                touch_pen,
                power };
enum pagetype c_page = root_menu;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  webshareInit();
  audioInit();
  (5000);
  displayInit();
  


  log_i("current display frequency is: %d", dispGetFreq());
  log_i("current volume is: %d", audioGetVolume());

}

void loop() {
  switch (c_page) {
    //root
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
    //sim
    case call: call_page(); break;
    case message: message_page(); break;
    //settings
    case language: language_page(); break;
    case voice_output: voice_output_page(); break;
    case braille_output: braille_output_page(); break;
    case touch_pen: touch_pen_page(); break;
    case power: power_page(); break;
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
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("           welcome to root menu"));
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
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/write.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/read.mp3");
      } else if (menu_selected(3, sub_pos)) {
        audioConnecttoSD("C/menu/eng/audio_book.mp3");
      }
      if (menu_selected(4, sub_pos)) {
        audioConnecttoSD("C/menu/eng/calculator.mp3");
      } else if (menu_selected(5, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/wireless.mp3");
      } else if (menu_selected(6, sub_pos)) {
        audioConnecttoSD("C/menu/eng/sim.mp3");
      } else if (menu_selected(7, sub_pos)) {
        audioConnecttoSD("C/menu/eng/settings.mp3");
      }
    }
    keypress_detect();
    if (btn_down_isdown) {
      if (sub_pos == root_menu_cnt) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
      if (sub_pos == 1) {
        sub_pos = root_menu_cnt;
      } else {
        sub_pos--;
      }
      update_display = true;
      btn_up_isdown = false;
    }

    if (btn_accept_isdown) {
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
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("        doc writing "));
      print_divider();
      print_selected(1, 1);
      Serial.println(F("write to text file"));
      Serial.println();
      print_divider();
    }
    keypress_detect();
    if (btn_accept_isdown) {
      String st = braille_input();
      char udata = kerneldata_to_userdata(st);
      Serial.println(udata);
      if (numlock && numlock_f(udata)) appendFile(SD, "/text.txt", (String)(udata));
      if (!numlock) {
        if (!capslock && (int)udata > 64 && (int)udata < 92) {
          udata = (int)udata + 32;
        }
        appendFile(SD, "/text.txt", (String)(udata));
      }
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
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice number writing
void practice_number_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//practice special charecter writing
void practice_specialcharecter_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_writing;
      return;
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
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
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
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = learn_writing;
      return;
    }
    while (millis() - loopstart < 25) {
      delay(2);
    }
  }
}
//learn special charecter writing
void learn_specialcharecter_writing_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
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
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
       dispClear();
    dispDisable();
      c_page = practice_reading;
      return;
    }
    if(btn_accept_isdown)
    {
  char iv_str[2] = {0};
  getRandomStr(iv_str, 1);
  Serial.print("random char "); Serial.println(iv_str);
  String random_kerneldata=userdata_to_kerneldata((String)iv_str);
  random_kerneldata=random_kerneldata.substring(1);
 if(disp_state){
    dispEnable();
    dispPrint(random_kerneldata);}
    audioConnecttoSD("C/menu/eng/guess_alphabet.mp3");
      String st = braille_input();
      char ch = kerneldata_to_userdata(st);
      if(((String)iv_str).compareTo((String)(ch))==0)
audioConnecttoSD("C/sounds/correct.mp3");
else audioConnecttoSD("C/sounds/wrong.mp3");
 btn_accept_isdown=false;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_reading;
      return;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
      c_page = practice_reading;
      return;
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
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  const char* ch;
  uint8_t sub_pos = 65;
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
    }
    keypress_detect();
    if(btn_accept_isdown)
    {
      String str_data=(String)((char)(sub_pos));
      String data=userdata_to_kerneldata(str_data.c_str());
      String bi_str=data.substring(1);
    String ch_name = "C/en/alp/" + str_data + ".mp3";
    ch = ch_name.c_str();
    audioConnecttoSD(ch);
    Serial.println(bi_str);
    if(disp_state){
    dispEnable();
    dispPrint(bi_str.c_str());}
btn_accept_isdown=false;
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
//learn number reading
void learn_number_reading_page(void) {
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
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
  btn_cancel_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
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
    }
    keypress_detect();
    if (btn_cancel_isdown) {
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
    }
    String page_name = "C/menu/eng/page" + (String)(sub_pos) + ".mp3";
    page_num = page_name.c_str();
    audioConnecttoSD(page_num);
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
      if (sub_pos == 4) {
        sub_pos = 1;
      } else {
        sub_pos++;
      }
      update_display = true;
      btn_down_isdown = false;
    }
    if (btn_up_isdown) {
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
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to wireless section "));
      print_divider();
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
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/wifi.mp3");
      } else if (menu_selected(2, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/bluetooth.mp3");
      }
      else if (menu_selected(3, sub_pos))  //C/en/alp/A.mp3
      {
        audioConnecttoSD("C/menu/eng/web_share.mp3");
      }
      else if (menu_selected(4, sub_pos))  //C/en/alp/A.mp3
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
  while (true) {
    loopstart = millis();
    if (update_display) {
      update_display = false;
      clear_screen();
      Serial.println(F("welcome to wifi section "));
      print_divider();
      print_selected(1, sub_pos);
      Serial.println(F("enter wifi ssid"));
      print_selected(2, sub_pos);
      Serial.println(F("enter wifi password"));
      print_selected(3, sub_pos);
      Serial.println(F("press enter to set credential"));
      Serial.println();
      Serial.println();
      print_divider();
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
          Serial.println(F("                                  *set credential"));
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
        case 1: Serial.println(F("                                  *turned on bluetooth"));return;
        case 2: Serial.println(F("                                  *turned off bluetooth"));return;
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
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/web_share_enable.mp3");
      } else if (menu_selected(2, sub_pos)) 
      {
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
        case 1:webshareEnable();Serial.println(F("                                  webshare enabled"));return;
        case 2: webshareDisable();Serial.println(F("                                webshare disabled"));return;
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
      if (menu_selected(1, sub_pos)) {
        audioConnecttoSD("C/menu/eng/remote_shell_enable.mp3");
      } else if (menu_selected(2, sub_pos))  
      {
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
        case 1: Serial.println(F("                                 remote shell enabled"));return;
        case 2: Serial.println(F("                               remote shell disabled"));return;
      }
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
        btn_backspace_isdown = false;
      }
     if(phone_number.length() == 11)
        audioConnecttoSD("C/menu/eng/make_call.mp3");
      if (ctrl&&btn_accept_isdown) {
          make_call(phone_number);
          btn_accept_isdown = false;
          phone_number = "";
          btn_accept_isdown = false;
          ctrl=false;
      }
     if (btn_cancel_isdown) {
      c_page = sim; return;
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
      Serial.println(F("power"));
      Serial.println();
      Serial.println();
      print_divider();
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
        case 5: c_page = power; return;
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
        case 1:voice_state=true; Serial.println(F("                                  *voice output enabled"));return;
        case 2:voice_state=false; Serial.println(F("                                  *voice output disabled"));return;
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
      Serial.println();
      print_divider();
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
        case 1: disp_state=true;Serial.println(F("                                  *braille output enabled"));return;
        case 2: disp_state=false;Serial.println(F("                                  *braille output disabled"));return;
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
//power
void power_page() {
  btn_cancel_isdown = false;
  btn_up_isdown = false;
  btn_down_isdown = false;
  btn_accept_isdown = false;
  bool update_display = true;
  uint32_t loopstart;
  uint8_t sub_pos = 1;
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
        case 1: Serial.println(F("                                  *power off"));
        case 2: ESP.restart();
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
     if (data.compareTo((String)eng[i].userdata) == 0)
      {
         return eng[i].kerneldata;
      }
  }
}

void getRandomStr(char* output, int len){
    char* eligible_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i = 0; i< len; i++){
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
  int cnt = 0;
  char customKey = customKeypad.getKey();
  if (customKey == 'e') {
    btn_accept_isdown = true;
  }
  if (customKey == 'u') {
    btn_up_isdown = true;
  }
  if (customKey == 'd') {
    btn_down_isdown = true;
  }
  if (customKey == 'c') {
    btn_cancel_isdown = true;
  }
  if (customKey == 'b' && c_page == doc_writing) {
    Serial.println("backspace");
    btn_backspace_isdown = true;
  }
   if (customKey == '*') {
    ctrl = true;
  }
  if (customKey == 'C') {
    capslock = !capslock;
    if (capslock) {
      Serial.println("capslock on");
    }
    if (!capslock) {
      Serial.println("capslock off");
    }
  }
  if (customKey == 'n') {
    numlock = !numlock;
    if (numlock) {
      Serial.println("numlock on");
    }
    if (!numlock) {
      Serial.println("numlock off");
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
    audioConnecttoSD("C/sounds/beep.mp3");
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









