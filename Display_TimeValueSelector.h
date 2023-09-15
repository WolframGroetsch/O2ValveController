TimeSelectorValue tsv = { 6, 6, 6 };
byte timeSelectorRowEdit = 0;

const Rectangle timeSelectorButtons[7] = {
  { 90, 45, 40, 30 },    // Button first value up
  { 90, 125, 40, 30 },   // Button first value down
  { 140, 45, 40, 30 },   // Button second value up
  { 140, 125, 40, 30 },  // Button second value down
  { 190, 45, 40, 30 },   // Button third value up
  { 190, 125, 40, 30 },  // Button third value down
  { 80, 175, 160, 30 },  // Confirm button
};


TimeSelectorValue ConvertIntToTimeSelectorValue(int intValue) {
  TimeSelectorValue tsv_temp;
  tsv_temp.ones = intValue % 10;
  tsv_temp.tens = (intValue / 10) % 10;
  tsv_temp.hundreds = (intValue / 100) % 10;
  return tsv_temp;
}


int ConvertTimeSelectorValueToInt(TimeSelectorValue tsv_temp) {
  return ((tsv_temp.hundreds * 100) + (tsv_temp.tens * 10) + tsv_temp.ones);
}


void DrawTimeSelector() {
  tft.fillScreen(BLACK);  // clear the whole screen

  tft.drawRoundRect(80, 35, 160, 130, 8, WHITE);  // Draw bounding box
  PrintText(98, 180, 3, NULL, "Confirm");         // Print confirm text

  // Draw the seven buttoms for the number selector
  tft.drawRoundRect(timeSelectorButtons[0].x, timeSelectorButtons[0].y, timeSelectorButtons[0].w, timeSelectorButtons[0].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[1].x, timeSelectorButtons[1].y, timeSelectorButtons[1].w, timeSelectorButtons[1].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[2].x, timeSelectorButtons[2].y, timeSelectorButtons[2].w, timeSelectorButtons[2].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[3].x, timeSelectorButtons[3].y, timeSelectorButtons[3].w, timeSelectorButtons[3].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[4].x, timeSelectorButtons[4].y, timeSelectorButtons[4].w, timeSelectorButtons[4].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[5].x, timeSelectorButtons[5].y, timeSelectorButtons[5].w, timeSelectorButtons[5].h, 4, WHITE);
  tft.drawRoundRect(timeSelectorButtons[6].x, timeSelectorButtons[6].y, timeSelectorButtons[6].w, timeSelectorButtons[6].h, 4, WHITE);

  // Add button texts
  PrintText(timeSelectorButtons[0].x + 12, timeSelectorButtons[0].y + 5, 3, NULL, "+");
  PrintText(timeSelectorButtons[1].x + 12, timeSelectorButtons[1].y + 5, 3, NULL, "-");
  PrintText(timeSelectorButtons[2].x + 12, timeSelectorButtons[2].y + 5, 3, NULL, "+");
  PrintText(timeSelectorButtons[3].x + 12, timeSelectorButtons[3].y + 5, 3, NULL, "-");
  PrintText(timeSelectorButtons[4].x + 12, timeSelectorButtons[4].y + 5, 3, NULL, "+");
  PrintText(timeSelectorButtons[5].x + 12, timeSelectorButtons[5].y + 5, 3, NULL, "-");
}


void UpdateTimeSelectorValueDisplay() {
  tft.fillRect(90, 80, 140, 40, BLACK);
  //PrintText(x, y, size, font, text);
  PrintText(98, 83, 5, NULL, String(tsv.hundreds).c_str());
  PrintText(148, 83, 5, NULL, String(tsv.tens).c_str());
  PrintText(198, 83, 5, NULL, String(tsv.ones).c_str());
}


// Changes the display mode to the TimeSelector. Stores the row in which we edit the value and populates the selector with the current value, then displays it.
void EnableTimeSelectMode(byte rowToEdit) {
  // should change display state here
  dpState = DisplayState::TIMEEDIT;
  DrawTimeSelector();

  // save the row we are editing
  timeSelectorRowEdit = rowToEdit;

  // convert value to edit into time selector value format of ones, tens and hundreds
  tsv = ConvertIntToTimeSelectorValue(cs.GetTp(timeSelectorRowEdit).GetTime());

  //DrawTimeSelector();
  UpdateTimeSelectorValueDisplay();
}
