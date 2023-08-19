/*
 * Need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin
 * put png file in (<project directory>/data)
 * Select Tools > ESP32 Sketch Data Upload menu item. This should start
 uploading the files into ESP32 flash file system.
*/
// #include <M5StickCPlus.h>
#include <M5Stack.h>
#include <SPIFFS.h>

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    if (!SPIFFS.begin(true)) {
        M5.Lcd.println("SPIFFS Mount Failed");
        M5.Lcd.println("SPIFFS format start...");
        SPIFFS.format();
        M5.Lcd.println("SPIFFS format complete");
        return;
    } else {
      M5.Lcd.println("Hello");
    }

    M5.Lcd.drawPngFile(SPIFFS, "/test.png", 0, 0);
}

void loop() {
    M5.update();
    if (M5.BtnA.wasPressed()) {
        M5.Lcd.drawPngFile(SPIFFS, "/test.png", 0, 0);
    }

    if (M5.BtnB.wasPressed()) {
        M5.Lcd.drawPngFile(SPIFFS, "/test.png", 0, 0);
    }
}
