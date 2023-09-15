
// Stores the position and size of buttons that we draw to the screen. Used to draw them and check for clicks. 
struct Rectangle {
  int x;  // X position
  int y;  // Y position
  int w;  // Width
  int h;  // Height
};

// Storage for the three values in the time selector
struct TimeSelectorValue {
  int ones;
  int tens;
  int hundreds;
};