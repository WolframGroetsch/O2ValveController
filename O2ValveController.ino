// Defines
#define VALVES 4
#define MAXTIMEPOINTS 28 // maximum number of timepoints is defined to be 28 which results in a total length of the ControlSeries instance of 198 bytes. This allows us to save 5 entries to the EEPROM.
#define LOGGING false // Keep at false unless absolutley needed. Arduino UNO runs low of memory with logging activated and starts acting up. First the display does not display anything anymore when Serial monitor is openend, then timepoints are not displayed anymore, likely caused by graphics library. 

// Add headers here, some depend on defines above
//#include <avr/pgmspace.h>
#include "Enumerators.h"
#include "Structs.h"
#include "TimeUnitConversion.h"
#include "TimepointClass.h"
#include "ControlSeriesClass.h"

// Global instance of our control series
ControlSeries cs;
// Global instance of the display state we keep to track currently displayed state
DisplayState dpState = DisplayState::MAIN_EDIT;

// forward declarations - declare methods contained in RunControlSeries.h but which are used in Display.h which is included before
void StartControlSeries();
void StopControlSeries();

// Add headers here that depend on some of the defines or variables above
#include "EEPROM_SaveLoad.h"
#include "Display.h"
#include "RunControlSeries.h"


void setup() {
#if LOGGING
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
#endif

  // initialize the TFT display and its touch component
  TFTSetup();

  // create a new global instance of the ControlSeries class
  //cs = NewControlSeriesInstance();

#if LOGGING
  cs.log();
  Serial.print("Size of the cs array: " + String(sizeof(cs)));
#endif

  // Set the display to main state and draw it
  dpState = DisplayState::MAIN_EDIT;
  DrawMainScreen();

  // Initialize the valve pins as output
  InitializeValvePins();
  TurnAllValvesOff();
}


void loop() {
  // Check if a new timepoint is reached
  UpdateControlSeriesStates();

  // Read touch display inputs
  ReadTouch();
}
