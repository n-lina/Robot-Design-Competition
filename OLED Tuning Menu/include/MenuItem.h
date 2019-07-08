/*
  MenuItem.h - Library for creating menu items to interface with the TINAH board EEPROM menu system.
*/

#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"

class MenuItem
{
  public:
    MenuItem(const char* item, unsigned int* address );
    const char* getName();
    int getValue();
    void setValue( int value );
	int getEepNum();
  private:
    const char* itemName;
    unsigned int* EEPROMAddress;
};

#endif