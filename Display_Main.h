#define GRIDX 8
#define GRIDY 9
const int gridLineXPositions[] = { 0, 25, 75, 125, 175, 225, 275, 320 };
const byte gridLineYPositions[] = { 0, 30, 60, 90, 120, 150, 180, 210, 240 };

// List for the y-axis text line positions for 6 displayable lines. Aligned to be centered between the grid lines.
const byte textLineYPositions[] = { 38, 68, 98, 128, 158, 188, 218 };

const Rectangle loopTickBox = { 180, 216, 18, 18 };

const Rectangle bottomButtons[7] = {
  { 5, 213, 25, 24 },    // Up
  { 35, 213, 25, 24 },   // Down
  { 65, 213, 25, 24 },   // Add
  { 95, 213, 25, 24 },   // Remove
  { 125, 213, 75, 24 },  // Loop
  { 205, 213, 40, 24 },  // Save
  { 250, 213, 70, 24 }   // Start
};

const Rectangle stopButton = { 255, 213, 60, 24 };

// An offset value which is used to "scroll" through lines of time points if we have more than the 6 we can display at once
byte displayedLineOffset = 0;



// Takes the line index (from the grid we display the timpoints in) and returns the corresponding timepoint index.
byte TranslateLineToTimepoint(byte lineIndex) {
  return lineIndex + displayedLineOffset;
}


// Takes the timepoint index and returns the line it is displayed on
int TranslateTimepointToLine(byte tpIndex) {
  return tpIndex - displayedLineOffset;
}


// Checks if a given line is empty
bool IsLineEmpty(byte lineIndex) {
  return !(TranslateLineToTimepoint(lineIndex) < cs.GetActiveTimepoints());
}


// Draws the entire grid to the display
void DrawGrid() {
  // Horizontal lines
  for (int i = 1; i < 8; i++) {
    tft.drawFastHLine(0, gridLineYPositions[i], tft.width(), WHITE);
  }
  // Vertical lines
  for (int j = 1; j < 7; j++) {
    tft.drawFastVLine(gridLineXPositions[j], 0, 210, WHITE);
  }
}


// Writes the header texts
void WriteHeader() {
  PrintText(6, 8, 2, NULL, "#");
  PrintText(45, 8, 2, NULL, "T");
  PrintText(84, 8, 2, NULL, "UOT");
  PrintText(139, 8, 2, NULL, "V1");
  PrintText(189, 8, 2, NULL, "V2");
  PrintText(239, 8, 2, NULL, "V3");
  PrintText(289, 8, 2, NULL, "V4");
}


// Prints an entire timepoint line to the display at the given line index
void WriteTpLine(byte lineIndex) {  // , byte tpIndex, Timepoint tp
  byte linePos = textLineYPositions[lineIndex];
  byte tpIndex = TranslateLineToTimepoint(lineIndex);
  Timepoint tp = cs.GetTp(TranslateLineToTimepoint(lineIndex));

  PrintText(0, linePos, 2, NULL, String(tpIndex + 1).c_str());
  PrintText(33, linePos, 2, NULL, String(tp.GetTime()).c_str());
  PrintText(83, linePos, 2, NULL, tp.GetTimeUnitString().c_str());
  String valveState = "";
  if (tp.GetValveState(0)) {
    valveState = "X";
  } else {
    valveState = "";
  }
  PrintText(145, linePos, 2, NULL, valveState.c_str());
  if (tp.GetValveState(1)) {
    valveState = "X";
  } else {
    valveState = "";
  }
  PrintText(195, linePos, 2, NULL, valveState.c_str());
  if (tp.GetValveState(2)) {
    valveState = "X";
  } else {
    valveState = "";
  }
  PrintText(245, linePos, 2, NULL, valveState.c_str());
  if (tp.GetValveState(3)) {
    valveState = "X";
  } else {
    valveState = "";
  }
  PrintText(295, linePos, 2, NULL, valveState.c_str());
}


void UpdateLoopState() {
  tft.fillRect(loopTickBox.x, loopTickBox.y, loopTickBox.w, loopTickBox.h, BLACK);          // overwrite the checkbox
  tft.drawRoundRect(loopTickBox.x, loopTickBox.y, loopTickBox.w, loopTickBox.h, 2, WHITE);  // redraw the checkbox border
  if (cs.GetIsLooping()) PrintText(loopTickBox.x + 4, loopTickBox.y, 2, NULL, "x");         // fill it with the cross according to state
}


void DrawBottomButtons() {
  tft.fillRect(0, 211, 320, 29, BLACK);  // clear bottom
  for (int i = 0; i < 7; i++) {
    tft.drawRoundRect(bottomButtons[i].x, bottomButtons[i].y, bottomButtons[i].w, bottomButtons[i].h, 4, WHITE);  // Draw bounding box
  }

  // Write the texts into the buttons
  PrintSymbol(bottomButtons[0].x + 7, textLineYPositions[6], 2, NULL, 0x18); // See https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf for an overview of the characters / codes. 
  PrintSymbol(bottomButtons[1].x + 7, textLineYPositions[6], 2, NULL, 0x19);
  PrintText(bottomButtons[2].x + 7, textLineYPositions[6], 2, NULL, "+");
  PrintText(bottomButtons[3].x + 7, textLineYPositions[6], 2, NULL, "-");
  PrintText(bottomButtons[4].x + 7, textLineYPositions[6], 2, NULL, "Loop");
  PrintText(bottomButtons[5].x + 7, textLineYPositions[6], 1, NULL, "Save/");
  PrintText(bottomButtons[5].x + 7, textLineYPositions[6] + 8, 1, NULL, "Load");
  PrintText(bottomButtons[6].x + 7, textLineYPositions[6], 2, NULL, "Start");

  // Draw the loop state checkbox with state at end of loop button
  UpdateLoopState();
}


void DrawMainScreenTpContent() {
  // get the number of entries in the ControlSeries and display them
  for (int i = 0; i < 6; i++) {
    if (cs.isTpAtIndexActive(i + displayedLineOffset)) {
      //byte tpIndex = TranslateLineToTimepoint(i);
      WriteTpLine(i);  // , tpIndex, cs.GetTp(tpIndex)
    }
  }
}


void DrawMainScreen() {
  tft.fillScreen(BLACK);
  DrawGrid();
  WriteHeader();
  DrawBottomButtons();
  DrawMainScreenTpContent();
}


void ClearTimePointNumberInLine(byte lineIndex) {
  byte linePos = gridLineYPositions[lineIndex] + 1;
  tft.fillRect(gridLineXPositions[0], linePos, 24, 29, BLACK);
}


void ClearTimeValueInLine(byte lineIndex) {
  byte linePos = gridLineYPositions[lineIndex] + 1;
  tft.fillRect(gridLineXPositions[1] + 1, linePos, 49, 29, BLACK);
}


void ClearUnitOfTimeInLine(byte lineIndex) {
  byte linePos = gridLineYPositions[lineIndex] + 1;
  tft.fillRect(gridLineXPositions[2] + 1, linePos, 49, 29, BLACK);
}


void ClearValveStatesInLine(byte lineIndex) {
  byte linePos = gridLineYPositions[lineIndex] + 1;
  tft.fillRect(gridLineXPositions[3] + 1, linePos, 49, 29, BLACK);
  tft.fillRect(gridLineXPositions[4] + 1, linePos, 49, 29, BLACK);
  tft.fillRect(gridLineXPositions[5] + 1, linePos, 49, 29, BLACK);
  tft.fillRect(gridLineXPositions[6] + 1, linePos, 49, 29, BLACK);
}


// Clears the content of a line without touching the grid lines so we can easily redraw the line content.
void ClearLine(byte lineIndex) {
  ClearTimePointNumberInLine(lineIndex);
  ClearTimeValueInLine(lineIndex);
  ClearUnitOfTimeInLine(lineIndex);
  ClearValveStatesInLine(lineIndex);
}


void ClearAllLines() {
  ClearLine(1);
  ClearLine(2);
  ClearLine(3);
  ClearLine(4);
  ClearLine(5);
  ClearLine(6);
}


void AddTimepointButton() {
  if (cs.AddTimepoint()) {
    // make sure that when we add a timepoint we scroll to the bottom so it is visible
    if (cs.GetActiveTimepoints() >= 6) {
      displayedLineOffset = cs.GetActiveTimepoints() - 6;
    }
    ClearAllLines();
    DrawMainScreenTpContent();
  }
}


void RemoveTimepointButton() {
  if (cs.RemoveTimepoint()) {
    // make sure that when we remove a timepoint we scroll the others after so we are not left with an empty line at the bottom
    if (cs.GetActiveTimepoints() >= 6) {
      displayedLineOffset = cs.GetActiveTimepoints() - 6;
    }
    ClearAllLines();
    DrawMainScreenTpContent();
  }
}


void ScrollUp() {
  // check if it is valid - confines the scroll so we wont be left with an empty line
  if (displayedLineOffset > 0) {
    displayedLineOffset--;
  } else {
    return;
  }

  ClearAllLines();            // clear all content
  DrawMainScreenTpContent();  // redraw with new line index
}


void ScrollDown() {
  // check if it is valid - confines the scroll so we wont be left with an empty line
  if (displayedLineOffset < cs.GetActiveTimepoints() - 6) {
    displayedLineOffset++;
  } else {
    return;
  }

  ClearAllLines();            // clear all content
  DrawMainScreenTpContent();  // redraw with new line index
}


// Methods for run mode

void DrawStopButton() {
  tft.fillRect(0, 211, 320, 29, BLACK);                                                 // clear bottom
  tft.drawRoundRect(stopButton.x, stopButton.y, stopButton.w, stopButton.h, 4, WHITE);  // Draw bounding box
  PrintText(stopButton.x + 7, textLineYPositions[6], 2, NULL, "Stop");
  PrintText(10, textLineYPositions[6], 2, NULL, "Next TP in");
}


void HighlightTimepoint(byte tpIndex) {
  int lineToHighlight = 0;

  // First we need to check which line our timepoint is on and if it is visible
  lineToHighlight = TranslateTimepointToLine(tpIndex);
#if LOGGING
  Serial.println("Line to highlight = " + String(lineToHighlight));
#endif

  // try to scroll the content so we highlight line 3 and can see the next three timepoints
  if (lineToHighlight > 2 && tpIndex < (cs.GetActiveTimepoints() - 3)) {
    // we need to scroll down by as much as the value is bigger than 5
    displayedLineOffset += (lineToHighlight - 2);
    ClearAllLines();            // clear all content
    DrawMainScreenTpContent();  // redraw with new line index
  }

  // make sure the tp we highlight is not above or below the table
  if (lineToHighlight < 0) {
    // we need to scroll up by as much as the value is minus
    displayedLineOffset += lineToHighlight;
    ClearAllLines();            // clear all content
    DrawMainScreenTpContent();  // redraw with new line index
  }
  else if (lineToHighlight > 5) {
    // we need to scroll down by as much as the value is bigger than 5
    displayedLineOffset += (lineToHighlight - 5);
    ClearAllLines();            // clear all content
    DrawMainScreenTpContent();  // redraw with new line index
  }

  // fetch the translated line index again since we scrolled the content
  lineToHighlight = TranslateTimepointToLine(tpIndex);

  // overwrite old highlight by redrawing the grid
  DrawGrid();
  // draw the highlight
  if (lineToHighlight < 6) {
    tft.drawFastHLine(0, gridLineYPositions[lineToHighlight + 1], tft.width(), RED);
    tft.drawFastHLine(0, gridLineYPositions[lineToHighlight + 2], tft.width(), RED);
  }
}

void WriteTimeTillNextTp(String text) {
  tft.fillRect(139, 211, 115, 29, BLACK);
  PrintText(140, textLineYPositions[6], 2, NULL, text.c_str());
}
