> <img src="image/_dipti_logo_.png" width="200" height="100"> 
<img src="image/_diptiV2_layout1_.png" width="400" height="270"><img src="image/_diptiV2_layout3_.png" width="400" height="270">
<p>DiptiBraille is an affordable literacy device for the visually impaired, enabling text input using Braille. It supports document writing, reading, and audio playback for audiobooks. Users can connect an external Braille display for enhanced feedback. Wireless options include WiFi and WebShare for easy data transfer. Essential communication features like calls and SMS are included for seamless interaction.</p>

># Operating System
[DiptiOS 3](https://drive.google.com/drive/folders/1-XRWvRZWpASW5opDc2k0D57xcp3DkUi7?usp=sharing) is a pioneering operating system meticulously crafted to cater to the unique needs of braille users. Rooted in the robust framework of [FreeRTOS](https://www.freertos.org/RTOS.html), DiptiOS/3 sets a new standard for accessibility, seamlessly integrating with braille display hardware to deliver unparalleled functionality and ease of use.

Experience the sophistication of DiptiBraille's sleek design and intuitive interface, which offer both demo and pro versions. <b><i>Unlock the full potential with the Pro Version, priced at $50</i></b>, or explore its capabilities at no cost with the Demo Version.

|✅ available|❌ not available|🔄 upcoming|🟡 partially available|
|------------|-----------------|-----------|-----------------------|

| Features           | Pro| Demo | Features           | Pro|Demo |  Features          | Pro|Demo |
|-------------------------|-----|--------|--------------------|-----|--------|-------------------------|--------|------|
| 📄 Doc Writing | ✅ |✅ |🖋️ Practice Writing | ✅|❌| 📚 Learn Writing| ✅ |🟡|
|📜 Doc Reading | ✅|✅ | 📖 Practice Reading|  ✅ |❌|📘 Learn Reading| ✅ |🟡|
|🎧 Audio Book |✅|✅ |🎵  Music Library  | 🔄 |❌   | 📞 Call|✅|🟡|
|💬 SMS |✅|❌| 🧮 Calculator | ✅|🟡 |⌨️ BLE KeyBoard| ✅ |✅|
|<img src="https://static-00.iconduck.com/assets.00/bluetooth-icon-1365x2048-1dbwtuc9.png" alt="Bluetooth Icon" width="16" height="17">Bluetooth|✅| ❌ | <img src="https://dbservices.com/assets/article/2019/10/filemaker-cloud-1.png" alt="WebShare Icon" width="24" height="24"> WebShare| ✅ |❌| <img src="https://www.unifiedremote.com/remotes/raw/unifiedremote_remotes_master/main_command/icon_hires.png" alt="RemoteShell Icon" width="20" height="20"> RemoteShell|🔄| ❌ |
|<img src="https://camo.githubusercontent.com/e80580d13d9769082fdd40894d586e4c8611da7998068ef2294ba35dd5975b15/68747470733a2f2f662e636c6f75642e6769746875622e636f6d2f6173736574732f313036373930372f313733313732342f37353661333561322d363330662d313165332d383732632d3936323166666364623830322e706e67" alt="WiFi Icon" width="24" height="24"> WiFi Manager| ✅|🟡 |⬇️ Download Manager |  ✅|❌ |📱 Phone Manager| ✅ |❌|
|⚡Continuous Updates |  ✅ |🟡| 🎮 Games | ✅ |🟡 |📻 Radio| ✅ |✅ |
### DiptiOS/3 Ensures
| OS Feature                      | Description                                                                                               |
|------------------------------|-----------------------------------------------------------------------------------------------------------|
| Optimized Braille Support   | Comprehensive support for braille display devices ensures smooth input and output.                        |
| Intuitive Interface         | User-friendly interface tailored for braille users prioritizes ease of navigation.                          |
| Real-Time Responsiveness    | Built on FreeRTOS for instant feedback and interaction.                                                    |
| Customization Options       | Personalize braille experience with customizable settings.                                                  |
| Seamless Connectivity       | Effortless data exchange with other devices and platforms.                                                  |
| Enhanced Accessibility     | Enjoy voice feedback and enhanced audio.                                                             |
| Reliability and Stability  | Backed by FreeRTOS for consistent performance and uptime.                                                   |


># Open Source Hardware
### Printed Circuit Boards
|**Base Material:** FR-4|**Layers:** 2|**Thickness:** 1.6 mm|**Surface:** HASL(with lead)|**Quality:** IPC 2|**Silkscreen:** White|
|-----------------------|-------------|---------------------|----------------------------|------------------|---------------------|

| Main Unit PCB Outlook | Additional Info | **Display Unit PCB Outlook**  | **Additional Info** |
|---------------------------|---------------------------------------------------------|---------------------------|---------------------------------------------------------|
| <img src="image/mainunit.PNG" width="300" height="180"> |<br> **Dim:** 251x148<br> **Color:** Green<br> | <img src="image/displayunit.PNG" width="300" height="180"> |<br>**Dim:** 116x77<br> **Color:** Black |

### 🔧 Components List
| Component                           | Quantity | Component                           | Quantity |
|-------------------------------------|----------|-------------------------------------|----------|
| 💻 ESP32 S3 N16R8                   | x1       | 📶 SIM800L GSM module               | x1       |
| ⌨️ Cherry MX Mechanical keys        | x16      | ⌨️ Mechanical Key Cap               | x16      |
| 🔊 I2S Amplifier                    | x1       | 🔊 Speaker 3W                       | x1       |
| 📇 SD Card Module                   | x1       | 📇 SD Card                          | x1       |
| ⚙️ Motor Driver - TB6612FNG         | x6       | 🔄 Shift Register - 74HC595         | x2       |
| 🔋 4.2v 2000mAh Li-on Battery       | x3       | ⚡ BMS (Circuit Protector)          | x1       |
| ⚡ 5V Constant Step Down BUCK Converter | x2    | 🔌 12v Laptop Charger               | x1       |
| ⚡ Capacitor                        | x2       | ⚡ Diode                            | x20      |
| 🔘 Push Button                      | x10      |                                     |          |

# External Libraries We have Used
- [gpio_viewer](https://github.com/thelastoutpostworkshop/gpio_viewer) by [charlesgiguere](https://github.com/charlesgiguere)
- [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S) by [schreibfaul1](https://github.com/schreibfaul1)
- [ESPFMfGK](https://github.com/holgerlembke/ESPFMfGK) by [Holger Lembke](https://github.com/holgerlembke)
- [WebSerialLite](https://github.com/asjdf/WebSerialLite) by [杨成锴](https://github.com/asjdf)
- [WifiManager](https://github.com/tzapu/WiFiManager) by [tzapu](https://github.com/tzapu)
- [Keypad](https://github.com/Chris--A/Keypad) by [Christopher Andrews](https://github.com/Chris--A)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) by [Me No Dev](https://github.com/me-no-dev)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) by [Me No Dev](https://github.com/me-no-dev)
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) by [T-vK](https://github.com/T-vK)

  We express our gratitude❤️ to these developers for their remarkable libraries.

