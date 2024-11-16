#ifndef ECHOPAD_H
#define ECHOPAD_H
#include <TFT_eSPI.h>
#include <SPI.h>
#include<Arduino.h>
// Define some text editor settings
#define MAX_ROWS 10           // Maximum number of rows (adjusted for round display)
#define MAX_COLS 18           // Maximum characters per row (adjust for round display)
#define CURSOR_BLINK_DELAY 500 // Cursor blink delay in ms
#define READER_MODE true
#define WRITER_MODE false
class EchoPad {
public:
  EchoPad(TFT_eSPI& tftInstance);
  void mode(bool M);
  void handleUserInput(char input);
  void blinkCursor();
  void drawTitleBar();
  void drawTextEditor();
  void drawTextReader();
  void resetEditor(); // Reset function declaration
private:
  bool ui_mode=WRITER_MODE;
  TFT_eSPI& tft;
  char textBuffer[MAX_ROWS][MAX_COLS] = { { ' ' } };  // Text buffer initialized with spaces
  int currentRow = 0;                                 // Current row in the editor
  int currentCol = 0;                                 // Current column in the editor

  bool cursorState = true;            // Cursor visibility state
  unsigned long lastCursorBlink = 0;  // For blinking cursor timing
  void drawCursor();
  void drawBorders();
  void drawStatusBar();
  
};

#endif
