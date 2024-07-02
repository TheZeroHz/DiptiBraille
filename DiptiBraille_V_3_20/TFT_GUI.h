#ifndef TFT_GUI_H
#define TFT_GUI_H
#define boot0 "/boot0.jpg"
#define boot1 "/boot1.jpg"

#include <TFT_eSPI.h>
#include <SPI.h>
#include <JPEGDecoder.h>
#include<Arduino.h>

class TFT_GUI {
public:
    TFT_GUI(TFT_eSPI& tftInstance, int screenWidth, int screenHeight, int itemHeight, int animationDuration);
    void setItems(String items[], int itemCount);
    void setSelectedItem(int index);
    void drawMenu();
    void updateMenuTransition();
    void travarse(int pre,int select);
    void displayHardwareInfo(bool sdMounted, uint64_t sdTotalSize, uint64_t sdUsedSpace, const char* sdType);
    void drawSdJpeg(const char *filename, int xpos, int ypos);
    void bootUP();
    void setBright();

private:
    TFT_eSPI& tft;
    TFT_eSprite spr;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int ITEM_HEIGHT;
    int animationDuration;

    int selectedItem;
    int itemCount;
    String* menuItems;

    unsigned long animationStartTime;
    bool isAnimating;
    int fromItem;
    int toItem;

    void drawGradientBackground();
    void drawMenuItem(String item, int y, bool selected, int textSize, float fadeProgress);
    void startMenuTransition(int from, int to);
    void jpegRender(int xpos, int ypos);
    void jpegInfo();
    void showTime(uint32_t msTime);
};

#endif
