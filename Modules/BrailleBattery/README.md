# Battery Level Monitoring for 18650 Battery on ESP32

This code provides a simple method to monitor the charge level of a 18650 battery using the **Battery18650Stats** library and display the battery status as a visual bar chart in the serial monitor.

## Features

- **Battery Voltage Monitoring**: Reads the battery voltage and calculates the charge level.
- **Battery Charge Level Display**: Displays a graphical bar (0 to 5 bars) representing the charge level in the serial monitor.
- **Adjustable Polling Interval**: Updates the battery status every 2 seconds.

## Requirements

- **ESP32 Board** with Analog-to-Digital Converter (ADC) pin support.
- **Battery18650Stats Library**: This library is used for reading and interpreting battery charge levels based on voltage.
- **18650 Battery** connected to an ADC pin through a voltage divider if needed, depending on the ADC's range.

## Pin Configuration

- **ADC_PIN**: `GPIO 3` - Connected to the battery's output (through a voltage divider if necessary).

## Code Explanation

### Functions

- **`printBatteryBars(int chargeLevel)`**: Prints a graphical representation of the battery charge level in the form of a bar made of 5 segments.
  - Each segment represents 20% of the battery's charge.
  - A filled square (`■`) indicates 20% increments in charge, while an empty space represents remaining segments.

### Main Loop

1. **Reads Battery Charge and Voltage**:
   - `battery.getBatteryChargeLevel()` returns the battery's charge level as a percentage.
   - `battery.getBatteryVolts()` returns the current battery voltage.
   
2. **Displays Data**:
   - Prints the voltage and charge level in the serial monitor.
   - Calls `printBatteryBars()` to print a bar display of the charge status.

3. **Delay**: Pauses for 2 seconds before refreshing the values.

## Example Output

The output will look like this in the serial monitor:

```
Volts: 3.7
Charge level: 60
Battery Status: [■■■  ]
```

## Code

```cpp
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
```

## Notes

- Make sure the `Battery18650Stats` library is installed and configured correctly.
- Ensure that the battery voltage does not exceed the maximum input voltage of the ADC pin used.
- Modify `ADC_PIN` if using a different analog pin.

This setup provides a straightforward way to monitor the battery level in your projects, making it ideal for battery-powered devices like remote sensors or portable gadgets.