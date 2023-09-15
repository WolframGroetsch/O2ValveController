const Rectangle saveLoadButtons[10] = {
  { 5, 10, 140, 24 },    // Save Slot 1
  { 165, 10, 140, 24 },   // Load Slot 1
  { 5, 50, 140, 24 },   // Save Slot 2
  { 165, 50, 140, 24 },   // Load Slot 2
  { 5, 90, 140, 24 },  // Save Slot 3
  { 165, 90, 140, 24 },  // Load Slot 3
  { 5, 130, 140, 24 },   // Save Slot 4
  { 165, 130, 140, 24 },   // Load Slot 4
  { 5, 170, 140, 24 },  // Save Slot 5
  { 165, 170, 140, 24 }  // Load Slot 5
};

const Rectangle returnButton = { 230, 213, 85, 24 };


void DrawSaveLoadButtons() {
  for (int i = 0; i < 10; i++) {
    tft.drawRoundRect(saveLoadButtons[i].x, saveLoadButtons[i].y, saveLoadButtons[i].w, saveLoadButtons[i].h, 4, WHITE);  // Draw bounding box
  }

  tft.drawRoundRect(returnButton.x, returnButton.y, returnButton.w, returnButton.h, 4, WHITE);  // Draw bounding box
  PrintText(returnButton.x + 7, 218, 2, NULL, "Return");

  // Write the texts into the buttons
  PrintText(saveLoadButtons[0].x + 7, saveLoadButtons[0].y + 5, 2, NULL, "Save Slot 1");
  PrintText(saveLoadButtons[1].x + 7, saveLoadButtons[1].y + 5, 2, NULL, "Load Slot 1");
  PrintText(saveLoadButtons[2].x + 7, saveLoadButtons[2].y + 5, 2, NULL, "Save Slot 2");
  PrintText(saveLoadButtons[3].x + 7, saveLoadButtons[3].y + 5, 2, NULL, "Load Slot 2");
  PrintText(saveLoadButtons[4].x + 7, saveLoadButtons[4].y + 5, 2, NULL, "Save Slot 3");
  PrintText(saveLoadButtons[5].x + 7, saveLoadButtons[5].y + 5, 2, NULL, "Load Slot 3");
  PrintText(saveLoadButtons[6].x + 7, saveLoadButtons[6].y + 5, 2, NULL, "Save Slot 4");
  PrintText(saveLoadButtons[7].x + 7, saveLoadButtons[7].y + 5, 2, NULL, "Load Slot 4");
  PrintText(saveLoadButtons[8].x + 7, saveLoadButtons[8].y + 5, 2, NULL, "Save Slot 5");
  PrintText(saveLoadButtons[9].x + 7, saveLoadButtons[9].y + 5, 2, NULL, "Load Slot 5");
}


void DrawSaveLoadScreen(){
  tft.fillScreen(BLACK);
  DrawSaveLoadButtons();
}
