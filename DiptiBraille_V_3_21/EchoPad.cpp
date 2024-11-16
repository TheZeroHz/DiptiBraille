#include "EchoPad.h"
void EchoPad::mode(bool M){
  ui_mode=M;
}
EchoPad::EchoPad(TFT_eSPI& tftInstance):tft(tftInstance){}

void EchoPad::handleUserInput(char input){
    switch (input) {
      case '^':  // Move cursor up
        if (currentRow > 0) currentRow--;
        break;

      case '_':  // Move cursor down
        if (currentRow < MAX_ROWS - 1) currentRow++;
        break;

      case '<':  // Move cursor left
        if (currentCol > 0) {
          currentCol--;
        } else if (currentRow > 0) { // If at the start of a row, move to the end of the previous row
          currentRow--;
          currentCol = MAX_COLS - 1;
        }
        break;

      case '>':  // Move cursor right
        if (currentCol < MAX_COLS - 1) {
          currentCol++;
        } else if (currentRow < MAX_ROWS - 1) { // If at the end of a row, move to the start of the next row
          currentRow++;
          currentCol = 0;
        }
        break;

      case '\n':  // Handle newline (Enter)
        if (currentRow < MAX_ROWS - 1) {
          currentRow++;
          currentCol = 0; // Move to the next line
        }
        break;

      case '\b':  // Handle backspace
        if (currentCol > 0) {
          currentCol--;
          textBuffer[currentRow][currentCol] = ' ';
        } else if (currentRow > 0 && currentCol == 0) {
          // Move back to the previous line if we're at the start of a row
          currentRow--;
          currentCol = MAX_COLS - 1;  // Move to the end of the previous line
          textBuffer[currentRow][currentCol] = ' ';
        }
        break;

      default:
        // Printable ASCII characters
        if (input >= 32 && input <= 126) {
          textBuffer[currentRow][currentCol] = input;
          currentCol++;
          // Wrap to next row if the current column exceeds the maximum columns
          if (currentCol >= MAX_COLS) {
            if (currentRow < MAX_ROWS - 1) {
              currentRow++;
              currentCol = 0;
            } else {
              // Handle overflow by staying at the last column
              currentCol = MAX_COLS - 1;
            }
          }
        }
        break;
    }

   if(ui_mode==WRITER_MODE) drawTextEditor();  // Refresh the display after each input
   else if(ui_mode==READER_MODE)drawTextReader();
}




// Function to blink the cursor
void EchoPad::blinkCursor() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastCursorBlink >= CURSOR_BLINK_DELAY) {
    cursorState = !cursorState;  // Toggle cursor state

    // Draw only the cursor
    drawCursor();
    lastCursorBlink = currentMillis;
  }
}
void EchoPad::drawTextEditor() {
  // Draw borders and layout
  drawBorders();

  // Loop through the text buffer and display each character
  for (int row = 0; row < MAX_ROWS; row++) {
    for (int col = 0; col < MAX_COLS; col++) {
      char ch = textBuffer[row][col];
      tft.setCursor(30 + col * 10, 60 + row * 14);  // Adjust positions for text
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print(ch);
    }
  }

  // Draw the cursor
  drawCursor();

  // Update the status bar with the current row and column
  drawStatusBar();
}

void EchoPad::drawTextReader() {
  // Draw borders and layout
  drawTitleBar();
  drawBorders();
  // Loop through the text buffer and display each character
  for (int row = 0; row < MAX_ROWS; row++) {
    for (int col = 0; col < MAX_COLS; col++) {
      char ch = textBuffer[row][col];
      tft.setCursor(30 + col * 10, 60 + row * 14);  // Adjust positions for text
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print(ch);
    }
  }
}


// Function to draw the cursor
void EchoPad:: drawCursor() {
  // Draw the cursor with a different color
  tft.setCursor(30 + currentCol * 10, 60 + currentRow * 14); // Adjust cursor position
  if (cursorState) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);  // Cursor in yellow color
    tft.print("I");  // Draw cursor as an underscore
  } else {
    // Erase the cursor by drawing the current character over it
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print(textBuffer[currentRow][currentCol]);
  }
}


// Function to draw the title bar
void EchoPad::drawTitleBar() {
  tft.fillRect(0, 0, 240, 30, TFT_BLUE);   // Title bar with blue background
  tft.setTextColor(TFT_WHITE, TFT_BLUE);   // White text
  tft.setTextFont(2);
  tft.setCursor(90, 10);
  tft.print("EchoPad");
}

// Function to draw borders around the text area (adjusted for round TFT)
void EchoPad::drawBorders() {
  tft.drawRect(25, 50, 185, 160, TFT_WHITE); // Inner rectangular border for text
}

// Function to draw a status bar at the bottom
void EchoPad::drawStatusBar() {
  tft.fillRect(0, 210, 240, 30, TFT_DARKGREEN);  // Status bar background
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setCursor(80, 215);
  tft.printf("Row: %d, Col: %d", currentRow + 1, currentCol + 1);  // Show row and column
}

void EchoPad::resetEditor() {
  // Clear the text buffer
  for (int row = 0; row < MAX_ROWS; row++) {
    for (int col = 0; col < MAX_COLS; col++) {
      textBuffer[row][col] = ' ';
    }
  }

  // Reset cursor position
  currentRow = 0;
  currentCol = 0;
  cursorState = true;

  if(ui_mode==WRITER_MODE) drawTextEditor();  // Refresh the display after each input
  else if(ui_mode==READER_MODE)drawTextReader();
}