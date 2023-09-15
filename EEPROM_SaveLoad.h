#include <EEPROM.h>

#define SLOTSIZE  200


SaveToSlot(byte slot, const ControlSeries& data) {
  int address = slot * SLOTSIZE;
  EEPROM.put(address, data);
#if LOGGING
  Serial.print("Written to slot " + String(slot));
#endif
}


LoadFromSlot(byte slot) {
  int address = slot * SLOTSIZE;
  EEPROM.get(address, cs); // will read the data from the given address and store it in ControlSeriesClass instance cs
#if LOGGING
  Serial.print("Loaded from slot " + String(slot));
#endif
}
