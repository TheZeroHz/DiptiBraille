#include <Arduino.h>
#include <Battery18650Stats.h>

#define ADC_PIN 3

Battery18650Stats battery(ADC_PIN);

void printBatteryBars(int chargeLevel) {
  Serial.print("Battery Status: [");
  for (int i = 0; i < 5; ++i) {
    if (chargeLevel >= (i + 1) * 20) {
      Serial.print("■");  // Unicode square character
    } else {
      Serial.print(" ");
    }
  }
  Serial.print("]");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int chargeLevel = battery.getBatteryChargeLevel();
  float batteryVolts = battery.getBatteryVolts();

  Serial.print("Volts: ");
  Serial.println(batteryVolts);

  Serial.print("Charge level: ");
  Serial.println(chargeLevel);

  printBatteryBars(chargeLevel);

  delay(2000);
}
