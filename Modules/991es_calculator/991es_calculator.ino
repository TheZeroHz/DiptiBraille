#include <TFT_eSPI.h>
#include "tinyexpr.h"

TFT_eSPI tft = TFT_eSPI();
String inputBuffer = ""; // Stores the current input expression
bool newCalculation = true;

// Define constants as double variables
double pi = 3.14159265359;
double e = 2.71828182846;

void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(0); // Set appropriate rotation for round display
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
}

void loop() {
    String expression = getUserInput(); // Collect user input dynamically
    if (expression != "") {
        tft.fillScreen(TFT_BLACK);
        float result = evaluateExpression(expression); // Calculate result
        displayResult(expression, result);             // Display on screen
        newCalculation = true;                         // Reset for new calculation
    }
}

// Function to simulate getting user input (replace with keypad or serial)
String getUserInput() {
    if (Serial.available()) {
        if (newCalculation) {
            inputBuffer = "";
            newCalculation = false;
        }
        char c = Serial.read();
        if (c == '\n') { // Press Enter to evaluate
            String expr = inputBuffer;
            inputBuffer = "";
            return expr;
        } else {
            inputBuffer += c; // Append character to buffer
            displayInput(inputBuffer); // Show on TFT display
        }
    }
    return "";
}

// Display user input on screen
void displayInput(const String &input) {
    tft.fillRect(0, 0, 240, 40, TFT_BLACK); // Clear input area
    tft.setTextDatum(MC_DATUM); // Middle Center
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Input:", 120, 20, 1);
    tft.drawString(input, 120, 40, 1);
}

// Evaluate mathematical expression using TinyExpr library
float evaluateExpression(const String &expr) {
    te_variable vars[] = {
        {"pi", &pi},
        {"e", &e}
        // Add more variables here if needed
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

// Display error on TFT
void displayError(int errPos) {
    tft.fillRect(0, 0, 240, 40, TFT_BLACK); // Clear input area
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Syntax Error!", 120, 20, 1);
    tft.drawString("Position: " + String(errPos), 120, 40, 1);
    delay(1500); // Show error message briefly
    tft.fillRect(0, 0, 240, 40, TFT_BLACK); // Clear error area after showing
}

// Display result on screen
void displayResult(const String &expression, float result) {
    int yOffset = 80; // Starting Y position for result display

    tft.fillRect(0, yOffset - 20, 240, 60, TFT_BLACK); // Clear previous result area
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(MC_DATUM); // Middle Center alignment

    // Display expression and result in center
    tft.drawString("Expression:", 120, yOffset, 1);
    yOffset += 20;
    tft.drawString(expression, 120, yOffset, 1);
    yOffset += 30; // Space between expression and result
    tft.drawString("Result:", 120, yOffset, 1);
    yOffset += 20;
    tft.drawFloat(result, 2, 120, yOffset, 1); // Display result with 2 decimal places
}

// Additional Function Examples (expand as needed)
float add(float a, float b) { return a + b; }
float subtract(float a, float b) { return a - b; }
float multiply(float a, float b) { return a * b; }
float divide(float a, float b) { return (b != 0) ? a / b : NAN; }

// Custom Functions (trigonometry, logs, etc.)
float sinDeg(float degrees) { return sin(degrees * PI / 180); }
float cosDeg(float degrees) { return cos(degrees * PI / 180); }
float tanDeg(float degrees) { return tan(degrees * PI / 180); }
float logBase10(float x) { return log10(x); }
float naturalLog(float x) { return log(x); }
float power(float base, float exp) { return pow(base, exp); }
