class ControlSeries {
  public:
    // Constructor (initialize the class)
    ControlSeries() {
      this->isLooping = false;
      this->activeTimepoints = 4;
      for (int i = 0; i < length(); i++) {
#if LOGGING
        Serial.println("ControlSeriesClass.h: Default valve toggled at index " + String(i) + ": " + String(i % 4));
#endif
        this->timePoints[i].ToggleValve(i % 4);
      }
    };

    // Destructor (clean up resources)
    //~ControlSeries();

    // Public methods (functions)

    // Toggles the time unit at the given timepoint index. Checks if the index is valid and returns true if successfully toggled.
    bool SetTime(int tpIndex, int newTime) {
      if (tpIndex >= this->length()) {
        return false;
      }
      this->timePoints[tpIndex].SetTime(newTime);
      return true;
    }

    // Toggles the time unit at the given timepoint index. Checks if the index is valid and returns true if successfully toggled.
    bool ToggleTimeUnit(int tpIndex) {
      if (tpIndex >= this->length()) {
        return false;
      }
      this->timePoints[tpIndex].ToggleTimeUnit();
      return true;
    }


    // Toggles the valve at the given timepoint index and valve index. Checks if the index is valid and returns true if successfully toggled.
    bool ToggleValve(int tpIndex, int valveIndex) {
      if (tpIndex >= this->length()) {
        return false;
      }
      this->timePoints[tpIndex].ToggleValve(valveIndex);
      return true;
    }


    void ToggleIsLooping() {
      this->isLooping = !this->isLooping;
    }


    bool AddTimepoint() {
      if (this->activeTimepoints >= MAXTIMEPOINTS) return false;

      this->activeTimepoints = this->activeTimepoints + 1;
      return true;
    }


    bool RemoveTimepoint() {
      if (this->activeTimepoints <= 1) return false;

      this->activeTimepoints = this->activeTimepoints - 1;
      return true;
    }


    byte GetActiveTimepoints() {
      return this->activeTimepoints;
    }


    Timepoint GetTp(int tpIndex) {
      if (tpIndex >= this->length()) {
        return;
      }
      return this->timePoints[tpIndex];
    }


    bool GetIsLooping() {
      return this->isLooping;
    }


    // Returns the index of the active valve. If it is 255, no valve was active
    byte GetActiveValve(byte tpIndex) {
      byte activeValveIndex = 255;
      for (int i = 0; i < VALVES; i++) {
        if (timePoints[tpIndex].GetValveState(i)) {
          activeValveIndex = i;
          break;
        }
      }
      return activeValveIndex;
    }


    unsigned long GetTimeInMillis(byte tpIndex) {
      return timePoints[tpIndex].GetTimeInMillis();
    }


    // Logs the entire content of this ControlSeries class to the console.
    void log() const {
      byte numOfEntries = this->length();
      for (int i = 0; i < numOfEntries; i++) {
#if LOGGING
        Serial.println("ControlSeriesClass.h: " + timePoints[i].toString());
#endif
      }
    }


    // Returns the length of the entries in the ControlSeries
    byte length() const {
      return sizeof(this->timePoints) / sizeof(this->timePoints[0]);
    }


    // Returns the length of the entries in the ControlSeries
    bool isTpAtIndexActive(byte index) const {
      return index < this->activeTimepoints;
    }


  private:
    // Private variables (accessible only within the class)
    byte activeTimepoints = 4;
    Timepoint timePoints[MAXTIMEPOINTS];
    bool isLooping;

};

ControlSeries NewControlSeriesInstance() {
  return ControlSeries();
}
