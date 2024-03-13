#ifndef BRAILLECONFIGS_H
#define BRAILLECONFIGS_H
#include<Arduino.h>
#define DEBUG_AUDIO true
#define DEBUG_WEBSHARE true
#define braille_version "3.18"
String update_url="https://drive.google.com/uc?id=1F_yV9xp4O4s3-oyEsIUqHSSqvUNJXbRn&export=download";
String latest_update_loc="/C/firmware/current/update.bin";
String failsafe_update_loc="/C/firmware/failsafe/update.bin";
String previous_update_loc="/C/firmware/previous/update.bin";
// SD CARD
#define SD_CS 10
#define SPI_MOSI 11
#define SPI_MISO 13
#define SPI_SCK 12
// I2S Amplifier
#define I2S_DOUT 6
#define I2S_BCLK 5
#define I2S_LRC 4
#endif