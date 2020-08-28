#include <EEPROM.h>

void read_parameters_from_eeprom()
{
  max_pos = EEPROM.read(servo_max_eeprom_address);
  min_pos = EEPROM.read(servo_min_eeprom_address);
  timer = EEPROM.read(timer_addr_eeprom_address);
}