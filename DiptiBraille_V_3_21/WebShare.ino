
// Adds the filé systems
void addFileSystems(void) {
  
  /**/  //<-- Addd space there like this /** /
  if (FFat.begin(true)) {
    if (!filemgr.AddFS(FFat, "Flash/FFat", false)) {
      Serial.println(F("Adding FFAT failed."));
    }
  } else {
    Serial.println(F("FFat File System not inited."));
  }
  /**/

  // Initialize SD card and gather information
  sdMounted = SD.begin(CS);


  if (sdMounted) {
    sdTotalSize = SD.totalBytes();
    sdUsedSpace = SD.usedBytes();

    sdcard_type_t cardType = SD.cardType();
    switch(cardType) {
      case CARD_NONE:
        sdType = "None";
        break;
      case CARD_MMC:
        sdType = "MMC";
        break;
      case CARD_SD:
        sdType = "SDSC";
        break;
      case CARD_SDHC:
        sdType = "SDHC";
        break;
      case CARD_UNKNOWN:
      default:
        sdType = "Unknown";
        break;
    }
    if (!filemgr.AddFS(SD, "SD-Card", false)) {
      Serial.println(F("Adding SD failed."));
    }
  }
else {
    Serial.println(F("SD File System not inited."));
  }
  /**/
}

uint32_t checkFileFlags(fs::FS &fs, String filename, uint32_t flags) {
  // Checks if target file name is valid for action. This will simply allow everything by returning the queried flag
  if (flags & ESPFMfGK::flagIsValidAction) {
    return flags & (~ESPFMfGK::flagIsValidAction);
  }

  // Checks if target file name is valid for action.
  if (flags & ESPFMfGK::flagIsValidTargetFilename) {
    return flags & (~ESPFMfGK::flagIsValidTargetFilename);
  }

  // Default actions
  uint32_t defaultflags = ESPFMfGK::flagCanDelete | ESPFMfGK::flagCanRename | ESPFMfGK::flagCanGZip |  // ^t
                          ESPFMfGK::flagCanDownload | ESPFMfGK::flagCanUpload | ESPFMfGK::flagCanEdit | // ^t
                          ESPFMfGK::flagAllowPreview;

  return defaultflags;
}

void setupFilemanager(void) {
  // See above.
  filemgr.checkFileFlags = checkFileFlags;

  filemgr.WebPageTitle = "FileManager";
  filemgr.BackgroundColor = "white";
  filemgr.textareaCharset = "accept-charset=\"utf-8\"";

  if ((WiFi.status() == WL_CONNECTED) && (filemgr.begin())) {
    Serial.print(F("Open Filemanager with http://"));
    Serial.print(WiFi.localIP());
    Serial.print(F(":"));
    Serial.print(filemanagerport);
    Serial.print(F("/"));
    Serial.println();
  } else {
    Serial.print(F("Filemanager: did not start"));
  }
}

//