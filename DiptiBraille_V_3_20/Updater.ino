void printProgressBar(int progress) {
  const int barWidth = 50;  // Width of the progress bar
  int numChars = static_cast<int>((static_cast<float>(progress) / 100.0) * barWidth);
  
  Serial.print("[");
  for (int i = 0; i < barWidth; ++i) {
    if (i < numChars) {
      Serial.print("=");
    } else {
      Serial.print(" ");
    }
  }
  Serial.print("] ");
  Serial.print(progress);
  Serial.println("%");
}
void performUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    size_t written = 0;
    size_t chunkSize = 1024; // Adjust this value as needed
    uint8_t buffer[chunkSize];

    uint32_t lastPrintTime = 0;

    while (written < updateSize) {
      size_t chunk = min(chunkSize, updateSize - written);
      size_t bytesRead = updateSource.readBytes(buffer, chunk);
      
      if (bytesRead > 0) {
        size_t bytesWritten = Update.write(buffer, bytesRead);
        if (bytesWritten > 0) {
          written += bytesWritten;

          // Print progress every second
          uint32_t now = millis();
          if (now - lastPrintTime > 1000 || written == updateSize) {
            int percentage = (written * 100) / updateSize;
            printProgressBar(percentage);
            lastPrintTime = now;
          }
        } else {
          Serial.println("Error occurred during update. Error #: " + String(Update.getError()));
          break;
        }
      } else {
        Serial.println("Error reading from update source.");
        break;
      }
    }

    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    }
  } else {
    Serial.println("Not enough space to begin OTA");
  }
}


// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::FS &fs,String loc) {
  File updateBin = fs.open(loc);
  if (updateBin) {
    if (updateBin.isDirectory()) {
      Serial.print(loc);
      Serial.println(" is not a file");
      updateBin.close();
      return;
    }

    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
      Serial.println("Try to start update");
      performUpdate(updateBin, updateSize);
    } else {
      Serial.println("Error, file is empty");
    }

    updateBin.close();

    // whe finished remove the binary from sd card to indicate end of the process
  } else {
    Serial.println("Could not load update.bin from sd root");
  }
}

void downloadUpdate(){
  downloader d;
  d.Url(update_url);
  d.FileName("update.bin");
  d.SaveTo("/C/firmware/current/");
  Serial.println("Download Started...");
  d.Download();
  Serial.println("Download Completed..");
}
