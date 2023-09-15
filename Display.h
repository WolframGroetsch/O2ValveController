// Display code - Resolution 320*240 (Pixel)
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_TFTLCD.h>  // Hardware-specific library
#include <TouchScreen.h>

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Calibration values for touchscreen
#define TS_MINX 177  // 177
#define TS_MINY 150  // 150
#define TS_MAXX 913  // 913
#define TS_MAXY 911  // 911

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define WHITE 0xFFFF

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define UIDELAY 200

// // LANDSCAPE CALIBRATION     320 x 240 - Calibration values obtained for our specific touch display
// x = map(p.y, LEFT=150, RT=911, 0, 320)
// y = map(p.x, TOP=913, BOT=177, 0, 240)


// Used to print a piece of text to the display
void PrintText(int x, int y, int sz, const GFXfont *f, const char *msg) {
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(GREEN);
  tft.setTextSize(sz);
  tft.print(msg);
}


// Prints a symbol of the given hex code to the given position. See https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf on page 16 for the table of symbols. 
void PrintSymbol(int x, int y, int sz, const GFXfont *f, const unsigned long charIndex){
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(GREEN);
  tft.setTextSize(sz);
  tft.write(charIndex);
}


// After we have defined common variables and methods, add the sub headers containing methods for the main and timevalueselector screen.
#include "Display_Main.h"
#include "Display_SaveLoad.h"
#include "Display_TimeValueSelector.h"


void TFTSetup() {
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481;  //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.cp437(true);

  // write start screen and little delay
  PrintText(10, 25, 2, NULL, "Oxygen Valve Controller");
  PrintText(10, 55, 2, NULL, "Version 1.0");
  PrintText(10, 95, 2, NULL, "Developed by");
  PrintText(10, 125, 2, NULL, "Wolfram Groetsch");
  PrintText(10, 155, 2, NULL, "at Tampere University");
  PrintText(10, 185, 2, NULL, "2023");
  delay(3000);
}


bool ProcessMainStopButtonClicked(int x, int y) {
  if (x >= stopButton.x && x < (stopButton.x + stopButton.w) && y >= stopButton.y && y < (stopButton.y + stopButton.h)) {
#if LOGGING
    Serial.println("Stop button clicked.");
#endif
    StopControlSeries();
    return true;
  }
  return false;
}


bool ProcessMainSaveLoadButtonsClicked(int x, int y) {
  // return button
  if (x >= returnButton.x && x < (returnButton.x + returnButton.w) && y >= returnButton.y && y < (returnButton.y + returnButton.h)) {
    dpState = DisplayState::MAIN_EDIT;
    DrawMainScreen();
    return true;
  }

  byte btnIndex = 255;

  for (int i = 0; i < 10; i++) {
    if (x >= saveLoadButtons[i].x && x < (saveLoadButtons[i].x + saveLoadButtons[i].w) && y >= saveLoadButtons[i].y && y < (saveLoadButtons[i].y + saveLoadButtons[i].h)) {
      btnIndex = i;
      //#if LOGGING
      Serial.println("Clicked on save/load button with index " + String(btnIndex));
      //#endif
      break;
    }
  }

  // if no button was found, we return false and skip the rest
  if (btnIndex == 255) return false;

  switch (btnIndex) {
    case 0:  // Save Slot 1
      SaveToSlot(0, cs);
      break;

    case 1:  // Load Slot 1
      LoadFromSlot(0);
      break;

    case 2:  // Save Slot 2
      SaveToSlot(1, cs);
      break;

    case 3:  // Load Slot 2
      LoadFromSlot(1);
      break;

    case 4:  // Save Slot 3
      SaveToSlot(2, cs);
      break;

    case 5:  // Load Slot 3
      LoadFromSlot(2);
      break;

    case 6:  // Save Slot 4
      SaveToSlot(3, cs);
      break;

    case 7:  // Load Slot 4
      LoadFromSlot(3);
      break;

    case 8:  // Save Slot 5
      SaveToSlot(4, cs);
      break;

    case 9:  // Load Slot 5
      LoadFromSlot(4);
      break;
  }

  // We either saved or loaded something here at this point, so jump back to main edit screen to display action to user.
  dpState = DisplayState::MAIN_EDIT;
  DrawMainScreen();
  return true;
}


bool ProcessMainBottomButtonsClicked(int x, int y) {
  byte btnIndex = 255;

  for (int i = 0; i < 7; i++) {
    if (x >= bottomButtons[i].x && x < (bottomButtons[i].x + bottomButtons[i].w) && y >= bottomButtons[i].y && y < (bottomButtons[i].y + bottomButtons[i].h)) {
      btnIndex = i;
#if LOGGING
      Serial.println("Clicked on bottom row buttom with index " + String(btnIndex));
#endif
      break;
    }
  }

  // if no button was found, we return false and skip the rest
  if (btnIndex == 255) return false;

  switch (btnIndex) {
    case 0:  // Up button
      ScrollUp();
      break;

    case 1:  // Down button
      ScrollDown();
      break;

    case 2:  // Add button
      AddTimepointButton();
      break;

    case 3:  // Remove button
      RemoveTimepointButton();
      break;

    case 4:  // Loop button
      cs.ToggleIsLooping();
      UpdateLoopState();
      break;

    case 5:  // Save/Load button
      dpState = DisplayState::MAIN_SAVELOAD;
      DrawSaveLoadScreen();
      break;

    case 6:  // Start Button
      StartControlSeries();
      break;
  }

  return true;
}


// Checks if a touch click was done in the main screen within the grid or on the buttons below
void ProcessMainTouchEvent(int x, int y) {
  byte col = 0;
  byte row = 0;

  // We are in run mode, see if the stop button was clicked
  if (dpState == DisplayState::MAIN_RUN) {
    if (ProcessMainStopButtonClicked(x, y)) {
      delay(UIDELAY);
      return;
    }
  }

  // If we are in the save load window, check what we clicked there
  if (dpState == DisplayState::MAIN_SAVELOAD) {
    if (ProcessMainSaveLoadButtonsClicked(x, y)) {
      delay(UIDELAY);
      return;
    }
  }

  // if we are not in the edit mode, we do not care what was clicked below
  if (dpState != DisplayState::MAIN_EDIT) return;

  // Check first if we clicked into any of the bottom buttons. If so we delay and skip further code.
  if (ProcessMainBottomButtonsClicked(x, y)) {
    delay(UIDELAY);
    return;
  }

  for (byte n = 0; n < GRIDX; n++) {
    byte nPlusIndex = n + 1;
    if (nPlusIndex >= GRIDX) {
      nPlusIndex = GRIDX - 1;  // make sure we do not overflow
    }
    if ((x > gridLineXPositions[n]) && (x < gridLineXPositions[nPlusIndex])) {
      col = n;
#if LOGGING
      Serial.println("Clicked into column index " + String(n));
#endif
    }
  }

  for (byte j = 0; j < GRIDY; j++) {
    byte jPlusIndex = j + 1;
    if (jPlusIndex >= GRIDY) {
      jPlusIndex = GRIDY - 1;  // make sure we do not overflow
    }
    if ((y > gridLineYPositions[j]) && (y < gridLineYPositions[jPlusIndex])) {
      row = j;
#if LOGGING
      Serial.println("Clicked into row index " + String(j));
#endif
    }
  }

  // skip if we did not find a row or column or if we are still on the index or header row
  if (col == 0 || row == 0) {
#if LOGGING
    Serial.println("Skip touch event");
#endif
    delay(UIDELAY);
    return;
  }

  // skip if the line we clicked does not contain anything
  if (IsLineEmpty(row - 1)) {
    delay(UIDELAY);
    return;
  }

  // Time field clicked
  if (col == 1) {
    EnableTimeSelectMode(TranslateLineToTimepoint(row - 1));
  }

  // Time unit clicked
  if (col == 2) {
    // update the display only if we manage to successfully toggle the time unit (if the clicked row index is valid)
    if (cs.ToggleTimeUnit(TranslateLineToTimepoint(row - 1))) {
      ClearUnitOfTimeInLine(row);
      WriteTpLine(row - 1);  // , TranslateLineToTimepoint(row - 1), cs.GetTp(row - 1)
    }
  }

  // Valves clicked
  if (col >= 3 && col <= 6) {
    // update the display only if we manage to successfully toggle the valve (if the clicked row index is valid)
    if (cs.ToggleValve(TranslateLineToTimepoint(row - 1), col - 3)) {
      ClearValveStatesInLine(row);
      WriteTpLine(row - 1);  // , TranslateLineToTimepoint(row - 1), cs.GetTp(row - 1)
    }
  }

  delay(UIDELAY);  // delay after a successfull touch so we do not update UI unnecessarily fast.
}


// Checks if a touch was made to any of the buttons in the time selector
void ProcessTimeSelectorTouchEvent(int x, int y) {
  // we need to check here in which of the 7 buttons or so the click was
  // there will be three values display as ones, thens and hundreds. Each has a button above and below with an arrow to increase or decrease them between 0 and 9.
  // The seventh button is to confirm it, with which we will convert these three values into a single one and set it to the time point.
  byte btnIndex = 255;
  bool confirmed = false;

  for (int i = 0; i < 7; i++) {
    if (x >= timeSelectorButtons[i].x && x < (timeSelectorButtons[i].x + timeSelectorButtons[i].w) && y >= timeSelectorButtons[i].y && y < (timeSelectorButtons[i].y + timeSelectorButtons[i].h)) {
      btnIndex = i;
#if LOGGING
      Serial.println("Clicked on buttom with index " + String(btnIndex));
#endif
      break;
    }
  }

  if (btnIndex == 255) {
    return;  // we did not find a valind btnIndex so we skip
  }

  switch (btnIndex) {
    case 0:
      tsv.hundreds++;
      if (tsv.hundreds > 9) tsv.hundreds = 0;
      break;
    case 1:
      tsv.hundreds--;
      if (tsv.hundreds < 0) tsv.hundreds = 9;
      break;
    case 2:
      tsv.tens++;
      if (tsv.tens > 9) tsv.tens = 0;
      break;
    case 3:
      tsv.tens--;
      if (tsv.tens < 0) tsv.tens = 9;
      break;
    case 4:
      tsv.ones++;
      if (tsv.ones > 9) tsv.ones = 0;
      break;
    case 5:
      tsv.ones--;
      if (tsv.ones < 0) tsv.ones = 9;
      break;
    case 6:
      confirmed = true;
#if LOGGING
      Serial.println("Confirm btn clicked.");
#endif
      break;
  }

  if (confirmed) {
    int newTime = ConvertTimeSelectorValueToInt(tsv);
    cs.SetTime(timeSelectorRowEdit, newTime);
    dpState = DisplayState::MAIN_EDIT;
    DrawMainScreen();
  } else {
    UpdateTimeSelectorValueDisplay();
    delay(UIDELAY);  // delay after a successfull touch so we do not update UI unnecessarily fast.
  }
}


void ReadTouch() {
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width and tft.height
    int x = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
    int y = map(p.x, TS_MAXX, TS_MINX, 0, tft.height());

    // decide here in which display mode we are and fire the appropriate touch processing event
    if (dpState == DisplayState::MAIN_EDIT || dpState == DisplayState::MAIN_RUN || dpState == DisplayState::MAIN_SAVELOAD) {
      ProcessMainTouchEvent(x, y);
    } else if (dpState == DisplayState::TIMEEDIT) {
      ProcessTimeSelectorTouchEvent(x, y);
    }

    // // Draw spot for debugging
    // if (((y - 1) > 0) && ((y + 1) < tft.height())) {
    //   tft.fillCircle(x, y, 1, RED);
    // }
  }
}
