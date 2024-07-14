#include "TFT_GUI.h"
void TFT_GUI::setBright(int percentage){
  int val=map(percentage,0,100,0,255);
  analogWrite(TFT_BL,val);
}
void TFT_GUI::off(){
  digitalWrite(TFT_BL,LOW);
}
TFT_GUI::TFT_GUI(TFT_eSPI& tftInstance, int screenWidth, int screenHeight, int itemHeight, int animationDuration)
    : tft(tftInstance), SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight), ITEM_HEIGHT(itemHeight), animationDuration(animationDuration), spr(&tft) {
    spr.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    selectedItem = 0;
    itemCount = 0;
    menuItems = nullptr;
    isAnimating = false;
    pinMode(TFT_BL,OUTPUT);
}

void TFT_GUI::setItems(String items[], int itemCount) {
    this->itemCount = itemCount;
    menuItems = new String[itemCount];
    for (int i = 0; i < itemCount; i++) {
        menuItems[i] = items[i];
    }
    drawMenu();
}

void TFT_GUI::setSelectedItem(int index) {
    tft.setTextFont(4);
    selectedItem = index;
    drawMenu();
}

void TFT_GUI::drawGradientBackground() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        uint16_t color = tft.color565(0, 0, (255 * y) / SCREEN_HEIGHT);
        spr.drawFastHLine(0, y, SCREEN_WIDTH, color);
    }
}

void TFT_GUI::drawMenuItem(String item, int y, bool selected, int textSize, float fadeProgress) {
    spr.setTextSize(textSize);
    uint16_t textColor;
    if (selected) {
        uint8_t r = map(fadeProgress, 0, 1, 255, 0);
        textColor = tft.color565(r, r, r);
    } else {
        textColor = TFT_CYAN;
    }
    spr.setTextColor(textColor);
    spr.setTextDatum(MC_DATUM);
    spr.drawString(item, SCREEN_WIDTH / 2, y);
}

void TFT_GUI::drawMenu() {
    spr.fillSprite(TFT_BLACK);
    drawGradientBackground();

    for (int i = 0; i < itemCount; i++) {
        int y = SCREEN_HEIGHT / 2 + (i - selectedItem) * ITEM_HEIGHT;
        int textSize = (i == selectedItem) ? 2 : 1;
        drawMenuItem(menuItems[i], y, i == selectedItem, textSize, 0.0);
    }

    spr.pushSprite(0, 0);
}

void TFT_GUI::startMenuTransition(int from, int to) {
    fromItem = from;
    toItem = to;
    animationStartTime = millis();
    isAnimating = true;
}

void TFT_GUI::updateMenuTransition() {
    unsigned long currentTime = millis();
    if (currentTime - animationStartTime >= animationDuration) {
        isAnimating = false;
        selectedItem = toItem;
        drawMenu();
    } else {
        float progress = (float)(currentTime - animationStartTime) / animationDuration;
        spr.fillSprite(TFT_BLACK);
        drawGradientBackground();

        for (int i = 0; i < itemCount; i++) {
            int yFrom = SCREEN_HEIGHT / 2 + (i - fromItem) * ITEM_HEIGHT;
            int yTo = SCREEN_HEIGHT / 2 + (i - toItem) * ITEM_HEIGHT;
            int y = yFrom + (yTo - yFrom) * progress;
            int textSize = (i == toItem) ? 2 : 1;
            drawMenuItem(menuItems[i], y, i == toItem, textSize, progress);
        }

        spr.pushSprite(0, 0);
    }
}

void TFT_GUI::travarse(int pre,int select) {
    static unsigned long previousTime = millis();
    unsigned long currentTime = millis();
    if (!isAnimating && (currentTime - previousTime >= 1000)) {
        previousTime = currentTime;
        startMenuTransition(pre, select);
    }
    if (isAnimating) {
        updateMenuTransition();
    }
}

void TFT_GUI::displayHardwareInfo(bool sdMounted, uint64_t sdTotalSize, uint64_t sdUsedSpace, const char* sdType) {
  // Set the background color based on SD card status
  if (sdMounted) tft.fillScreen(TFT_BLUE);
  else tft.fillScreen(TFT_RED);
  // Center the title
  int centerX = tft.width() / 2;
  int cursorY = 20;
  tft.setCursor(40, cursorY+5);
  tft.setTextColor(TFT_BLUE,TFT_WHITE);
  tft.setTextSize(2);
  tft.println("[Boot Loader]");
  tft.setTextColor(TFT_WHITE);

  // Set smaller text size for the details
  tft.setTextSize(1);
  cursorY += 30;

  // Define column positions
  int col1X = 40;
  int col2X = 130; // Adjust this based on your screen width and desired spacing
  int rowSpacing = 15;
  int rowStartY = cursorY;
  int numRows = 9 + (sdMounted ? 4 : 1); // Number of rows including headers and SD card info if mounted
  // Print info in a two-column table format
  tft.setCursor(col1X, cursorY);
  tft.printf("OS Version:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%s","3.21");

   cursorY += rowSpacing;
  // Print info in a two-column table format
  tft.setCursor(col1X, cursorY);
  tft.printf("Release Type:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%s","Beta");


 cursorY += rowSpacing;
  // Print info in a two-column table format
  tft.setCursor(col1X, cursorY);
  tft.printf("SDK Version:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%s", ESP.getSdkVersion());
  
  cursorY += rowSpacing;
  // Print info in a two-column table format
  tft.setCursor(col1X, cursorY);
  tft.printf("Chip Model:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%s", ESP.getChipModel());

  cursorY += rowSpacing;
  tft.setCursor(col1X, cursorY);
  tft.printf("CPU Core:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%d", ESP.getChipCores());

  cursorY += rowSpacing;
  tft.setCursor(col1X, cursorY);
  tft.printf("CPU Freq:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%d MHz", ESP.getCpuFreqMHz());

  cursorY += rowSpacing;
  tft.setCursor(col1X, cursorY);
  tft.printf("Flash Size:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%d MB", ESP.getFlashChipSize() / (1024 * 1024));

  cursorY += rowSpacing;
  tft.setCursor(col1X, cursorY);
  tft.printf("Heap Size:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%d KB", ESP.getHeapSize() / 1024);

  cursorY += rowSpacing;
  tft.setCursor(col1X, cursorY);
  tft.printf("Free Heap:");
  tft.setCursor(col2X, cursorY);
  tft.printf("%d KB", ESP.getFreeHeap() / 1024);

  if (sdMounted) {
    cursorY += rowSpacing;
    tft.setCursor(col1X, cursorY);
    tft.printf("SD Card:");
    tft.setCursor(col2X, cursorY);
    tft.printf("Mounted");

    cursorY += rowSpacing;
    tft.setCursor(col1X, cursorY);
    tft.printf("SD Type:");
    tft.setCursor(col2X, cursorY);
    tft.printf("%s", sdType);

    cursorY += rowSpacing;
    tft.setCursor(col1X, cursorY);
    tft.printf("Total Size:");
    tft.setCursor(col2X, cursorY);
    tft.printf("%llu MB", sdTotalSize / (1024 * 1024));

    cursorY += rowSpacing;
    tft.setCursor(col1X, cursorY);
    tft.printf("Used Space:");
    tft.setCursor(col2X, cursorY);
    tft.printf("%llu MB", sdUsedSpace / (1024 * 1024));
  } else {
    cursorY += rowSpacing;
    tft.setCursor(col1X, cursorY);
    tft.printf("SD Card:");
    tft.setCursor(col2X, cursorY);
    tft.printf("Not mounted");
    
    cursorY += rowSpacing;
    tft.setTextColor(TFT_BLUE,TFT_WHITE);
    tft.setCursor(centerX - tft.textWidth("<..!--SYSTEM FAIL--!..>") / 2, cursorY);
    tft.println("<..!--SYSTEM FAIL--!..>");
  }

  // Draw table borders
  int tableWidth = tft.width() - 20;
  int tableHeight = rowSpacing * numRows;
  int startX = 5;
  int startY = rowStartY - 5;

  // Draw outer rectangle
  tft.drawRect(startX, startY, tableWidth, tableHeight, TFT_WHITE);

  // Draw horizontal lines
  for (int i = 1; i < numRows; ++i) {
    tft.drawLine(startX, startY + i * rowSpacing, startX + tableWidth, startY + i * rowSpacing, TFT_WHITE);
  }

  // Draw vertical line between columns
  tft.drawLine(col2X - 5, startY, col2X -5, startY + tableHeight, TFT_WHITE); // Adjust -10 based on your spacing
  if(!sdMounted){
    //while(1);
    tft.fillScreen(TFT_RED);
    tft.setCursor(40, 80);
    tft.setTextColor(TFT_BLUE,TFT_WHITE);
    tft.setTextSize(2);
    tft.println("SD FAIL :-(");
    }
}


void TFT_GUI::drawSdJpeg(const char *filename, int xpos, int ypos){
  // Open the named file (the Jpeg decoder library will close it)
  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");
  // Use one of the following methods to initialise the decoder:
  bool decoded = JpegDec.decodeSdFile(jpegFile);  // Pass the SD file handle to the decoder,
  //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();
    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}





void TFT_GUI::jpegRender(int xpos, int ypos){

  //jpegInfo(); // Print information from the JPEG file (could comment this line out)

  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  bool swapBytes = tft.getSwapBytes();
  tft.setSwapBytes(true);
  
  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
  uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // Fetch data from the file, decode and display
  while (JpegDec.read()) {    // While there is more data in the file
    pImg = JpegDec.pImage ;   // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

    // Calculate coordinates of top left corner of current MCU
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      uint16_t *cImg;
      int p = 0;
      cImg = pImg + win_w;
      for (int h = 1; h < win_h; h++)
      {
        p += mcu_w;
        for (int w = 0; w < win_w; w++)
        {
          *cImg = *(pImg + w + p);
          cImg++;
        }
      }
    }

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if (( mcu_x + win_w ) <= tft.width() && ( mcu_y + win_h ) <= tft.height())
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    else if ( (mcu_y + win_h) >= tft.height())
      JpegDec.abort(); // Image has run off bottom of screen so abort decoding
  }

  tft.setSwapBytes(swapBytes);

  showTime(millis() - drawTime); // These lines are for sketch testing only
}


void TFT_GUI::jpegInfo(){
    // Print information extracted from the JPEG file
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print("Width      :");
  Serial.println(JpegDec.width);
  Serial.print("Height     :");
  Serial.println(JpegDec.height);
  Serial.print("Components :");
  Serial.println(JpegDec.comps);
  Serial.print("MCU / row  :");
  Serial.println(JpegDec.MCUSPerRow);
  Serial.print("MCU / col  :");
  Serial.println(JpegDec.MCUSPerCol);
  Serial.print("Scan type  :");
  Serial.println(JpegDec.scanType);
  Serial.print("MCU width  :");
  Serial.println(JpegDec.MCUWidth);
  Serial.print("MCU height :");
  Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}

void TFT_GUI::showTime(uint32_t msTime){
  //tft.setCursor(0, 0);
  //tft.setTextFont(1);
  //tft.setTextSize(2);
  //tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.print(F(" JPEG drawn in "));
  //tft.print(msTime);
  //tft.println(F(" ms "));
  Serial.print(F(" JPEG drawn in "));
  Serial.print(msTime);
  Serial.println(F(" ms "));
}

void TFT_GUI::fadeUP(){
for(int i=0;i<=255;i++){analogWrite(TFT_BL,i);delay(4);}
}

void TFT_GUI::fadeDown(){
for(int i=255;i>=0;i--){analogWrite(TFT_BL,i);delay(4);}
}
void TFT_GUI::bootUP(){
  tft.fillScreen(TFT_BLACK);
  fadeUP();
  drawSdJpeg("/boot0.jpg", 0, 0);     // This draws a jpeg pulled off the SD Card
  delay(4000);
  fadeDown();
  drawSdJpeg("/boot1.jpg", 0, 0);
  fadeUP();
  delay(4000);
  fadeDown();
}