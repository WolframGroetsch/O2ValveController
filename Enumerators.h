// Enumerators are placed in a separate file so Arduin IDE compiler is forced to know them beforehand. 
// Otherwise functions are placed at the top and an error stating that the type is unknown is thrown. 

// Enumerator for time
enum class TimeUnitEnum : byte {SECOND, MINUTE, HOUR};

// Enumerator for the display state
enum class DisplayState : byte {MAIN_EDIT, MAIN_RUN, MAIN_SAVELOAD, TIMEEDIT};
