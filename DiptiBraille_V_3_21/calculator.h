#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "tinyexpr.h"

class Calculator {
public:
    // Constructor that takes a TFT display pointer
    Calculator(TFT_eSPI *display);
    // Methods
    void parse(char ch);          // Parses each character input
    float calculate();             // Initiates calculation of the current expression
    void displayInput();           // Displays the current input on the TFT
    void displayResult();          // Displays the result on the TFT
    void displayError(int errPos); // Displays error messages
    String getExpr();
    String getRes();
    String getResExpr();
    void reset();

private:
    // Variables
    TFT_eSPI *tft;            // Pointer to the TFT display
    String inputBuffer,hisBuff;       // Stores the current input expression
    float result;             // Stores the result of the calculation
    String resBuffer;
    // Constants for common mathematical values
    double pi = 3.14159265359;
    double e = 2.71828182846;

    // Internal function for evaluating expressions
    float evaluateExpression(const String &expr);
};

#endif // CALCULATOR_H
