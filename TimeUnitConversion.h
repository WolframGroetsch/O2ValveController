// constant conversion factors from millis to units
const unsigned long secondsToMillis = 1000;
const unsigned long minutesToMillis = 60000;
const unsigned long hoursToMillis = 3600000;


// Returns the above defined conversion factors into milliseconds based on the given time unit.
unsigned long GetConversionFactor(TimeUnitEnum timeUnit) {
  switch (timeUnit) {
    case TimeUnitEnum::SECOND:
      return secondsToMillis;

    case TimeUnitEnum::MINUTE:
      return minutesToMillis;

    case TimeUnitEnum::HOUR:
      return hoursToMillis;

    default:
#if LOGGING
      Serial.println("TimeEnum: Conversion failed, invalid unit.");
#endif
      return 0;
  }
}


// Returns a value in miliseconds based on the given time and unit
unsigned long GetMillisForSetTime(TimeUnitEnum timeUnit, int timeValue) {
  return GetConversionFactor(timeUnit) * timeValue;
}


// Returns a string with the set time unit for text logging purposes.
String GetTimeUnitEnumString(TimeUnitEnum timeUnit) {
  switch (timeUnit) {
    case TimeUnitEnum::SECOND:
      return "Seconds";

    case TimeUnitEnum::MINUTE:
      return "Minutes";

    case TimeUnitEnum::HOUR:
      return "Hours";

    default:
#if LOGGING
      Serial.println("TimeEnum: String generation failed, invalid unit.");
#endif
      return "INVALID";
  }
}


// Returns an abbreviated string version of the set time unit for displaying purposes.
String GetTimeUnitEnumAbbrString(TimeUnitEnum timeUnit, bool centered) {
  switch (timeUnit) {
    case TimeUnitEnum::SECOND:
      if (centered)
        return " s ";
      else
        return "s";

    case TimeUnitEnum::MINUTE:
      if (centered)
        return "min";
      else
        return "min";

    case TimeUnitEnum::HOUR:
      if (centered)
        return " h ";
      else
        return "h";

    default:
#if LOGGING
      Serial.println("TimeEnum: String generation failed, invalid unit.");
#endif
      return "x";
  }
}
