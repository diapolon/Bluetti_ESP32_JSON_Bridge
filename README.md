## About

This is a Fork of [mariolukas/Bluetti_ESP32_Bridge](https://github.com/mariolukas/Bluetti_ESP32_Bridge) which itself is based on https://github.com/warhammerkid/bluetti_mqtt. The Bridge can be used to read and write values via Bluetooth to the Bluetti Powerstations via JSON http api instead of the complexer mqtt. In addition to that I removed the display support because I wanted to make it even simlper.

The original code is tested on an AC300. Other Powerstations should also work but are untested.

More information can be found in the issues and documentation of the mariolukas/Bluetti_ESP32_Bridge repo aswell.

## Features

* easy configuration with WiFi manager
* support for BLUETTI power stations (change `BLUETTI_TYPE` for that)
  * AC300 (tested)
  * AC200 (untested)
  * EB3A (untested)
  * EP500 (untested)
  * EP500P (untested)
  * EP600 (untested)
* supported BLUETTI functions
  * setting value / commands
    * ac output on/off
    * dc output on/off
  * reading values / states
    * ac input power
    * dc input power
    * ac output power
    * dc output power
    * dsp firmware version
    * arm firmware version
    * serial number
    * device type
    * power generation
    * total battery percent

## Getting Started

### Compiling and Flashing to ESP32

#### Arduino IDE

You will need to install a board support package for your ESP32. Additionally the following libraries are needed: 

* https://github.com/tzapu/WiFiManager
* https://github.com/ayushsharma82/AsyncElegantOTA
* https://github.com/me-no-dev/ESPAsyncWebServer
* https://github.com/me-no-dev/AsyncTCP/archive

Change the partition scheme with Tools -> Partition Scheme to

* Minimal SPIFFS (1.9 MB App with OTA/ 190KB SPIFFS)

![Wifi Manager start menu](doc/images/partition.png)

This setting is required because the Bluetooth stack already uses a lot of the ESP32 memory.

Optional: Do changes in config.h file. The device can be set by changing 'BLUETTI_TYPE'.

Finally upload the Sketch to your ESP32.

*INFO*: Until now only BLUETTI_AC300, BLUETTI_EP500P was tested. If you own one of the supported devices please let me know if it works.

#### PlatformIO

Compiling
```
$ pio run
```

Flashing Factory Image
```
$ esptool.py write_flash 0x0 build/Bluetti_ESP32_Bridge.factory.bin
```

Updating only App (don't delete settings)
```
# Write Partition A
$ esptool.py write_flash 0x10000 build/Bluetti_ESP32_Bridge.ota.bin
...
# Write Partition B
$ esptool.py write_flash 0x1F0000 build/Bluetti_ESP32_Bridge.ota.bin
```

The configuration interface also offers OTA updates. You can flash also `build/Bluetti_ESP32_Bridge.ota.bin` there. If you already configured your device you can use `http://<ip_address/update` to update your firmware (no-reconfiguration needed)

### Usage

Just connect the ESP32 to a power supply or the power station itself. Connect your mobile phone or computer
to the Wifi mangaer of the ESP32 firmware, it should show up as "ESP32_Bluetti".

After the connection to the hotspot is established a wifi manager popup will appear. Otherwise
open 192.168.4.1 in your local webbrowser.

Select "Configure WiFi"

![Wifi Manager start menu](doc/images/wifi_manager.png)

Configure your WiFi and enter the Bluetooth ID of your
Bluetti Device. Optionally you can specify username and password to protect the web OTA interface.
You can use a mobile phone and/or the Bluetti APP for finding the correct Bluetooth ID of your device.

![Wifi Manager start menu](doc/images/wifi_setup.png)

Save the settings. The ESP32 starts sending messages to your MQTT server.


### JSON API

#### Commands / Setting Values

* `GET <device-ip>/setData?command=<your-command>&value=<your-value>
  * `ac_output_on` is either `on` or `off` 
  * `dc_output_on` is either `on` or `off`

For some devices there are more values that can be set
see `bluetti_device_command[]` of you device (f.e bluetti_device_command in [ Device_AC200M.h](./Bluetti_ESP32/Device_AC200M.h)). Be cautios with the values you send, as they might not be valid values.

#### State / Getting Values

* `GET <device-ip>/getData` returns a json containing the following values
  * ac_output_on
  * dc_output_on
  * dc_input_power
  * ac_input_power
  * ac_output_power
  * dc_output_power
  * serial
  * dsp_version
  * arm_version
  * power_generation
  * total_battery_percent

## Changelog

* removed `MQTT.h`, `MQTT.cpp`, but used method `map_field_name` and `map_command_value` from them -> BWifi.cpp

* removed mqtt values and handling from everywhere (including Wifimanager Parameters)
* removed all Display compatibility

* added server.on... for  `/getData` and `/setData` (including jsonData value)
* changed event handler for Bluetooth reads from `MQTT.cpp - void data_payload_field` to `BWifi.cpp - void update_values`
* renamed serial prints `BT/MQTT` -> `BT` on multiple places
* removed `/switchLogging` url

* changed index.h html Headings and the MQTT Block -> JSON, removed mqtt values
* removed unecessary libs (due to feature removal)


## Disclaimer

The code within this repository comes with no guarantee, use it on your own risk.

Don't touch these firmwares if you don't know how to put the device in the programming mode if something goes wrong.
As per the GPL v3 license, I assume no liability for any damage to you or any other person or equipment.
