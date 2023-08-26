#include <M5StickCPlus.h>
#include <M5GFX.h>
#include <pgmspace.h>
#include <SPIFFS.h>
#include <timinoo_m5_images.h>

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

M5GFX display;
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
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(posX, posY);
  M5.Lcd.print(smallText);
}

void normtxt(int posX, int posY, String normText) {
  // Display normal text (size 2)
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(posX, posY);
  M5.Lcd.print(normText);
}

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  // USB cable to the right (0=bottom; 3=left)
  M5.Lcd.setRotation(1);
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
  M5.Lcd.fillScreen(TFT_BLACK);
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

  switch (gameMode) {
    case 0:
      // Idling
      animationStepMax = 8;
      checkButton();
      // Icon frame
      M5.Lcd.drawXBitmap(69, 1, speech_bubble_56x48_bits, speech_bubble_56x48_width, speech_bubble_56x48_height, TFT_WHITE);
      // Icon (always happy)
      if (superHappyCounter<1) {
        M5.Lcd.drawXBitmap(87, 12, happy_cat_28x28_bits, happy_cat_28x28_width, happy_cat_28x28_height, TFT_WHITE);
      } else {
        M5.Lcd.drawXBitmap(87, 12, super_happy_28x28_bits, super_happy_28x28_width, super_happy_28x28_height, TFT_WHITE);
        superHappyCounter -= 1;
      }
      switch (animationStep) {
        case 1 ... 2:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          break;
        case 3 ... 4:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_002_48x48_bits, cat_sitting_002_48x48_width, cat_sitting_002_48x48_height, TFT_WHITE);
          break;
        case 5 ... 6:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_003_48x48_bits, cat_sitting_003_48x48_width, cat_sitting_003_48x48_height, TFT_WHITE);
          break;
        case 7 ... 8:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_004_48x48_bits, cat_sitting_004_48x48_width, cat_sitting_004_48x48_height, TFT_WHITE);
          break;
      }
      break;
    case 1:
      // Idle - looking left
      animationStepMax = 8;
      checkButton();
      // Icon frame
      M5.Lcd.drawXBitmap(69, 1, speech_bubble_56x48_bits, speech_bubble_56x48_width, speech_bubble_56x48_height, TFT_WHITE);
      // Icon (anything else than happy)
      switch (currentIcon) {
        case 2:
          // Play
          M5.Lcd.drawXBitmap(85, 16, play_32x20_bits, play_32x20_width, play_32x20_height, TFT_WHITE);
          break;
        case 3:
          // Study
          M5.Lcd.drawXBitmap(88, 12, study_26x28_bits, study_26x28_width, study_26x28_height, TFT_WHITE);
          break;
        case 4:
          // Cuddle
          M5.Lcd.drawXBitmap(89, 14, cuddle_24x24_bits, cuddle_24x24_width, cuddle_24x24_height, TFT_WHITE);
          break;
        case 5:
          // Bubbles
          M5.Lcd.drawXBitmap(86, 11, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
          break;
        case 6:
          // Pizza
          M5.Lcd.drawXBitmap(88, 12, pizza_26x28_bits, pizza_26x28_width, pizza_26x28_height, TFT_WHITE);
          break;
      }
      switch (animationStep) {
        case 1 ... 2:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          break;
        case 3 ... 4:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_006_48x48_bits, cat_sitting_006_48x48_width, cat_sitting_006_48x48_height, TFT_WHITE);
          break;
        case 5 ... 6:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_007_48x48_bits, cat_sitting_007_48x48_width, cat_sitting_007_48x48_height, TFT_WHITE);
          break;
        case 7 ... 8:
          checkButton();
          M5.Lcd.drawXBitmap(8, 8, cat_sitting_004_48x48_bits, cat_sitting_004_48x48_width, cat_sitting_004_48x48_height, TFT_WHITE);
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
          M5.Lcd.setCursor(0, 10);
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          switch (selectedFood) {
            case 1:
              M5.Lcd.drawXBitmap(50, 28, strawberry_28x28_bits, strawberry_28x28_width, strawberry_28x28_height, TFT_WHITE);
              M5.Lcd.print("  Yummy  strawberry  ");
              break;
            case 2:
              M5.Lcd.drawXBitmap(50, 28, grape_28x28_bits, grape_28x28_width, grape_28x28_height, TFT_WHITE);
              M5.Lcd.print("    Fresh  grapes    ");
              break;
            case 3:
              M5.Lcd.drawXBitmap(50, 28, milk_28x28_bits, milk_28x28_width, milk_28x28_height, TFT_WHITE);
              M5.Lcd.print("     Farm  milk      ");
              break;
            case 4:
              M5.Lcd.drawXBitmap(50, 28, orange_28x28_bits, orange_28x28_width, orange_28x28_height, TFT_WHITE);
              M5.Lcd.print("    Juicy  orange    ");
              break;
            case 5:
              M5.Lcd.drawXBitmap(50, 28, apple_28x28_bits, apple_28x28_width, apple_28x28_height, TFT_WHITE);
              M5.Lcd.print("    Tasty  apple     ");
              break;
            case 6:
              M5.Lcd.drawXBitmap(50, 28, ghost_28x28_bits, ghost_28x28_width, ghost_28x28_height, TFT_WHITE);
              M5.Lcd.print("      No  food       ");
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
            M5.Lcd.setCursor(0, 55);
            M5.Lcd.print("        Yum !        ");
          }
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          switch (selectedFood) {
            case 1:
              M5.Lcd.drawXBitmap(50, 14, strawberry_28x28_bits, strawberry_28x28_width, strawberry_28x28_height, TFT_WHITE);
              break;
            case 2:
              M5.Lcd.drawXBitmap(50, 14, grape_28x28_bits, grape_28x28_width, grape_28x28_height, TFT_WHITE);
              break;
            case 3:
              M5.Lcd.drawXBitmap(50, 14, milk_28x28_bits, milk_28x28_width, milk_28x28_height, TFT_WHITE);
              break;
            case 4:
              M5.Lcd.drawXBitmap(50, 14, orange_28x28_bits, orange_28x28_width, orange_28x28_height, TFT_WHITE);
              break;
            case 5:
              M5.Lcd.drawXBitmap(50, 14, apple_28x28_bits, apple_28x28_width, apple_28x28_height, TFT_WHITE);
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
      M5.Lcd.drawXBitmap(8, 8, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
      generalCounter += 2;
      if (generalCounter<31) {
        M5.Lcd.drawXBitmap(80, 45 - generalCounter, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(92, 40 - generalCounter, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(104, 45 - generalCounter, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
      } else if (generalCounter>30 && generalCounter < 85) {
        M5.Lcd.drawXBitmap(80, 15, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(92, 10, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(104, 15, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          M5.Lcd.drawXBitmap(kokoXPos, 40, koko_le_snail_26x22_bits, koko_le_snail_26x22_width, koko_le_snail_26x22_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          M5.Lcd.drawXBitmap(97, 40, koko_le_snail_26x22_bits, koko_le_snail_26x22_width, koko_le_snail_26x22_height, TFT_WHITE);
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            currentSequence = 2;
          }
          break;
        case 2:
          // Introduction
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 56, 239, 56, WHITE);
          M5.Lcd.drawXBitmap(97, 21, koko_le_snail_26x22_bits, koko_le_snail_26x22_width, koko_le_snail_26x22_height, TFT_WHITE);
          smoltxt(6, 18, "Get ready for a\nnew lesson with...\n\n~ Koko Le Snail ~");
          generalCounter += 1;
          if (generalCounter>24) {
            generalCounter = 0;
            currentSequence = 3;
          }
          break;
        case 3:
          // Snail wisdom quote
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 56, 239, 56, WHITE);
          M5.Lcd.drawXBitmap(97, 21, koko_le_snail_26x22_bits, koko_le_snail_26x22_width, koko_le_snail_26x22_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(51, 28, study_26x28_bits, study_26x28_width, study_26x28_height, TFT_WHITE);
          normtxt(0, 10, "    + 1  Education   ");
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
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
          M5.Lcd.drawXBitmap(38, 8, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
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
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              break;
            case 16 ... 36:
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(58 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              break;
            case 37 ... 77:
              M5.Lcd.drawXBitmap(23 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(58 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(73 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              break;
            case 78 ... 98:
              M5.Lcd.drawXBitmap(23 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(58 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(73 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(32 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(64 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              break;
            case 99 ... 110:
              M5.Lcd.drawXBitmap(23 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(58 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(73 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(0 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(32 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(64 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(96 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              break;
            case 111 ... 150:
              M5.Lcd.drawXBitmap(23 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(38 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(58 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(73 + randomNumber, 16, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(0 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(32 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(64 + randomNumber, 0, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(96 + randomNumber, 32, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(16 + randomNumber, 48, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
              M5.Lcd.drawXBitmap(48 + randomNumber, 48, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
          }
          M5.Lcd.drawLine(0, 63, generalCounter, 63, WHITE);
          break;
        case 1:
          normtxt(0, 10, "      All clean yay !");
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          M5.Lcd.drawXBitmap(69, 50, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
          M5.Lcd.drawXBitmap(81, 50, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
          M5.Lcd.drawXBitmap(93, 50, cuddle_heart_11x10_bits, cuddle_heart_11x10_width, cuddle_heart_11x10_height, TFT_WHITE);
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
        M5.Lcd.drawXBitmap(3, 18, casino_frame_40x40_bits, casino_frame_40x40_width, casino_frame_40x40_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(44, 18, casino_frame_40x40_bits, casino_frame_40x40_width, casino_frame_40x40_height, TFT_WHITE);
        M5.Lcd.drawXBitmap(85, 18, casino_frame_40x40_bits, casino_frame_40x40_width, casino_frame_40x40_height, TFT_WHITE);
        checkButton();
        for (gameIconXPos = 9; gameIconXPos < 92; gameIconXPos += 41) {
          randomFoodType = random(0, 8);
          switch (randomFoodType) {
            case 0:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, ghost_28x28_bits, ghost_28x28_width, ghost_28x28_height, TFT_WHITE);
              break;
            case 1:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, bar_28x28_bits, bar_28x28_width, bar_28x28_height, TFT_WHITE);
              break;
            case 2:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, strawberry_28x28_bits, strawberry_28x28_width, strawberry_28x28_height, TFT_WHITE);
              break;
            case 3:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, apple_28x28_bits, apple_28x28_width, apple_28x28_height, TFT_WHITE);
              break;
            case 4:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, grape_28x28_bits, grape_28x28_width, grape_28x28_height, TFT_WHITE);
              break;
            case 5:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, milk_28x28_bits, milk_28x28_width, milk_28x28_height, TFT_WHITE);
              break;
            case 6:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, orange_28x28_bits, orange_28x28_width, orange_28x28_height, TFT_WHITE);
              break;
            case 7:
              M5.Lcd.drawXBitmap(gameIconXPos, 24, dollar_28x28_bits, dollar_28x28_width, dollar_28x28_height, TFT_WHITE);
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
        M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        switch (gamePick) {
          case 0:
            // Ghost
            M5.Lcd.drawXBitmap(50, 28, ghost_28x28_bits, ghost_28x28_width, ghost_28x28_height, TFT_WHITE);
            normtxt(0, 10, "   Nothing, boo !    ");
            break;
          case 1:
            // Bar
            M5.Lcd.drawXBitmap(50, 28, bar_28x28_bits, bar_28x28_width, bar_28x28_height, TFT_WHITE);
            normtxt(0, 10, "     + 1 of all!     ");
            break;
          case 2:
            // Strawberry
            M5.Lcd.drawXBitmap(50, 28, strawberry_28x28_bits, strawberry_28x28_width, strawberry_28x28_height, TFT_WHITE);
            normtxt(0, 10, "   + 1  strawberry   ");
            break;
          case 3:
            // Apple
            M5.Lcd.drawXBitmap(50, 28, apple_28x28_bits, apple_28x28_width, apple_28x28_height, TFT_WHITE);
            normtxt(0, 10, "     + 1  apple      ");
            break;
          case 4:
            // Grape
            M5.Lcd.drawXBitmap(50, 28, grape_28x28_bits, grape_28x28_width, grape_28x28_height, TFT_WHITE);
            normtxt(0, 10, "     + 1  grape      ");
            break;
          case 5:
            // Milk
            M5.Lcd.drawXBitmap(50, 28, milk_28x28_bits, milk_28x28_width, milk_28x28_height, TFT_WHITE);
            normtxt(0, 10, "      + 1  milk      ");
            break;
          case 6:
            // Orange
            M5.Lcd.drawXBitmap(50, 28, orange_28x28_bits, orange_28x28_width, orange_28x28_height, TFT_WHITE);
            normtxt(0, 10, "     + 1  orange     ");
            break;
          case 7:
            // Replay + Double
            M5.Lcd.drawXBitmap(50, 28, dollar_28x28_bits, dollar_28x28_width, dollar_28x28_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(50, 14, door_28x30_bits, door_28x30_width, door_28x30_height, TFT_WHITE);
          smoltxt(35, 59, "Knock knock!");
          generalCounter += 1;
          if (generalCounter>4800) {
            currentSequence = 1;
            generalCounter = 0;
          }
          break;
        case 1:
          // Hello
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          if (randomVisit<1000) {
            M5.Lcd.drawXBitmap(96, 14, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
          } else if (randomVisit > 3000) {
            M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          switch (randomVisit) {
            case 0 ... 499:
              smoltxt(45, 59, "I got matcha tea!");
              M5.Lcd.drawXBitmap(96, 14, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
              break;
            case 500 ... 999:
              smoltxt(45, 59, "I got coco cake!");
              M5.Lcd.drawXBitmap(96, 14, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
              break;
            case 3001 ... 3100:
              // Strawberry
              smoltxt(45, 59, "I am cooking");
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
              break;
            case 3101 ... 3136:
              // Cuddle
              smoltxt(45, 59, "I am sad");
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          switch (randomVisit) {
            case 0 ... 999:
              smoltxt(45, 59, "Have some <3");
              M5.Lcd.drawXBitmap(96, 14, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
              if (randomVisit<500) {
                M5.Lcd.drawXBitmap(49, 14, matcha_30x32_bits, matcha_30x32_width, matcha_30x32_height, TFT_WHITE);
              } else if (randomVisit<1000){
                M5.Lcd.drawXBitmap(49, 14, coco_cake_28x32_bits, coco_cake_28x32_width, coco_cake_28x32_height, TFT_WHITE);
              }
              break;
            case 3001 ... 3050:
              // Strawberry
              smoltxt(45, 59, "I need strawberries");
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
              break;
            case 3051 ... 3100:
              // Orange
              smoltxt(45, 59, "I need oranges");
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
              break;
            case 3101 ... 3136:
              // Cuddle
              smoltxt(45, 59, "I need to cuddle");
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
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
          M5.Lcd.setCursor(0, 10);
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          switch (randomVisit) {
            case 0 ... 999:
              if (randomVisit<500) {
                M5.Lcd.drawXBitmap(49, 28, matcha_30x32_bits, matcha_30x32_width, matcha_30x32_height, TFT_WHITE);
              } else {
                M5.Lcd.drawXBitmap(50, 28, coco_cake_28x32_bits, coco_cake_28x32_width, coco_cake_28x32_height, TFT_WHITE);
              }
              M5.Lcd.print("        Yum !        ");
              break;
            case 3001 ... 3050:
              // Strawberry
              smoltxt(45, 59, "Thank you <3");
              M5.Lcd.print("   - 1 Strawberry    ");
              M5.Lcd.drawXBitmap(49, 28, strawberry_28x28_bits, strawberry_28x28_width, strawberry_28x28_height, TFT_WHITE);
              break;
            case 3051 ... 3100:
              // Orange
              smoltxt(45, 59, "Thanks a lot!");
              M5.Lcd.print("     - 1 Orange      ");
              M5.Lcd.drawXBitmap(49, 28, orange_28x28_bits, orange_28x28_width, orange_28x28_height, TFT_WHITE);
              break;
            case 3101 ... 3136:
              // Cuddle
              M5.Lcd.print("  You hug ChiChi  ");
              M5.Lcd.drawXBitmap(40, 28, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
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
          M5.Lcd.drawXBitmap(-24, 13, cat_sitting_001_48x48_bits, cat_sitting_001_48x48_width, cat_sitting_001_48x48_height, TFT_WHITE);
          switch (randomVisit) {
            case 0 ... 999:
              M5.Lcd.drawXBitmap(96, 14, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
              smoltxt(45, 59, "That was yummy!");
              break;
            case 3001 ... 3136:
              M5.Lcd.drawXBitmap(96, 14, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
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
          M5.Lcd.setCursor(0, 10);
          switch (randomVisit) {
            case 0 ... 999:
              M5.Lcd.print("    + 10000 points   ");
              break;
            case 3001 ... 3136:
              M5.Lcd.print("    + 5000 points    ");
              break;
          }
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
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
          M5.Lcd.setCursor(0, 10);
          M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
          M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
          M5.Lcd.drawXBitmap(96, 28, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
          smoltxt(40, 59, "Maybe next time...");
          M5.Lcd.print("   You have none...  ");
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
        M5.Lcd.setCursor(0, 10);
        M5.Lcd.print("[B]  Kiss ~ Cuss  [A]");
        M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
        checkButton();
        if ( frameCounter % 6 == 0 ) {
          randomFoodType = random(0, 4);
          randomGameIconXPos = random(20, 80);
        }
        switch (randomFoodType) {
          case 0:
            M5.Lcd.drawXBitmap(randomGameIconXPos, 28, koko_le_snail_26x22_bits, koko_le_snail_26x22_width, koko_le_snail_26x22_height, TFT_WHITE);
            break;
          case 1:
            M5.Lcd.drawXBitmap(randomGameIconXPos, 28, sophie_28x26_bits, sophie_28x26_width, sophie_28x26_height, TFT_WHITE);
            break;
          case 2:
            M5.Lcd.drawXBitmap(randomGameIconXPos, 28, chichi_30x28_bits, chichi_30x28_width, chichi_30x28_height, TFT_WHITE);
            break;
          case 3:
            M5.Lcd.drawXBitmap(randomGameIconXPos, 28, ghost_28x28_bits, ghost_28x28_width, ghost_28x28_height, TFT_WHITE);
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
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, skull_11x11_bits, skull_11x11_width, skull_11x11_height, TFT_WHITE);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, skull_11x11_bits, skull_11x11_width, skull_11x11_height, TFT_WHITE);
                }
              }
              delay(5);
            }
            break;
          case 3:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, heart_11x10_bits, heart_11x10_width, heart_11x10_height, TFT_WHITE);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, heart_11x10_bits, heart_11x10_width, heart_11x10_height, TFT_WHITE);
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
        M5.Lcd.drawXBitmap(51, 28, cuss_28x28_bits, cuss_28x28_width, cuss_28x28_height, TFT_WHITE);
        M5.Lcd.setCursor(0, 10);
        M5.Lcd.print("      No points      ");
        M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
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
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, heart_11x10_bits, heart_11x10_width, heart_11x10_height, TFT_WHITE);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, heart_11x10_bits, heart_11x10_width, heart_11x10_height, TFT_WHITE);
                }
              }
              delay(5);
            }
            break;
          case 3:
            for ( int xoffseteven = 0; xoffseteven > -24; xoffseteven -= 1 ) {
              for ( int backgroundx = xoffseteven; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 0; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, skull_11x11_bits, skull_11x11_width, skull_11x11_height, TFT_WHITE);
                }
              }
              for ( int backgroundx = abs(xoffseteven)-12; backgroundx < 128; backgroundx += 24 ) {
                for ( int backgroundy = 12; backgroundy < 64; backgroundy += 24 ) {
                  M5.Lcd.drawXBitmap(backgroundx, backgroundy, skull_11x11_bits, skull_11x11_width, skull_11x11_height, TFT_WHITE);
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
        M5.Lcd.drawXBitmap(51, 28, kiss_28x28_bits, kiss_28x28_width, kiss_28x28_height, TFT_WHITE);
        M5.Lcd.setCursor(0, 10);
        M5.Lcd.print("    + 1000 points    ");
        M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
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
        M5.Lcd.setCursor(0, 10);
        M5.Lcd.print(" Thanks for playing! ");
        M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
        M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
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
      M5.Lcd.setCursor(0, 10);
      switch (currentSequence) {
        case 0:
          // Pizza
          M5.Lcd.print("  > Feed  TiMiNoo <  ");
          M5.Lcd.drawXBitmap(51, 28, pizza_26x28_bits, pizza_26x28_width, pizza_26x28_height, TFT_WHITE);
          break;
        case 1:
          // Gamepad
          M5.Lcd.print("> Play with TiMiNoo <");
          M5.Lcd.drawXBitmap(48, 28, play_32x20_bits, play_32x20_width, play_32x20_height, TFT_WHITE);
          break;
        case 2:
          // Bubbles
          M5.Lcd.print("  > Wash  TiMiNoo <  ");
          M5.Lcd.drawXBitmap(49, 28, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
          break;
        case 3:
          // Heart
          M5.Lcd.print(" > Cuddle  TiMiNoo < ");
          M5.Lcd.drawXBitmap(52, 28, cuddle_24x24_bits, cuddle_24x24_width, cuddle_24x24_height, TFT_WHITE);
          break;
        case 4:
          // Book
          M5.Lcd.print("  > Train TiMiNoo <  ");
          M5.Lcd.drawXBitmap(51, 28, study_26x28_bits, study_26x28_width, study_26x28_height, TFT_WHITE);
          break;
      }
      M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
      M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
      generalCounter += 1;
      if (generalCounter>48) {
        currentSequence = 0;
        generalCounter = 0;
        gameMode = 0;
      }
      break;
    case 11:
      // Statistics screen
      M5.Lcd.setCursor(0, 10);
      switch (currentSequence) {
        case 0:
          M5.Lcd.print("|||     Hunger    |||");
          gamePick = catHunger;
          M5.Lcd.drawXBitmap(4, 28, pizza_26x28_bits, pizza_26x28_width, pizza_26x28_height, TFT_WHITE);
          break;
        case 1:
          M5.Lcd.print("|||     Hygiene   |||");
          gamePick = catHygiene;
          M5.Lcd.drawXBitmap(4, 28, bubbles_30x30_bits, bubbles_30x30_width, bubbles_30x30_height, TFT_WHITE);
          break;
        case 2:
          M5.Lcd.print("|||    Education  |||");
          gamePick = catEducation;
          M5.Lcd.drawXBitmap(4, 28, study_26x28_bits, study_26x28_width, study_26x28_height, TFT_WHITE);
          break;
        case 3:
          M5.Lcd.print("|||     Morale    |||");
          gamePick = catMorale;
          M5.Lcd.drawXBitmap(4, 30, cuddle_24x24_bits, cuddle_24x24_width, cuddle_24x24_height, TFT_WHITE);
          break;
        case 4:
          M5.Lcd.print("|||      Fun      |||");
          gamePick = catEntertainment;
          M5.Lcd.drawXBitmap(4, 32, play_32x20_bits, play_32x20_width, play_32x20_height, TFT_WHITE);
          break;
      }
      M5.Lcd.setCursor(64, 40);
      switch (gamePick) {
        case 0:
          M5.Lcd.print("---");
          break;
        case 1:
          M5.Lcd.print("[]");
          break;
        case 2:
          M5.Lcd.print("[][]");
          break;
        case 3:
          M5.Lcd.print("[][][]");
          break;
      }
      M5.Lcd.drawLine(0, 8, 239, 8, WHITE);
      M5.Lcd.drawLine(0, 18, 239, 18, WHITE);
      generalCounter += 1;
      if (generalCounter>48) {
        generalCounter = 0;
        currentSequence = 0;
        gameMode = 0;
      }
      break;
    case 99:
      // Show version
      M5.Lcd.drawXBitmap(56, 35, timinoo_logo_128x64_bits, timinoo_logo_128x64_width, timinoo_logo_128x64_height, TFT_WHITE);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.print("\n\n\n\n\n\n\n       v1.2.13      ");
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
  
  M5.update();
  delay(40);
}
