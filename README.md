# RAK811-GettingStarted-US915-TTN
Using the RAK811 with the AT Command firmware can be a quick and easy way to connect to The Things Network in the US915 band.
Every RAK811 module has a Device EUI built-in.
## Initial Setup (only done once)
The commands in this section need only to be done once. The parameters will be stored in EEPROM.
* Ensure that the frequency band is `US915`. If not US915 then the module must be flashed with the correct firmware.
```
at+band
```
Response: `OKUS915`
* Set the mode to LoraWAN
```
at+mode=0
```
Response: `OK`
* Get the Device EUI
```
at+get_config=dev_eui
```
Response: `OK373934355A371111` Copy this number (without the `OK`). Used to create the Device on The Things Network.
* Restrict the channels to the TTN sub band 1
```
at+set_config=ch_mask:0,ff00&ch_mask:1,0000&ch_mask:2,0000&ch_mask:3,0000&ch_mask:4,0000
```
Response: `OK`
* Set the App EUI and App Key
The APP EUI and App Key should be copied from The Things Network Device page.
```
at+set_config=app_eui:70B3D57ED001000B&app_key:EC7E7F139DD6E6BB5BAB0000A25F27B0
```
Response: `OK`
## Join and Send messages
Once the module has been setup the arduino code only needs to Join the network and send messages.
* Join the network using OTAA
```
at+join=otaa
```
Response: First `OK` and then within 10 seconds of successful join: `at+recv=3,0,0`
* Send a message
The following will send an unconfirmed message (should almost always be unconfirmed = 0) payload "01670110" on portf 1
```
at+send=0,1,01670110
```
Response (if successful): First `OK` and then `at+recv=2,0,0`