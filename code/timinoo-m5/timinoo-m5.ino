#include <M5StickCPlus.h>
#include <M5GFX.h>
#include <SPIFFS.h>
#include "timinoo_m5_images.h"

/*
 * Screen resolution: 240x135
 * 
 * Size 2 font
 *  -> Max cols: 20
 *  -> Max lines: 8
 * 
 * Size 1 font
 *  -> Max cols: ?
 *  -> Max lines: ?
 *  
 */

// https://github.com/m5stack/M5StickC-Plus
// https://github.com/m5stack/M5GFX
// https://github.com/m5stack/M5GFX/blob/master/examples/Basic/drawImageData/drawImageData.ino
// https://github.com/m5stack/M5GFX/blob/master/examples/Basic/GameOfLife/GameOfLife.ino

M5GFX display;
M5Canvas canvas[1];
extern const uint8_t jpg[];
int gameMode = 99;
long score = 0;
char scoreString[9];
const int buttonPin = 2;
int buttonState = 0;
unsigned long frameCounter = 0;
int animationStep = 1;
int animationStepMax = 4;
const byte ButtonPin = 2;
const unsigned long DebounceTime = 10;
boolean ButtonWasPressed = false;
unsigned long ButtonStateChangeTime = 0; // Debounce timer
int currentIcon = 0;
int generalCounter = 0;
int currentSequence = 0;
long gamePick = 0;
int strawberryFoodStock = 0;
int appleFoodStock = 2;
int grapeFoodStock = 0;
int milkFoodStock = 0;
int orangeFoodStock = 1;
int kokoXPos = 0;
long randomQuote = 0;
int selectedFood = 0;
int superHappyCounter = 0;
long randomVisit = 0;
long randomGameIconXPos = 0;
long randomFoodType = 0;
int gameIconXPos = 0;
int kisscussCounter = 0;
long randomNumber = 0;

// Cat status variables
// Status metrics
// 0 = depleted, 1 = low, 2 = average, 3 = full
long catHunger = random(1, 4);
long catHygiene = random(1, 4);
long catMorale = random(1, 4);
long catEducation = random(1, 3);
long catEntertainment = random(1, 4);

// Status change timing (decrement status variable every x frames)
// Production timings
unsigned long catHungerStep = random(800, 3000);
// unsigned long catHungerStep = 10;
unsigned long catHygieneStep = random(1000, 4000);
// unsigned long catHygieneStep = 15;
unsigned long catMoraleStep = random(500, 2000);
// unsigned long catMoraleStep = 20;
unsigned long catEducationStep = random(200, 1200);
// unsigned long catEducationStep = 3;
unsigned long catEntertainmentStep = random(240, 1500);
// unsigned long catEntertainmentStep = 1;

// Tracking status checks
unsigned long lastCatHungerCheck = 0;
unsigned long lastCatHygieneCheck = 0;
unsigned long lastCatMoraleCheck = 0;
unsigned long lastCatEducationCheck = 0;
unsigned long lastCatEntertainmentCheck = 0;


void smoltxt(int posX, int posY, String smallText) {
  // Display small text (size 1)
  /*
  canvas[0].setTextColor(1);
  canvas[0].setTextSize(1.0);
  canvas[0].setTextDatum(textdatum_t::bottom_center);
  canvas[0].drawString(smallText, canvas[0].width() >> 1, canvas[0].height() >> 1);
  canvas[0].pushRotateZoom(&display, 0,  (float)display.width() / canvas[0].width(), (float)display.height() / canvas[0].height());
  */
  display.setTextSize(1);
  display.setCursor(posX, posY);
  display.print(smallText);
}

void normtxt(int posX, int posY, String normText) {
  // Display normal text (size 2)
  display.setTextSize(2);
  display.setCursor(posX, posY);
  display.print(normText);
}

void setup() {
  M5.begin();
  display.begin();
  display.setColorDepth(8);
  display.setEpdMode(epd_mode_t::epd_fastest);
  if (display.width() < display.height())
  {
    display.setRotation(display.getRotation() ^ 1);
    display.setPivot(display.width() /2 -0.5, display.height() /2 - 0.5);
  }
  /*
  canvas[0].setColorDepth(8);
  canvas[0].createSprite(width, height);
  canvas[0].createPalette();
  canvas[0].setPaletteColor(1, TFT_WHITE);
  canvas[0].setPivot(canvas[i].width() /2 -0.5, canvas[i].height() /2 - 0.5);
  */
}

void checkButton()
{
  if ( M5.BtnA.wasPressed() ) {
    if (gameMode == 0) {
      gameMode = 9;
      currentSequence = 0;
      generalCounter = 0;
    } else if (gameMode == 1) {
      // Do something only if cat needs something
      switch (currentIcon) {
        case 2:
          // Play
          generalCounter = 0;
          currentSequence = 0;
          gameMode = 6;
          break;
        case 3:
          // Study
          randomQuote = random(1, 7);
          kokoXPos = 128;
          generalCounter = 0;
          currentSequence = 0;
          gameMode = 4;
          break;
        case 4:
          // Cuddle
          generalCounter = 0;
          gameMode = 3;
          break;
        case 5:
          // Clean
          currentSequence = 0;
          generalCounter = 0;
          gameMode = 5;
          break;
        case 6:
          // Feed
          selectedFood = 0;
          currentSequence = 0;
          generalCounter = 0;
          gameMode = 2;
          break;
      }
    } else if (gameMode == 5) {
      // Clean The Cat game
      generalCounter += 15;
    } else if (gameMode == 6) {
      // Game
      if (currentSequence == 0) {
        gamePick = random (0, 8);
        switch (gamePick) {
          case 0:
            if (score>666 && frameCounter % 2 == 0) {
              score -= 666;
            } else {
              gamePick = 6;
              orangeFoodStock += 1;
              score += 200;
            }
            break;
          case 1:
            strawberryFoodStock += 1;
            appleFoodStock += 1;
            grapeFoodStock += 1;
            milkFoodStock += 1;
            orangeFoodStock += 1;
            score += 500;
            break;
          case 2:
            strawberryFoodStock += 1;
            score += 300;
            break;
          case 3:
            appleFoodStock += 1;
            score += 200;
            break;
          case 4:
            grapeFoodStock += 1;
            score += 200;
            break;
          case 5:
            milkFoodStock += 1;
            score += 100;
            break;
          case 6:
            orangeFoodStock += 1;
            score += 200;
            break;
        }
        currentSequence = 1;
        generalCounter = 0;
        randomGameIconXPos = random(0, 3);
        randomFoodType = random(0, 7);
      }
    } else if (gameMode == 7) {
      if (currentSequence == 0) {
        currentSequence = 1;
        generalCounter = 0;
      }
    } else if (gameMode == 9 && currentSequence == 0) {
      generalCounter = 0;
      if ( M5.BtnA.wasPressed() ) {
        // Button A: Cuss
        if (randomFoodType<3) {
          // Unhappy
          currentSequence=1;
        } else {
          // Happy
          currentSequence=3;
        }
      } else if ( M5.BtnB.wasPressed() ) {
        // Button B: Kiss
        if (randomFoodType<3) {
          // Happy
          currentSequence=3;
        } else {
          // Unhappy
          currentSequence=1;
        }
      }
    } else if (gameMode == 10) {
      // Select menu item
      generalCounter = 0;
      switch (currentSequence) {
        case 0:
          // Feed
          selectedFood = 0;
          currentSequence = 0;
          generalCounter = 0;
          gameMode = 2;
          break;
        case 1:
          // Play
          generalCounter = 0;
          currentSequence = 0;
          gameMode = 6;
          break;
        case 2:
          // Clean
          currentSequence = 0;
          generalCounter = 0;
          gameMode = 5;
          break;
        case 3:
          // Cuddle
          generalCounter = 0;
          gameMode = 3;
          break;
        case 4:
          // Study
          randomQuote = random(1, 7);
          kokoXPos = 128;
          generalCounter = 0;
          currentSequence = 0;
          gameMode = 4;
          break;
      }
    } else if (gameMode == 11) {
      gameMode = 0;
      currentSequence = 0;
      generalCounter = 0;
    }
  } else if ( M5.BtnB.wasPressed() ) {
    if (gameMode == 0) {
      gameMode = 10;
      currentSequence = 0;
      generalCounter = 0;
    } else if (gameMode == 10) {
      generalCounter = 0;
      if ( M5.BtnB.wasPressed() ) {
        currentSequence -= 1;
        if (currentSequence < 0) {
          currentSequence = 4;
        }
      }
    }
  } else if ( M5.BtnB.wasPressed() ) {
    if (gameMode == 0) {
      gameMode = 11;
      currentSequence = 0;
      generalCounter = 0;
    } else if (gameMode == 11) {
      currentSequence += 1;
      if (currentSequence > 4) {
        currentSequence = 0;
      }
      generalCounter = 0;
    }
  }
}

void loop() {
  checkButton();
  frameCounter += 1;
  if (frameCounter > 4294967290) {
    frameCounter = 0;
    lastCatHungerCheck = 0;
    lastCatHygieneCheck = 0;
    lastCatMoraleCheck = 0;
    lastCatEducationCheck = 0;
    lastCatEntertainmentCheck = 0;
  }
  animationStep += 1;
  if (animationStep>animationStepMax) {
    animationStep=1;
  }

  // Refresh cat statistics
  // Hunger
  if (frameCounter == lastCatHungerCheck + catHungerStep) {
    catHunger -= 1;
    if (catHunger < 0) {
      catHunger = 0;
    }
    lastCatHungerCheck = frameCounter;
  }
  // Hygiene
  if (frameCounter == lastCatHygieneCheck + catHygieneStep) {
    catHygiene -= 1;
    if (catHygiene < 0) {
      catHygiene = 0;
    }
    lastCatHygieneCheck = frameCounter;
  }
  // Morale
  if (frameCounter == lastCatMoraleCheck + catMoraleStep) {
    catMorale -= 1;
    if (catMorale < 0) {
      catMorale = 0;
    }
    lastCatMoraleCheck = frameCounter;
  }
  // Education
  if (frameCounter == lastCatEducationCheck + catEducationStep) {
    if (catEducation < 3 && gameMode == 0) {
      // Time to study
      currentIcon = 3;
      gameMode = 1;
    }
    lastCatEducationCheck = frameCounter;
  }
  // Entertainment
  if (frameCounter == lastCatEntertainmentCheck + catEntertainmentStep) {
    catEntertainment -= 1;
    if (catEntertainment < 0) {
      catEntertainment = 0;
    }
    lastCatEntertainmentCheck = frameCounter;
  }

  // Act on cat stats
  if (catHunger == 0 && gameMode == 0) {
    // Time to feed the cat
    currentIcon = 6;
    randomVisit = random(0, 3136);
    if ((randomVisit<1000 && randomVisit % 2 == 0) || (randomVisit > 3000 && randomVisit % 2 == 0)) {
      currentSequence = 0;
      generalCounter = 0;
      gameMode = 7;
    } else {
      gameMode = 1;
    }
  } else if (catHygiene==0 && gameMode == 0) {
    // Time to clean
    currentIcon = 5;
    gameMode = 1;
  } else if (catMorale==0 && gameMode == 0) {
    // Time to cuddle
    currentIcon = 4;
    gameMode = 1;
  } else if (catEntertainment==0 && gameMode == 0) {
    // Time to play
    currentIcon = 2;
    gameMode = 1;
  }

  display.startWrite();
  display.clear();
  switch (gameMode) {
    case 0:
      // Idling
      animationStepMax = 8;
      checkButton();
      // Icon frame
      display.drawPng(speech_bubble_56x48, ~0u, 69, 1);
      // Icon (always happy)
      if (superHappyCounter<1) {
        display.drawPng(happy_cat_28x28, ~0u, 87, 12);
      } else {
        display.drawPng(super_happy_28x28, ~0u, 87, 12);
        superHappyCounter -= 1;
      }
      switch (animationStep) {
        case 1 ... 2:
          checkButton();
          display.drawPng(cat_sitting_001_48x48, ~0u, 8, 8);
          break;
        case 3 ... 4:
          checkButton();
          display.drawPng(cat_sitting_002_48x48, ~0u, 8, 8);
          break;
        case 5 ... 6:
          checkButton();
          display.drawPng(cat_sitting_003_48x48, ~0u, 8, 8);
          break;
        case 7 ... 8:
          checkButton();
          display.drawPng(cat_sitting_004_48x48, ~0u, 8, 8);
          break;
      }
      break;
    case 1:
      // Idle - looking left
      animationStepMax = 8;
      checkButton();
      // Icon frame
      display.drawPng(speech_bubble_56x48, ~0u, 69, 1);
      // Icon (anything else than happy)
      switch (currentIcon) {
        case 2:
          // Play
          display.drawPng(play_32x20, ~0u, 85, 16);
          break;
        case 3:
          // Study
          display.drawPng(study_26x28, ~0u, 88, 12);
          break;
        case 4:
          // Cuddle
          display.drawPng(cuddle_24x24, ~0u, 89, 14);
          break;
        case 5:
          // Bubbles
          display.drawPng(bubbles_30x30, ~0u, 86, 11);
          break;
        case 6:
          // Pizza
          display.drawPng(pizza_26x28, ~0u, 88, 12);
          break;
      }
      switch (animationStep) {
        case 1 ... 2:
          checkButton();
          display.drawPng(cat_sitting_001_48x48, ~0u, 8, 8);
          break;
        case 3 ... 4:
          checkButton();
          display.drawPng(cat_sitting_006_48x48, ~0u, 8, 8);
          break;
        case 5 ... 6:
          checkButton();
          display.drawPng(cat_sitting_007_48x48, ~0u, 8, 8);
          break;
        case 7 ... 8:
          checkButton();
          display.drawPng(cat_sitting_004_48x48, ~0u, 8, 8);
          break;
      }
      break;
    case 2:
      // Feed
      switch (currentSequence) {
        case 0:
          // Select food
          if (strawberryFoodStock > 0) {
            selectedFood = 1;
            strawberryFoodStock -= 1;
            score += 30;
          } else if (grapeFoodStock > 0) {
            selectedFood = 2;
            grapeFoodStock -= 1;
            score += 20;
          } else if (milkFoodStock > 0) {
            selectedFood = 3;
            milkFoodStock -= 1;
            score += 10;
          } else if (orangeFoodStock > 0) {
            selectedFood = 4;
            orangeFoodStock -= 1;
            score += 20;
          } else if (appleFoodStock > 0) {
            selectedFood = 5;
            appleFoodStock -= 1;
            score += 20;
          } else {
            selectedFood = 6;
          }
          currentSequence = 1;
          break;
        case 1:
          // Eat food
          display.setCursor(0, 10);
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          switch (selectedFood) {
            case 1:
              display.drawPng(strawberry_28x28, ~0u, 50, 28);
              display.print("  Yummy  strawberry  ");
              break;
            case 2:
              display.drawPng(grape_28x28, ~0u, 50, 28);
              display.print("    Fresh  grapes    ");
              break;
            case 3:
              display.drawPng(milk_28x28, ~0u, 50, 28);
              display.print("     Farm  milk      ");
              break;
            case 4:
              display.drawPng(orange_28x28, ~0u, 50, 28);
              display.print("    Juicy  orange    ");
              break;
            case 5:
              display.drawPng(apple_28x28, ~0u, 50, 28);
              display.print("    Tasty  apple     ");
              break;
            case 6:
              display.drawPng(ghost_28x28, ~0u, 50, 28);
              display.print("      No  food       ");
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            if (selectedFood != 6) {
              currentSequence = 2;
            } else {
              generalCounter = 0;
              currentSequence = 0;
              gameMode = 6;
            }
          }
          break;
        case 2:
          // Yum
          if (selectedFood != 6) {
            display.setCursor(0, 55);
            display.print("        Yum !        ");
          }
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          switch (selectedFood) {
            case 1:
              display.drawPng(strawberry_28x28, ~0u, 50, 14);
              break;
            case 2:
              display.drawPng(grape_28x28, ~0u, 50, 14);
              break;
            case 3:
              display.drawPng(milk_28x28, ~0u, 50, 14);
              break;
            case 4:
              display.drawPng(orange_28x28, ~0u, 50, 14);
              break;
            case 5:
              display.drawPng(apple_28x28, ~0u, 50, 14);
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            if (selectedFood < 6) {
              superHappyCounter = 100;
              catHunger = 3;
            }
            gameMode = 0;
          }
          break;
      }
      break;
    case 3:
      // Cuddle
      display.drawPng(cat_sitting_001_48x48, ~0u, 8, 8);
      generalCounter += 2;
      if (generalCounter<31) {
        display.drawPng(cuddle_heart_11x10, ~0u, 80, 45 - generalCounter);
        display.drawPng(cuddle_heart_11x10, ~0u, 92, 40 - generalCounter);
        display.drawPng(cuddle_heart_11x10, ~0u, 104, 45 - generalCounter);
      } else if (generalCounter>30 && generalCounter < 85) {
        display.drawPng(cuddle_heart_11x10, ~0u, 80, 15);
        display.drawPng(cuddle_heart_11x10, ~0u, 92, 10);
        display.drawPng(cuddle_heart_11x10, ~0u, 104, 15);
        smoltxt(70, 40, "I love you");
      } else if (generalCounter>84 && generalCounter < 108) {
        smoltxt(70, 40, "I love you");
      } else if (generalCounter>=109) {
        superHappyCounter = 100;
        score += 50;
        catMorale = 3;
        gameMode = 0;
      }
      break;
    case 4:
      // Educate
      switch (currentSequence) {
        case 0:
          // Snail arrives
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          display.drawPng(koko_le_snail_26x22, ~0u, kokoXPos, 40);
          kokoXPos -= 1;
          if (kokoXPos < 97) {
            kokoXPos = 97;
          }
          generalCounter += 1;
          if (generalCounter>29) {
            generalCounter = 0;
            currentSequence = 1;
          }
          break;
        case 1:
          // Snail says hello
          normtxt(0, 55, "           Hi! >     ");
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          display.drawPng(koko_le_snail_26x22, ~0u, 97, 40);
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            currentSequence = 2;
          }
          break;
        case 2:
          // Introduction
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 56, 239, 56, WHITE);
          display.drawPng(koko_le_snail_26x22, ~0u, 97, 21);
          smoltxt(6, 18, "Get ready for a\nnew lesson with...\n\n~ Koko Le Snail ~");
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            currentSequence = 3;
          }
          break;
        case 3:
          // Snail wisdom quote
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 56, 239, 56, WHITE);
          display.drawPng(koko_le_snail_26x22, ~0u, 97, 21);
          switch (randomQuote) {
            case 1:
              smoltxt(6, 18, "Sometimes dogs\nare grey.\n\n  -- Koko");
              break;
            case 2:
              smoltxt(6, 18, "Do not sneeze\non the bus.\n\n  -- Koko");
              break;
            case 3:
              smoltxt(6, 18, "Always wear\npants.\n\n  -- Koko");
              break;
            case 4:
              smoltxt(6, 18, "Never yawn\nduring class.\n\n  -- Koko");
              break;
            case 5:
              smoltxt(6, 18, "Wash your hands\nafter lunch.\n\n  -- Koko");
              break;
            case 6:
              smoltxt(6, 18, "Pull my finger...\nteehee!\n\n  -- Koko");
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            currentSequence = 4;
          }
          break;
        case 4:
          // Score
          display.drawPng(study_26x28, ~0u, 51, 28);
          normtxt(0, 10, "    + 1  Education   ");
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            superHappyCounter = 100;
            score += 100;
            catEducation += 1;
            gameMode = 0;
          }
          break;
      }
      break;
    case 5:
      // Clean
      switch (currentSequence) {
        case 0:
          // Clean The Cat game
          display.drawPng(cat_sitting_001_48x48, ~0u, 38, 8);
          checkButton();
          randomNumber = random(-2, 3);
          generalCounter -= 2;
          if (generalCounter<0) {
            generalCounter = 0;
          } else if (generalCounter>127) {
            generalCounter = 0;
            currentSequence = 1;
          }
          switch(generalCounter) {
            case 5 ... 15:
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              break;
            case 16 ... 36:
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 58 + randomNumber, 32);
              break;
            case 37 ... 77:
              display.drawPng(bubbles_30x30, ~0u, 23 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 58 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 73 + randomNumber, 16);
              break;
            case 78 ... 98:
              display.drawPng(bubbles_30x30, ~0u, 23 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 58 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 73 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 32 + randomNumber, 0);
              display.drawPng(bubbles_30x30, ~0u, 64 + randomNumber, 0);
              break;
            case 99 ... 110:
              display.drawPng(bubbles_30x30, ~0u, 23 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 58 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 73 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 0 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 32 + randomNumber, 0);
              display.drawPng(bubbles_30x30, ~0u, 64 + randomNumber, 0);
              display.drawPng(bubbles_30x30, ~0u, 96 + randomNumber, 32);
              break;
            case 111 ... 150:
              display.drawPng(bubbles_30x30, ~0u, 23 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 38 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 58 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 73 + randomNumber, 16);
              display.drawPng(bubbles_30x30, ~0u, 0 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 32 + randomNumber, 0);
              display.drawPng(bubbles_30x30, ~0u, 64 + randomNumber, 0);
              display.drawPng(bubbles_30x30, ~0u, 96 + randomNumber, 32);
              display.drawPng(bubbles_30x30, ~0u, 16 + randomNumber, 48);
              display.drawPng(bubbles_30x30, ~0u, 48 + randomNumber, 48);
          }
          // M5.Lcd.drawLine(0, 63, generalCounter, 63, WHITE);
          break;
        case 1:
          normtxt(0, 10, "      All clean yay !");
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          display.drawPng(cuddle_heart_11x10, ~0u, 69, 50);
          display.drawPng(cuddle_heart_11x10, ~0u, 81, 50);
          display.drawPng(cuddle_heart_11x10, ~0u, 93, 50);
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            superHappyCounter = 100;
            score += 200;
            catHygiene = 3;
            gameMode = 0;
          }
          break;
      }
      break;
    case 6:
      // Play: Catsino
      if (currentSequence == 0) {
        // Roll the dice
        checkButton();
        animationStepMax = 7;
        smoltxt(5, 6, "xxxx Catsino Deluxe xxxx");
        display.drawPng(casino_frame_40x40, ~0u, 3, 18);
        display.drawPng(casino_frame_40x40, ~0u, 44, 18);
        display.drawPng(casino_frame_40x40, ~0u, 85, 18);
        checkButton();
        for (gameIconXPos = 9; gameIconXPos < 92; gameIconXPos += 41) {
          randomFoodType = random(0, 8);
          switch (randomFoodType) {
            case 0:
              display.drawPng(ghost_28x28, ~0u, gameIconXPos, 24);
              break;
            case 1:
              display.drawPng(bar_28x28, ~0u, gameIconXPos, 24);
              break;
            case 2:
              display.drawPng(strawberry_28x28, ~0u, gameIconXPos, 24);
              break;
            case 3:
              display.drawPng(apple_28x28, ~0u, gameIconXPos, 24);
              break;
            case 4:
              display.drawPng(grape_28x28, ~0u, gameIconXPos, 24);
              break;
            case 5:
              display.drawPng(milk_28x28, ~0u, gameIconXPos, 24);
              break;
            case 6:
              display.drawPng(orange_28x28, ~0u, gameIconXPos, 24);
              break;
            case 7:
              display.drawPng(dollar_28x28, ~0u, gameIconXPos, 24);
              break;
          }
        }
        generalCounter += 1;
        if (generalCounter>800) {
          generalCounter = 0;
          gameMode = 0;
        }
      } else if (currentSequence == 1) {
        // See the result
        // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        switch (gamePick) {
          case 0:
            // Ghost
            display.drawPng(ghost_28x28, ~0u, 50, 28);
            normtxt(0, 10, "   Nothing, boo !    ");
            break;
          case 1:
            // Bar
            display.drawPng(bar_28x28, ~0u, 50, 28);
            normtxt(0, 10, "     + 1 of all!     ");
            break;
          case 2:
            // Strawberry
            display.drawPng(strawberry_28x28, ~0u, 50, 28);
            normtxt(0, 10, "   + 1  strawberry   ");
            break;
          case 3:
            // Apple
            display.drawPng(apple_28x28, ~0u, 50, 28);
            normtxt(0, 10, "     + 1  apple      ");
            break;
          case 4:
            // Grape
            display.drawPng(grape_28x28, ~0u, 50, 28);
            normtxt(0, 10, "     + 1  grape      ");
            break;
          case 5:
            // Milk
            display.drawPng(milk_28x28, ~0u, 50, 28);
            normtxt(0, 10, "      + 1  milk      ");
            break;
          case 6:
            // Orange
            display.drawPng(orange_28x28, ~0u, 50, 28);
            normtxt(0, 10, "     + 1  orange     ");
            break;
          case 7:
            // Replay + Double
            display.drawPng(dollar_28x28, ~0u, 50, 28);
            normtxt(0, 10, "Double score + Replay");
            break;
        }
        generalCounter += 1;
        if (generalCounter>24) {
          generalCounter = 0;
          if (gamePick > 0) {
            if (gamePick==7) {
              generalCounter = 0;
              currentSequence = 0;
              score = score * 2;
              gameMode = 6;
            } else {
              superHappyCounter = 100;
              catEntertainment = 3;
              gameMode = 0;
            }
          } else {
            // Got the ghost
            generalCounter = 0;
            currentSequence = 0;
            gameMode = 6;
          }
        }
      }
      break;
    case 7:
      // Random visitor
      switch (currentSequence) {
        case 0:
          // Knock
          checkButton();
          display.drawPng(door_28x30, ~0u, 50, 14);
          smoltxt(35, 59, "Knock knock!");
          generalCounter += 1;
          if (generalCounter>4800) {
            currentSequence = 1;
            generalCounter = 0;
          }
          break;
        case 1:
          // Hello
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          if (randomVisit<1000) {
            display.drawPng(sophie_28x26, ~0u, 96, 14);
          } else if (randomVisit > 3000) {
            display.drawPng(chichi_30x28, ~0u, 96, 14);
          }
          smoltxt(45, 59, "Hi friend!");
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 2;
            generalCounter = 0;
          }
          break;
        case 2:
          // Gift
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          switch (randomVisit) {
            case 0 ... 499:
              smoltxt(45, 59, "I got matcha tea!");
              display.drawPng(sophie_28x26, ~0u, 96, 14);
              break;
            case 500 ... 999:
              smoltxt(45, 59, "I got coco cake!");
              display.drawPng(sophie_28x26, ~0u, 96, 14);
              break;
            case 3001 ... 3100:
              // Strawberry
              smoltxt(45, 59, "I am cooking");
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              break;
            case 3101 ... 3136:
              // Cuddle
              smoltxt(45, 59, "I am sad");
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 3;
            generalCounter = 0;
          }
          break;
        case 3:
          // Drink
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          switch (randomVisit) {
            case 0 ... 999:
              smoltxt(45, 59, "Have some <3");
              display.drawPng(sophie_28x26, ~0u, 96, 14);
              if (randomVisit<500) {
                display.drawPng(matcha_30x32, ~0u, 49, 14);
              } else if (randomVisit<1000){
                display.drawPng(coco_cake_28x32, ~0u, 49, 14);
              }
              break;
            case 3001 ... 3050:
              // Strawberry
              smoltxt(45, 59, "I need strawberries");
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              break;
            case 3051 ... 3100:
              // Orange
              smoltxt(45, 59, "I need oranges");
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              break;
            case 3101 ... 3136:
              // Cuddle
              smoltxt(45, 59, "I need to cuddle");
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            switch (randomVisit) {
              case 0 ... 999:
                currentSequence = 4;
                break;
              case 3001 ... 3050:
                // Strawberry
                if (strawberryFoodStock>0) {
                  strawberryFoodStock -= 1;
                  currentSequence = 4;
                } else {
                  currentSequence = 7;
                }
                break;
              case 3051 ... 3100:
                // Orange
                if (orangeFoodStock>0) {
                  orangeFoodStock -= 1;
                  currentSequence = 4;
                } else {
                  currentSequence = 7;
                }
                break;
              case 3101 ... 3136:
                // Cuddle
                currentSequence = 4;
                break;
            }
          }
          break;
        case 4:
          // Bonus
          display.setCursor(0, 10);
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          switch (randomVisit) {
            case 0 ... 999:
              if (randomVisit<500) {
                display.drawPng(matcha_30x32, ~0u, 49, 28);
              } else {
                display.drawPng(coco_cake_28x32, ~0u, 50, 28);
              }
              display.print("        Yum !        ");
              break;
            case 3001 ... 3050:
              // Strawberry
              smoltxt(45, 59, "Thank you <3");
              display.print("   - 1 Strawberry    ");
              display.drawPng(strawberry_28x28, ~0u, 49, 28);
              break;
            case 3051 ... 3100:
              // Orange
              smoltxt(45, 59, "Thanks a lot!");
              display.print("     - 1 Orange      ");
              display.drawPng(orange_28x28, ~0u, 49, 28);
              break;
            case 3101 ... 3136:
              // Cuddle
              display.print("  You hug ChiChi  ");
              display.drawPng(cat_sitting_001_48x48, ~0u, 40, 28);
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 5;
            generalCounter = 0;
          }
          break;
        case 5:
          // Everyone is happy
          display.drawPng(cat_sitting_001_48x48, ~0u, -24, 13);
          switch (randomVisit) {
            case 0 ... 999:
              display.drawPng(sophie_28x26, ~0u, 96, 14);
              smoltxt(45, 59, "That was yummy!");
              break;
            case 3001 ... 3136:
              display.drawPng(chichi_30x28, ~0u, 96, 14);
              smoltxt(45, 59, "Aww thanks <3");
              break;
          }
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 6;
            generalCounter = 0;
          }
          break;
        case 6:
          display.setCursor(0, 10);
          switch (randomVisit) {
            case 0 ... 999:
              display.print("    + 10000 points   ");
              break;
            case 3001 ... 3136:
              display.print("    + 5000 points    ");
              break;
          }
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 0;
            generalCounter = 0;
            superHappyCounter = 100;
            switch (randomVisit) {
              case 0 ... 999:
                score += 10000;
                catHunger = 3;
                break;
              case 3001 ... 3136:
                score += 5000;
                break;
            }
            gameMode = 0;
          }
          break;
        case 7:
          // ChiChi is sad
          display.setCursor(0, 10);
          // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          display.drawPng(chichi_30x28, ~0u, 96, 28);
          smoltxt(40, 59, "Maybe next time...");
          display.print("   You have none...  ");
          generalCounter += 1;
          if (generalCounter>24) {
            currentSequence = 0;
            generalCounter = 0;
            gameMode = 0;
          }
          break;
      }
      break;
    case 9:
      // Game: Kiss Cuss
      if (currentSequence == 0) {
        // Display random character (Koko, Sophie, ChiChi or Ghost)
        checkButton();
        display.setCursor(0, 10);
        display.print("[B]  Kiss ~ Cuss  [A]");
        // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        checkButton();
        if ( frameCounter % 6 == 0 ) {
          randomFoodType = random(0, 4);
          randomGameIconXPos = random(20, 80);
        }
        switch (randomFoodType) {
          case 0:
            display.drawPng(koko_le_snail_26x22, ~0u, randomGameIconXPos, 28);
            break;
          case 1:
            display.drawPng(sophie_28x26, ~0u, randomGameIconXPos, 28);
            break;
          case 2:
            display.drawPng(chichi_30x28, ~0u, randomGameIconXPos, 28);
            break;
          case 3:
            display.drawPng(ghost_28x28, ~0u, randomGameIconXPos, 28);
            break;
        }
        generalCounter += 1;
        if (generalCounter>800) {
          generalCounter = 0;
          gameMode = 0;
        }
      } else if (currentSequence == 1) {
        // Unhappy 1
        switch (randomFoodType) {
          case 0 ... 2:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(skull_11x11, ~0u, backgroundx, backgroundy);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(skull_11x11, ~0u, backgroundx, backgroundy);
                }
              }
              delay(5);
            }
            break;
          case 3:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(heart_11x10, ~0u, backgroundx, backgroundy);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(heart_11x10, ~0u, backgroundx, backgroundy);
                }
              }
              delay(5);
            }
            break;
        }
        generalCounter += 1;
        if (generalCounter>2) {
          generalCounter = 0;
          currentSequence = 2;
        }
      } else if (currentSequence == 2) {
        // Unhappy 2
        display.drawPng(cuss_28x28, ~0u, 51, 28);
        display.setCursor(0, 10);
        display.print("      No points      ");
        // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        generalCounter += 1;
        if (generalCounter>16) {
          generalCounter = 0;
          currentSequence = 0;
          kisscussCounter += 1;
          if (kisscussCounter > 10) {
            currentSequence = 5;
            generalCounter = 0;
          }
          randomFoodType = random(0, 4);
        }
      } else if (currentSequence == 3) {
        // Happy 1
        switch (randomFoodType) {
          case 0 ... 2:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(heart_11x10, ~0u, backgroundx, backgroundy);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(heart_11x10, ~0u, backgroundx, backgroundy);
                }
              }
              delay(5);
            }
            break;
          case 3:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(skull_11x11, ~0u, backgroundx, backgroundy);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  display.drawPng(skull_11x11, ~0u, backgroundx, backgroundy);
                }
              }
              delay(5);
            }
            break;
        }
        generalCounter += 1;
        if (generalCounter>2) {
          generalCounter = 0;
          currentSequence = 4;
        }
      } else if (currentSequence == 4) {
        // Happy 2
        display.drawPng(kiss_28x28, ~0u, 51, 28);
        display.setCursor(0, 10);
        display.print("    + 1000 points    ");
        // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        generalCounter += 1;
        if (generalCounter>16) {
          score += 1000;
          generalCounter = 0;
          currentSequence = 0;
          kisscussCounter += 1;
          if (kisscussCounter > 10) {
            currentSequence = 5;
            generalCounter = 0;
          }
          randomFoodType = random(0, 4);
        }
      } else if (currentSequence == 5) {
        display.setCursor(0, 10);
        display.print(" Thanks for playing! ");
        // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        generalCounter += 1;
        if (generalCounter>24) {
          generalCounter = 0;
          gameMode = 0;
          kisscussCounter = 0;
          superHappyCounter = 100;
        }
      }
      break;
    case 10:
      display.setCursor(0, 10);
      switch (currentSequence) {
        case 0:
          // Pizza
          display.print("  > Feed  TiMiNoo <  ");
          display.drawPng(pizza_26x28, ~0u, 51, 28);
          break;
        case 1:
          // Gamepad
          display.print("> Play with TiMiNoo <");
          display.drawPng(play_32x20, ~0u, 48, 28);
          break;
        case 2:
          // Bubbles
          display.print("  > Wash  TiMiNoo <  ");
          display.drawPng(bubbles_30x30, ~0u, 49, 28);
          break;
        case 3:
          // Heart
          display.print(" > Cuddle  TiMiNoo < ");
          display.drawPng(cuddle_24x24, ~0u, 52, 28);
          break;
        case 4:
          // Book
          display.print("  > Train TiMiNoo <  ");
          display.drawPng(study_26x28, ~0u, 51, 28);
          break;
      }
      // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
      // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
      generalCounter += 1;
      if (generalCounter>48) {
        currentSequence = 0;
        generalCounter = 0;
        gameMode = 0;
      }
      break;
    case 11:
      // Statistics screen
      display.setCursor(0, 10);
      switch (currentSequence) {
        case 0:
          display.print("|||     Hunger    |||");
          gamePick = catHunger;
          display.drawPng(pizza_26x28, ~0u, 4, 28);
          break;
        case 1:
          display.print("|||     Hygiene   |||");
          gamePick = catHygiene;
          display.drawPng(bubbles_30x30, ~0u, 4, 28);
          break;
        case 2:
          display.print("|||    Education  |||");
          gamePick = catEducation;
          display.drawPng(study_26x28, ~0u, 4, 28);
          break;
        case 3:
          display.print("|||     Morale    |||");
          gamePick = catMorale;
          display.drawPng(cuddle_24x24, ~0u, 4, 30);
          break;
        case 4:
          display.print("|||      Fun      |||");
          gamePick = catEntertainment;
          display.drawPng(play_32x20, ~0u, 4, 32);
          break;
      }
      display.setCursor(64, 40);
      switch (gamePick) {
        case 0:
          display.print("---");
          break;
        case 1:
          display.print("[]");
          break;
        case 2:
          display.print("[][]");
          break;
        case 3:
          display.print("[][][]");
          break;
      }
      // M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
      // M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
      generalCounter += 1;
      if (generalCounter>48) {
        generalCounter = 0;
        currentSequence = 0;
        gameMode = 0;
      }
      break;
    case 99:
      // Show version
      display.drawPng(timinoo_logo_128x64, ~0u, 0, 0);
      display.setCursor(0, 0);
      display.print("\n\n\n\n\n\n\n       v1.2.13      ");
      generalCounter += 1;
      if (generalCounter>24) {
        gameMode = 0;
      }
      break;
  }
  if (gameMode < 2) {
    // Score
    ltoa(score, scoreString, 10);
    smoltxt(81, 60, scoreString);
  }

  display.display();
  delay(40);
  display.endWrite();
  M5.update();
}
