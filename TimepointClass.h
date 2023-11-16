class Timepoint {
  public:
    // Constructor (initialize the class with default values)
    Timepoint() {
      this->time = 10;
      for (int i = 0; i < VALVES; i++) {
        this->valves[i] = false;
      }
      this->timeUnit = TimeUnitEnum::SECOND;
    };

    // Destructor (clean up resources)
    //~Timepoint();


    // Public methods (functions)

    // Meant to be called in ControlSeries class if part of that.
    void SetTime(int newTime) {
      this->time = newTime;
    }


    // Meant to be called in ControlSeries class if part of that.
    void ToggleTimeUnit() {
      if (this->timeUnit == TimeUnitEnum::SECOND) {
        this->timeUnit = TimeUnitEnum::MINUTE;
      } else if (this->timeUnit == TimeUnitEnum::MINUTE) {
        this->timeUnit = TimeUnitEnum::HOUR;
      } else if (this->timeUnit == TimeUnitEnum::HOUR) {
        this->timeUnit = TimeUnitEnum::SECOND;
      }
#if LOGGING
      Serial.println("TimeUnit Toggled");
      //Serial.println("New unit: " + GetTimeUnitEnumAbbrString(this->timeUnit));
#endif
    }


    // Needs to be called in ControlSeries class if part of that. Otherwise values wont be changed except whole Timepoint class instance is overwritten.
    void ToggleValve(int valveIndex) {
      // check that valeIndex is valid
      if (valveIndex >= VALVES) {
#if LOGGING
        Serial.println("Timepoint: Invalid operation. Set valve index exceeds valve array length.");
#endif
        return;
      }

      // Cache the current state of the valve. Needed to be able to have no valves toggled at the end. 
      bool cachedState = this->valves[valveIndex];

      // delete all current entries. This makes sure that only one valve can be on at any time.
      for (int i = 0; i < VALVES; i++) {
        this->valves[i] = false;
      }

      // set the index to the opposite value (toggle) of the cached state.
      this->valves[valveIndex] = !cachedState; //!this->valves[valveIndex];
#if LOGGING
      Serial.println("Valve Toggled");
#endif
    }


    int GetTime() {
      return this->time;
    }


    unsigned long GetTimeInMillis() {
      return GetMillisForSetTime(this->timeUnit, this->time);
    }


    TimeUnitEnum GetTimeUnit() {
      return this->timeUnit;
    }


    String GetTimeUnitString() {
      return GetTimeUnitEnumAbbrString(this->timeUnit, true);
    }


    // Returns the state (boolean) of the valve with given index
    bool GetValveState(byte valveIndex) {
      if (valveIndex >= 0 && valveIndex < VALVES) {
        return this->valves[valveIndex];
      } else {
#if LOGGING
        Serial.println("TimepointClass.h: Invalid index in GetValveState.");
#endif
        return false;
      }
    }


    // A method to create a loggable string of the content in this timepoint for debugging purposes.
    String toString() const {
      String result = "Timepoint: ";
      result += "Time: " + String(this->time);
      result += " " + GetTimeUnitEnumString(timeUnit) + " ";
      result += "ValveStates: ";
      for (int i = 0; i < VALVES; i++) {
        result += "V" + String(i + 1) + " = " + String(valves[i]) + ", ";
      }
      return result;
    }


  private:
    // Private variables (accessible only within the class)
    int time;
    TimeUnitEnum timeUnit;
    bool valves[VALVES];
};
