#include "Arduino.h"
#include "MenuItem.h"
#include <avr/EEPROM.h>

MenuItem::MenuItem( const char* item, unsigned int* address ) {
  itemName = item;
  EEPROMAddress = address;
}

const char* MenuItem::getName() {
  return itemName;
}

int MenuItem::getValue() {
  return eeprom_read_word(EEPROMAddress);
}
void MenuItem::setValue( int value ) {
  eeprom_write_word(EEPROMAddress, value);
}

int MenuItem::getEepNum(){
	return (int)EEPROMAddress;
}