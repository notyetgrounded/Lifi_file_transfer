# Lifi_file_transfer
transmit files through light
this project works similar to lifi messenger with the same protocol
but with different sensor https://wiki.dfrobot.com/DFRobot_Ambient_Light_Sensor_SKU_DFR0026  for higher speed ( this sensor has a response time of 15 microsec)
and a different microcontroller that is esp32 for higher analog read speed(40 microsec).
in this project we are using transmitter esp spiffs storage for storing sending file temporarily.
And the recevied file can be printed in the serial monitor or can be stored in recever esp 32 sd card or spiffs storage.

