#include <M5StickCPlus.h>
#include <M5GFX.h>
#include <SPIFFS.h>

M5GFX display;
// String file_name = "/chichi_30x28.png";
String file_name = "/test.png";
bool SPIFFS_FORMAT = false;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(1);
  /*
  if (SPIFFS_FORMAT) {
    M5.Lcd.println("SPIFFS format start...");
    SPIFFS.format();
    M5.Lcd.println("SPIFFS format complete");
    File dataFile = SPIFFS.open(file_name, "w");
    // dataFile.println("Meh.");
    dataFile.close();
  }
  */

  if (SPIFFS.begin()) {  // Start SPIFFS, return 1 on success.
    M5.Lcd.println("SPIFFS Begin.");
    /*
    // Write operation
    File dataFile = SPIFFS.open(file_name, "w");  // Create a File object dafa  File to write information to
    // dataFile.println("Woop.");
    dataFile.close();
    */
  } else {
    M5.Lcd.println("SPIFFS Failed to Begin.\nFormatting...");
    M5.Lcd.println("SPIFFS format start...");
    SPIFFS.format();
    M5.Lcd.println("SPIFFS format complete");
    File dataFile = SPIFFS.open(file_name, "w");
    // dataFile.println("Coin.");
    dataFile.close();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();  // Check whether the key is pressed.  检测按键是否按下
  // display.startWrite();
  display.drawPngFile(SPIFFS, file_name.c_str(), 0, 0);
  display.drawPngFile(SPIFFS, file_name, 0, 0);
  // display.endWrite();
  if (M5.BtnA.isPressed()) {  // If the button is pressed.  如果按键按下
    if (SPIFFS.exists(file_name)) {  // Check whether the file_name file exists
      M5.Lcd.println("FOUND.");
      M5.Lcd.println(file_name);
    } else {
      M5.Lcd.println("NOT FOUND.");
      M5.Lcd.println(file_name);
    }
    /*
    File dataFile = SPIFFS.open(file_name, "r");  // Create aFile object dafaFile to read information to
    for (int i = 0; i < dataFile.size(); i++) {
      M5.Lcd.print((char)dataFile.read());
    }
    dataFile.close();  // Close the file after reading the file.
    */
  }
  delay(200);
}
