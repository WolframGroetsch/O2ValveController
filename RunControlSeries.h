#define V1 A5  // Valve 1 pin
#define V2 10  // Valve 2 pin
#define V3 11  // Valve 3 pin
#define V4 12   // Valve 4 pin
#define TIMESERIESUPDATEDELAYINMS 20

unsigned long tpStartTime = 0;  // time the last timepoint was started
unsigned long lastUpdate = 0;   // time we last time checked if we need to update

String lastWrittenTpString = "";

bool controlSeriesRunning = false;
byte currentTimepoint = 0;


void InitializeValvePins() {
  pinMode(V1, OUTPUT);
  pinMode(V2, OUTPUT);
  pinMode(V3, OUTPUT);
  pinMode(V4, OUTPUT);
}


// Turns a single valve on at the given index and all others off
void SetSingleValveOn(byte valveIndex) {
  switch (valveIndex) {
    case 0:
      digitalWrite(V1, HIGH);
      digitalWrite(V2, LOW);
      digitalWrite(V3, LOW);
      digitalWrite(V4, LOW);
      break;

    case 1:
      digitalWrite(V1, LOW);
      digitalWrite(V2, HIGH);
      digitalWrite(V3, LOW);
      digitalWrite(V4, LOW);
      break;

    case 2:
      digitalWrite(V1, LOW);
      digitalWrite(V2, LOW);
      digitalWrite(V3, HIGH);
      digitalWrite(V4, LOW);
      break;

    case 3:
      digitalWrite(V1, LOW);
      digitalWrite(V2, LOW);
      digitalWrite(V3, LOW);
      digitalWrite(V4, HIGH);
      break;
  }
}


// Turns all valves off
void TurnAllValvesOff() {
  digitalWrite(V1, LOW);
  digitalWrite(V2, LOW);
  digitalWrite(V3, LOW);
  digitalWrite(V4, LOW);
}


void UpdateTimeTillNextTp(unsigned long passedTime, unsigned long tpDuration) {
  // Skip execution here if we called the method at a timepoint change where passed time might be bigger than duration to avoid an overflow
  if (passedTime >= tpDuration) return;

  unsigned long timeLeft = tpDuration - passedTime;
  TimeUnitEnum unit;
  unsigned long timeLeftInUnit = 0;
  if (timeLeft / 3600000 > 1) {
    unit = TimeUnitEnum::HOUR;
    timeLeftInUnit = timeLeft / 3600000;
  }
  else if (timeLeft / 60000 > 1) {
    unit = TimeUnitEnum::MINUTE;
    timeLeftInUnit = timeLeft / 60000;
  }
  else if (timeLeft / 1000 < 120) {
    unit = TimeUnitEnum::SECOND;
    timeLeftInUnit = timeLeft / 1000;
  }
  //Serial.println("Time left in ms = " + String(timeLeft));
  //Serial.println("Next TP in " + String(timeLeftInUnit) + GetTimeUnitEnumAbbrString(unit));
  String tpString = String(timeLeftInUnit + 1) + " " + GetTimeUnitEnumAbbrString(unit, false);
  if (tpString != lastWrittenTpString) {
    lastWrittenTpString = tpString;
    WriteTimeTillNextTp(tpString);
  }
}


void SetInitialTp() {
  tpStartTime = millis();  // reset start time

  // Get the new active valve and set it. But make sure its index was found (is 255 if not found, i.e. none was set.) In that case we turn all off.
  byte activeValveIndex = cs.GetActiveValve(currentTimepoint);
  if (activeValveIndex != 255) {
    SetSingleValveOn(activeValveIndex);
  } else {
    TurnAllValvesOff();
  }
  UpdateTimeTillNextTp(0, cs.GetTimeInMillis(currentTimepoint)); // Force an immediate update of the remaining time to display it
#if LOGGING
  Serial.println("ControlSeries updated. Initial Timepoint = " + String(currentTimepoint) + ". Valve " + String(activeValveIndex) + " is turned on.");
#endif
}


void StartControlSeries() {
  currentTimepoint = 0;         // reset the current timepoint
  controlSeriesRunning = true;  // set flag to active
#if LOGGING
  Serial.println("Start control series");
#endif
  SetInitialTp();

  // Change the display state
  dpState = DisplayState::MAIN_RUN;
  DrawStopButton();

  // Force the initial timepoint to be highlighted
  HighlightTimepoint(currentTimepoint);
}


void StopControlSeries() {
  controlSeriesRunning = false;
  TurnAllValvesOff();
  //TODO return back to the edit mode screen here
  dpState = DisplayState::MAIN_EDIT;
  DrawBottomButtons();
  DrawGrid();  // overwrite all highlights
}





void UpdateControlSeriesStates() {
  // When the control series is not active, skip further execution
  if (!controlSeriesRunning) return;

  // Check when the last update was. We update the timepoint here only every 200ms to save some followup checks every loop.
  if ((millis() - lastUpdate) < TIMESERIESUPDATEDELAYINMS) return;
  lastUpdate = millis();

  // check if we passed the time of this timepoint
  unsigned long currentTime = millis();
  unsigned long tpPassedTime = currentTime - tpStartTime;
  unsigned long tpDuration = cs.GetTimeInMillis(currentTimepoint);

  UpdateTimeTillNextTp(tpPassedTime, tpDuration);

  // We skip further execution here if we have not reached the timepoint timelimit yet (so we keep the current state as is)
  if (tpPassedTime < tpDuration) return;

  // Fetch the new timepoint start time
  tpStartTime = millis();

  // Increase the timepoint. Make sure we are in bounds. Also check that if we are looping we start again and dont end.
  currentTimepoint++;
  if (currentTimepoint >= cs.GetActiveTimepoints()) {
    if (cs.GetIsLooping()) {
#if LOGGING
      Serial.println("We are looping over this timeseries. Start from beginning.");
#endif
      currentTimepoint = 0;
    } else {
      StopControlSeries();
      return;
    }
  }

  // If we need to make a change to the valves, get the new active valve and set it. But make sure its index was found (is 255 if not found, i.e. none was set.) In that case we turn all off.
  byte activeValveIndex = cs.GetActiveValve(currentTimepoint);
  if (activeValveIndex != 255) {
    SetSingleValveOn(activeValveIndex);
  } else {
    TurnAllValvesOff();
  }
#if LOGGING
  Serial.println("ControlSeries updated. New Timepoint = " + String(currentTimepoint) + ". Valve " + String(activeValveIndex) + " is turned on.");
#endif

  // Finally we update the active timepoint on the run screen
  HighlightTimepoint(currentTimepoint);
}
