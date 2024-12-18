#ifndef BWIFI_H
#define BWIFI_H
#include "Arduino.h"
#include "config.h"
#include "DeviceType.h"

typedef struct{
  int  salt = EEPROM_SALT;
  char bluetti_device_id[40] = "Bluetti Blutetooth Id";
  char ota_username[40] = "";
  char ota_password[40] = "";
} ESPBluettiSettings;


extern ESPBluettiSettings get_esp32_bluetti_settings();
extern void initBWifi(bool resetWifi);
extern void handleWebserver();
String processorWebsiteUpdates(const String& var);
extern void AddtoMsgView(String data);
extern void update_value(enum field_names field_name, String value);
String map_field_name(enum field_names f_name);

#endif
