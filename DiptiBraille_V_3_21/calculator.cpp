#include "calculator.h"
void Calculator::reset(){
  hisBuff="";
  resBuffer="";
  inputBuffer="";

}
// Constructor initializes TFT display pointer and clears the input buffer
Calculator::Calculator(TFT_eSPI *display) : tft(display), inputBuffer(""), result(0) {}
String Calculator::getResExpr(){
  String temp=hisBuff+"="+resBuffer;
  hisBuff="";
  resBuffer="";
  return temp;
}
// Method to parse each character entered by the user
void Calculator::parse(char ch) {
   if (ch == '\n') { // Initiate calculation on 'Enter' (newline)
        result = calculate();
        resBuffer=(String)result;
        displayResult();
        hisBuff=inputBuffer;
        inputBuffer = ""; // Clear buffer after calculation
    }
    else if (ch == '\b') { // Handle backspace
        if (!inputBuffer.isEmpty()) {
            inputBuffer.remove(inputBuffer.length() - 1); // Remove last character
            displayInput(); // Update the display after backspace
        }
    }
    else {
        inputBuffer += ch; // Append character to the input buffer
        displayInput();    // Display updated input
    }
}

// Method to evaluate and return the result of the current expression
float Calculator::calculate() {
    return evaluateExpression(inputBuffer);
}

// Displays the current input expression on the TFT screen
void Calculator::displayInput() {
    tft->fillRect(0, 0, 240, 40, TFT_BLACK); // Clear input area
    tft->setTextDatum(MC_DATUM); // Middle Center alignment
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->drawString("Input:", 120, 20, 1);
    tft->drawString(inputBuffer, 120, 40, 1);
}

// Display the result of the calculation on the TFT screen
void Calculator::displayResult() {
    int yOffset = 80;

    tft->fillRect(0, yOffset - 20, 240, 60, TFT_BLACK); // Clear previous result area
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->setTextDatum(MC_DATUM); // Middle Center alignment

    // Display expression and result in center
    tft->drawString("Expression:", 120, yOffset, 1);
    yOffset += 20;
    tft->drawString(inputBuffer, 120, yOffset, 1);
    yOffset += 30; // Space between expression and result
    tft->drawString("Result:", 120, yOffset, 1);
    yOffset += 20;
    tft->drawFloat(result, 2, 120, yOffset, 1); // Display result with 2 decimal places
}

// Display error information on the TFT screen
void Calculator::displayError(int errPos) {
    tft->fillRect(0, 0, 240, 40, TFT_BLACK); // Clear input area
    tft->setTextColor(TFT_WHITE, TFT_RED);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Syntax Error!", 120, 20, 1);
    tft->drawString("Position: " + String(errPos), 120, 40, 1);
    delay(1500); // Show error message briefly
    tft->fillRect(0, 0, 240, 40, TFT_BLACK); // Clear error area after showing
}

// Internal function to evaluate the mathematical expression using TinyExpr
float Calculator::evaluateExpression(const String &expr) {
    te_variable vars[] = {
        {"pi", &pi},
        {"e", &e}
    };

    int err;
    te_expr *expression = te_compile(expr.c_str(), vars, 2, &err);

    if (expression) {
        float result = te_eval(expression);
        te_free(expression);
        return result;
    } else {
        Serial.println("Error parsing expression!");
        displayError(err); // Show error on display
        return NAN;
    }
}
