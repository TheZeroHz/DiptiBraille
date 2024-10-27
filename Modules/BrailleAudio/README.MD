# DiptiBraille Audio I2S ESP32 Integration

This code is part of the **DiptiBraille** project, an ESP32-based braille device designed to provide audio playback capabilities using I2S communication with integrated SD and WiFi functionalities. This setup allows the device to play audio files from both online streams and SD card storage, which can support various features for accessibility, including adjustable volume control and audio file management.

## Features

- **Audio Playback**: Supports audio streaming from both online sources and SD card files.
- **Queue-based Command System**: Uses FreeRTOS queues to handle audio commands, ensuring reliable command exchange.
- **Volume Control**: Allows dynamic setting and retrieval of volume levels.
- **File Management**: Integrates with an SD card to store and play local audio files.
- **WiFi Connectivity**: Connects to online audio streams via WiFi for continuous playback.

## Hardware Requirements

- **ESP32 Module**
- **MicroSD Card Module** (Connected to SPI)
- **I2S-compatible Amplifier or DAC Module** (for audio output)
  
### Pin Configuration

- **SD Card Pins**
  - `SD_CS`: GPIO 10
  - `SPI_MOSI`: GPIO 11
  - `SPI_MISO`: GPIO 13
  - `SPI_SCK`: GPIO 12
- **I2S Audio Pins**
  - `I2S_DOUT`: GPIO 6
  - `I2S_BCLK`: GPIO 5
  - `I2S_LRC`: GPIO 4

## Libraries Required

- [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
- [Audio](https://github.com/schreibfaul1/ESP32-audioI2S) (for audio processing)

## Usage

### 1. Initialization

Upon setup, the code:
1. Initializes the SD card and verifies connection.
2. Connects to WiFi using the specified credentials.
3. Starts the audio task, which handles audio commands using queues.

### 2. Audio Commands

The program uses a struct (`audioMessage`) to communicate commands between tasks, supporting the following commands:
- **SET_VOLUME**: Sets the audio volume (range 0-21).
- **GET_VOLUME**: Retrieves the current volume level.
- **CONNECTTOHOST**: Connects to an online audio stream.
- **CONNECTTOSD**: Plays an audio file from the SD card.

### 3. Serial Commands

You can control the playback and volume through the Serial interface:
- **Audio File Playback**: Send the filename of an audio file stored on the SD card.
- **Volume Control**: Send an integer (0–21) to adjust the volume.

### Code Overview

- **`audioTask()`**: Core function that processes commands from the queue, adjusts volume, and manages audio source connections.
- **`audioInit()`**: Initializes and pins the audio task to core 1, keeping it isolated for efficient handling.
- **`transmitReceive()`**: Sends commands and receives responses through queues, ensuring synchronized communication.
- **`setup()`**: Initializes all peripherals and starts an initial stream from an online host.

### Example Commands

- **Set Volume**: Send an integer (0–21) over Serial.
- **Play Audio from SD**: Send the filename (e.g., `audioFile.mp3`) over Serial.

## Dependencies

Ensure that all required libraries are installed and configured correctly in your environment.
