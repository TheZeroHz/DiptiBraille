#include <Arduino.h>
#include "qrcode.h"
#include "qrencode.h"

int offsetsX;
int offsetsY;
int screenwidth;
int screenheight;
int multiply=2;

QRcode::QRcode(TFT_eSPI *tft){
  this->tft = tft;
}

void QRcode::init(){
    //((TFT_eSPI *)tft)->init();
    screenwidth = 130;
    screenheight =130; 
    //tft->setRotation(1);
    //tft->fillScreen(TFT_BLACK);
    int min = screenwidth;
    if (screenheight<screenwidth)
      min = screenheight;
    multiply = min/WD;
    offsetsX = 60;
    offsetsY = 60;

}

void QRcode::render(int x, int y, int color){
  x=(x*multiply)+offsetsX;
  y=(y*multiply)+offsetsY;
  if(color==1) {
    tft->drawPixel(x,y,TFT_GREEN);
    if (multiply>1) {
      tft->drawPixel(x+1,y,TFT_GREEN);
      tft->drawPixel(x+1,y+1,TFT_GREEN);
      tft->drawPixel(x,y+1,TFT_GREEN);
    }
  }
  else {
    tft->drawPixel(x,y,TFT_BLACK);
    if (multiply>1) {
      tft->drawPixel(x+1,y,TFT_BLACK);
      tft->drawPixel(x+1,y+1,TFT_BLACK);
      tft->drawPixel(x,y+1,TFT_BLACK);
    }
  }
}

void QRcode::create(String message) {
  // create QR code
  message.toCharArray((char *)strinbuf,260);
  qrencode();
  // print QR Code
  for (byte x = 0; x < WD; x+=2) {
    for (byte y = 0; y < WD; y++) {
      if ( QRBIT(x,y) &&  QRBIT((x+1),y)) {
        // black square on top of black square
        render(x, y, 1);
        render((x+1), y, 1);
      }
      if (!QRBIT(x,y) &&  QRBIT((x+1),y)) {
        // white square on top of black square
        render(x, y, 0);
        render((x+1), y, 1);
      }
      if ( QRBIT(x,y) && !QRBIT((x+1),y)) {
        // black square on top of white square
        render(x, y, 1);
        render((x+1), y, 0);
      }
      if (!QRBIT(x,y) && !QRBIT((x+1),y)) {
        // white square on top of white square
        render(x, y, 0);
        render((x+1), y, 0);
      }
    }
  }
}
