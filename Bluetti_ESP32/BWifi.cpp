#include "BluettiConfig.h"
#include "BWifi.h"
#include "BTooth.h"
#include "index.h"  //Web page header file
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip
#include <AsyncTCP.h> // https://github.com/me-no-dev/AsyncTCP/archive/master.zip
#include <ESPmDNS.h>
#include <AsyncElegantOTA.h> // https://github.com/ayushsharma82/AsyncElegantOTA/archive/master.zip


/*
 * --- new methods implemented on 17.08.2023 ----
 */
#include <ArduinoJson.h>
#include <utils.h>

SemaphoreHandle_t jsonMutex = xSemaphoreCreateMutex();// Mutex for synchronizing JSON access
DynamicJsonDocument jsonData(1024);

bool is_int(String val){
  // Check if the payload contains only digits
  bool isNumeric = true;
  for (size_t i = 0; i < val.length(); i++) {
    if (!isdigit(val[i])) {
        isNumeric = false;
        break;
    }
  }
  return isNumeric;
}

void update_value(enum field_names field_name, String value){
  //sometimes we get empty values / wrong vales - all the time device_type is empty
  if (map_field_name(field_name) == "device_type" && value.length() < 3){
    Serial.println(F("Error while publishTopic! 'device_type' can't be empty, reboot device)"));
    ESP.restart();
  }
  // saving the key value pair in the jsonData
  xSemaphoreTake(jsonMutex, portMAX_DELAY); // lock
  Serial.printf("Writing %s: %s", map_field_name(field_name).c_str(), value);
  jsonData[map_field_name(field_name)] = value;
  xSemaphoreGive(jsonMutex); // unlock
}
/*
 * --- end new methods ----
 */


/*
 * --- end reused methods from MQTT.cpp files (https://github.com/mariolukas/Bluetti_ESP32_Bridge)
 */

String map_field_name(enum field_names f_name){
   switch(f_name) {
      case DC_OUTPUT_POWER:
        return "dc_output_power";
        break; 
      case AC_OUTPUT_POWER:
        return "ac_output_power";
        break; 
      case DC_OUTPUT_ON:
        return "dc_output_on";
        break; 
      case AC_OUTPUT_ON:
        return "ac_output_on";
        break; 
      case AC_OUTPUT_MODE:
        return "ac_output_mode";
        break; 
      case POWER_GENERATION:
        return "power_generation";
        break;       
      case TOTAL_BATTERY_PERCENT:
        return "total_battery_percent";
        break; 
      case DC_INPUT_POWER:
        return "dc_input_power";
        break;
      case AC_INPUT_POWER:
        return "ac_input_power";
        break;
      case AC_INPUT_VOLTAGE:
        return "ac_input_voltage";
        break;
      case AC_INPUT_FREQUENCY:
        return "ac_input_frequency";
        break;
      case PACK_VOLTAGE:
        return "pack_voltage";
        break;
      case INTERNAL_PACK_VOLTAGE:
        return "internal_pack_voltage";
        break;
      case SERIAL_NUMBER:
        return "serial_number";
        break;
      case ARM_VERSION:
        return "arm_version";
        break;
      case DSP_VERSION:
        return "dsp_version";
        break;
      case DEVICE_TYPE:
        return "device_type";
        break;
      case UPS_MODE:
        return "ups_mode";
        break;
      case AUTO_SLEEP_MODE:
        return "auto_sleep_mode";
        break;
      case GRID_CHARGE_ON:
        return "grid_charge_on";
        break;
      case INTERNAL_AC_VOLTAGE:
        return "internal_ac_voltage";
        break;
      case INTERNAL_AC_FREQUENCY:
        return "internal_ac_frequency";
        break;
      case INTERNAL_CURRENT_ONE:
        return "internal_current_one";
        break;
      case INTERNAL_POWER_ONE:
        return "internal_power_one";
        break;
      case INTERNAL_CURRENT_TWO:
        return "internal_current_two";
        break;
      case INTERNAL_POWER_TWO:
        return "internal_power_two";
        break;
      case INTERNAL_CURRENT_THREE:
        return "internal_current_three";
        break;
      case INTERNAL_POWER_THREE:
        return "internal_power_three";
        break;
      case PACK_NUM_MAX:
        return "pack_max_num";
        break;
      case PACK_NUM:
        return "pack_num";
        break;
      case PACK_BATTERY_PERCENT:
        return "pack_battery_percent";
        break;
      case INTERNAL_DC_INPUT_VOLTAGE:
        return "internal_dc_input_voltage";
        break;
      case INTERNAL_DC_INPUT_POWER:
        return "internal_dc_input_power";
        break;
      case INTERNAL_DC_INPUT_CURRENT:
        return "internal_dc_input_current";
        break;
      case INTERNAL_CELL01_VOLTAGE:
        return "internal_cell01_voltage";    
        break;
      case INTERNAL_CELL02_VOLTAGE:
        return "internal_cell02_voltage";    
        break;
      case INTERNAL_CELL03_VOLTAGE:
        return "internal_cell03_voltage";    
        break;
      case INTERNAL_CELL04_VOLTAGE:
        return "internal_cell04_voltage";    
        break;
      case INTERNAL_CELL05_VOLTAGE:
        return "internal_cell05_voltage";    
        break;
      case INTERNAL_CELL06_VOLTAGE:
        return "internal_cell06_voltage";    
        break;
      case INTERNAL_CELL07_VOLTAGE:
        return "internal_cell07_voltage";    
        break;
      case INTERNAL_CELL08_VOLTAGE:
        return "internal_cell08_voltage";    
        break;
      case INTERNAL_CELL09_VOLTAGE:
        return "internal_cell09_voltage";    
        break;
      case INTERNAL_CELL10_VOLTAGE:
        return "internal_cell10_voltage";    
        break;
      case INTERNAL_CELL11_VOLTAGE:
        return "internal_cell11_voltage";    
        break;
      case INTERNAL_CELL12_VOLTAGE:
        return "internal_cell12_voltage";    
        break;
      case INTERNAL_CELL13_VOLTAGE:
        return "internal_cell13_voltage";    
        break;
      case INTERNAL_CELL14_VOLTAGE:
        return "internal_cell14_voltage";    
        break;
      case INTERNAL_CELL15_VOLTAGE:
        return "internal_cell15_voltage";    
        break;
      case INTERNAL_CELL16_VOLTAGE:
        return "internal_cell16_voltage";    
        break;     
      case LED_MODE:
        return "led_mode";
        break;
      case POWER_OFF:
        return "power_off";
        break;
      case ECO_ON:
        return "eco_on";
        break;
      case ECO_SHUTDOWN:
        return "eco_shutdown";
        break;
      case CHARGING_MODE:
        return "charging_mode";
        break;
      case POWER_LIFTING_ON:
        return "power_lifting_on";
        break;
      case AC_INPUT_POWER_MAX:
        return "ac_input_power_max";
        break;
      case AC_INPUT_CURRENT_MAX:
        return "ac_input_current_max";
        break;
      case AC_OUTPUT_POWER_MAX:
        return "ac_output_power_max";
        break;
      case AC_OUTPUT_CURRENT_MAX:
        return "ac_output_current_max";
        break;
      case BATTERY_MIN_PERCENTAGE:
        return "battery_min_percentage";
        break;
      case AC_CHARGE_MAX_PERCENTAGE:
        return "ac_charge_max_percentage";
        break;
      default:
        #ifdef DEBUG
          Serial.println(F("Info 'map_field_name' found unknown field!"));
        #endif
        return "unknown";
        break;
   }
  
}

//There is no reflection to do string to enum
//There are a couple of ways to work aroung it... but basically are just "case" statements
//Wapped them in a fuction
String map_command_value(String command_name, String value){
  String toRet = value;
  value.toUpperCase();
  command_name.toUpperCase(); //force case indipendence

  //on / off commands
  if(command_name == "POWER_OFF" || command_name == "AC_OUTPUT_ON" || command_name == "DC_OUTPUT_ON" || command_name == "ECO_ON" || command_name == "POWER_LIFTING_ON") {
    if (value == "ON") {
      toRet = "1";
    }
    if (value == "OFF") {
      toRet = "0";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "LED_MODE"){
    if (value == "LED_LOW") {
      toRet = "1";
    }
    if (value == "LED_HIGH") {
      toRet = "2";
    }
    if (value == "LED_SOS") {
      toRet = "3";
    }
    if (value == "LED_OFF") {
      toRet = "4";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "ECO_SHUTDOWN"){
    if (value == "ONE_HOUR") {
      toRet = "1";
    }
    if (value == "TWO_HOURS") {
      toRet = "2";
    }
    if (value == "THREE_HOURS") {
      toRet = "3";
    }
    if (value == "FOUR_HOURS") {
      toRet = "4";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "CHARGING_MODE"){
    if (value == "STANDARD") {
      toRet = "0";
    }
    if (value == "SILENT") {
      toRet = "1";
    }
    if (value == "TURBO") {
      toRet = "2";
    }
  }


  return toRet;
}
/*
 * --- end reused methods
 */


int publishErrorCount = 0;

AsyncWebServer server(80);
AsyncEventSource events("/events");

unsigned long lastTimeWebUpdate = 0;  

String lastMsg = ""; 

bool msgViewerDetails = false;
bool shouldSaveConfig = false;

char bluetti_device_id[40] = "e.g. ACXXXYYYYYYYY";

void saveConfigCallback () {
  shouldSaveConfig = true;
}

ESPBluettiSettings wifiConfig;

ESPBluettiSettings get_esp32_bluetti_settings(){
    return wifiConfig;
    return wifiConfig;
}

void eeprom_read(){
  Serial.println(F("Loading Values from EEPROM"));
  EEPROM.begin(512);
  EEPROM.get(0, wifiConfig);
  EEPROM.end();
}

void eeprom_saveconfig(){
  Serial.println(F("Saving Values to EEPROM"));
  EEPROM.begin(512);
  EEPROM.put(0, wifiConfig);
  EEPROM.commit();
  EEPROM.end();
} 

void setWiFiPowerSavingMode(){
  //esp_wifi_set_ps(WIFI_PS_MAX_MODEM); // maximum power saving, does not make sense here
  //esp_wifi_set_ps(WIFI_PS_NONE); // will cause kernel panic and reboot on my ESP32 (AlexBurghardt)
  esp_wifi_set_ps(WIFI_PS_MIN_MODEM); // default
}

void initBWifi(bool resetWifi){
  // init mutex for json writes
  assert(jsonMutex);

  eeprom_read();

  WiFiManagerParameter custom_ota_username("ota_username", "OTA Username", "", 40);
  WiFiManagerParameter custom_ota_password("ota_password", "OTA Password", "", 40, "type=password");
  WiFiManagerParameter custom_bluetti_device("bluetti", "Bluetti Bluetooth ID", bluetti_device_id, 40);

  WiFiManager wifiManager;

  if (resetWifi){
    wifiManager.resetSettings();
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
    eeprom_saveconfig();
  } else if (wifiConfig.salt != EEPROM_SALT) {
    Serial.println("Invalid settings in EEPROM, trying with defaults");
    ESPBluettiSettings defaults;
    wifiConfig = defaults;
  } else {
    wifiManager.setConfigPortalTimeout(300);
  }

  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_ota_username);
  wifiManager.addParameter(&custom_ota_password);
  wifiManager.addParameter(&custom_bluetti_device);
  
  wifiManager.setAPCallback([&](WiFiManager* wifiManager) {
		Serial.printf("Entered config mode:ip=%s, ssid='%s'\n", 
                        WiFi.softAPIP().toString().c_str(), 
                        wifiManager->getConfigPortalSSID().c_str());
                        
	});
  
  if (!wifiManager.autoConnect("Bluetti_ESP32")) {
    ESP.restart();
  }

  if (shouldSaveConfig) {
     strlcpy(wifiConfig.ota_username, custom_ota_username.getValue(), 40);
     strlcpy(wifiConfig.ota_password, custom_ota_password.getValue(), 40);
     strlcpy(wifiConfig.bluetti_device_id, custom_bluetti_device.getValue(), 40);
     eeprom_saveconfig();
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    // display will have blinking wifi signal until connected.
    delay(500);
    Serial.print(".");
  }
  
  WiFi.setAutoReconnect(true);

  Serial.println(F(""));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  
  if (MDNS.begin(DEVICE_NAME)) {
    Serial.println(F("MDNS responder started"));
  }

  //setup web server handling
  #if MSG_VIEWER_DETAILS
      msgViewerDetails = true;
      Serial.println(F("webserver BT variable logging enabled..."));
    #else
      msgViewerDetails = false;
      Serial.println(F("webserver BT variable logging disabled..."));
  #endif

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processorWebsiteUpdates);
  });
  server.on("/rebootDevice", [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "reboot in 2sec");
      delay(2000);
      ESP.restart();
  });
  server.on("/resetConfig", [](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", "reset Wifi and reboot in 2sec");
      delay(2000);
      initBWifi(true);
  });

  /*
   * --- added requests on 17.08.2023
   */
  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
    String jsonStr;
    serializeJson(jsonData, jsonStr);
    Serial.println(F("application/json: Returning the current jsonData."));
    request->send(200, "application/json", jsonStr);
  });

  server.on("/setData/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasArg("command")) {
      request->send(400, "text/plain", "Missing 'command' Parameter!");
      return;
    }
    if (!request->hasArg("value")) {
      request->send(400, "text/plain", "Missing 'value' Parameter!");
      return;
    }
    String command_name = request->arg("command");
    String payload = request->arg("value");

    //force case indipendence    
    command_name.toLowerCase();
    payload.toUpperCase();

    bt_command_t command;
    command.prefix = 0x01;
    command.field_update_cmd = 0x06;

    Serial.print(" Raw Payload - Updated: ");
    Serial.println(payload);

    bool foundMatch = false;
    // matching the command with the possible commands enum
    for (int i = 0; i < (sizeof(bluetti_device_command) / sizeof(device_field_data_t)); i++){
        if (map_field_name(bluetti_device_command[i].f_name) == command_name){
          command.page = bluetti_device_command[i].f_page;
          command.offset = bluetti_device_command[i].f_offset;
          payload = map_command_value(command_name, payload);
          foundMatch = true;
          break;
        }
    }
    // verfiy command
    if (!foundMatch){
      request->send(400, "text/plain", "Unknown Command!");
      return;
    }
    Serial.print(" Payload - Updated: ");
    Serial.println(payload);

    // verify payload value
    if (!is_int(payload)) {
      request->send(400, "text/plain", "Invalid Payload Value!");
      return;
    }

    command.len = swap_bytes(payload.toInt());
    command.check_sum = modbus_crc((uint8_t*)&command,6);
    
    Serial.println(F("setting the value via BT."));
    sendBTCommand(command);
    
    request->send(200, "text/plain", "ok");
  });
  /*
   * --- end added requests
   */

  //setup web server events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello my friend, I'm just your data feed!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  if (!wifiConfig.ota_username) {
    AsyncElegantOTA.begin(&server);
  } else {
    AsyncElegantOTA.begin(&server, wifiConfig.ota_username, wifiConfig.ota_password);
  }

  server.begin();
  Serial.println(F("HTTP server started"));

}

void handleWebserver() {
  //Serial.println(F("DEBUG handleWebserver"));
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi is disconnected, try to reconnect..."));
    WiFi.disconnect();
    WiFi.reconnect();
    AddtoMsgView(String(millis()) + ": WLAN ERROR! try to reconnect");
    delay(1000);
  }

  if ((millis() - lastTimeWebUpdate) > MSG_VIEWER_REFRESH_CYCLE*1000) {

    // Send Events to the Web Server with current data
    events.send("ping",NULL,millis());
    events.send(String(millis()).c_str(),"runtime",millis());
    events.send(String(WiFi.RSSI()).c_str(),"rssi",millis());
    events.send(String(isBTconnected()).c_str(),"bt_connected",millis());
    events.send(String(getLastBTMessageTime()).c_str(),"bt_last_msg_time",millis());
    if(msgViewerDetails){
      events.send(lastMsg.c_str(),"last_msg",millis());
    } 
    
    lastTimeWebUpdate = millis();
  }
}

String processorWebsiteUpdates(const String& var){
  // the values for MQTT were removed for this version (17.082023)
  if(var == "IP"){
    return String(WiFi.localIP().toString());
  }
  else if(var == "RSSI"){
    return String(WiFi.RSSI());
  }
  else if(var == "SSID"){
    return String(WiFi.SSID());
  }
  else if(var == "MAC"){
    return String(WiFi.macAddress());
  }
  else if(var == "RUNTIME"){
    return String(millis());
  }
  else if(var == "DEVICE_ID"){
    char msg[40];
    strlcpy(msg, wifiConfig.bluetti_device_id, 40);
    return msg;
  }
  else if(var == "BT_CONNECTED"){
    return String(isBTconnected());
  }
  else if(var == "LAST_BT_MSG_TIME"){
    return String(getLastBTMessageTime());
  }
  else if(var == "BT_ERROR"){
    return String(publishErrorCount);
  }
  else if(var == "LAST_MSG"){
    if (msgViewerDetails){
      return String("...waiting for data...");
    }
    else{
      return String("...disabled...");
    }
  }
  else {
    return "Not Available";
  }
}

void AddtoMsgView(String data){
  
  String tempMsg = "";
  
  int firstPos = lastMsg.indexOf("</p>");
  int nextPos = firstPos;
  int numEntry = 0;
  while(nextPos > 0){
    nextPos = lastMsg.indexOf("</p>",nextPos+4);
    if (nextPos > 0){
      numEntry++;
    }
  }

  if (numEntry > MSG_VIEWER_ENTRY_COUNT-2){
    tempMsg = lastMsg.substring(firstPos+4);
    lastMsg = tempMsg + "<p>" + data + "</p>";
  }
  else{
    lastMsg = lastMsg + "<p>" + data + "</p>";
  }
}
