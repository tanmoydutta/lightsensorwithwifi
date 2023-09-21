# lightsensorwithwifi
<h1>ESP32-C3 based Light sensor</h1>

This tiny module is designed around Espressief's ESP32-C3 System On a Chip. Purpose of this module is simple, measure the ambient or incident
light and communicate the reading through the chip's built-in WiFi interface. It uses AMS OSRAM TSL2591 sensor to measure the incident light.

Primary challenge has been to design an RF (Radio Frequency) optimised PCB (Printed Circuit Board) to accommodate this electronics. This designed is optimised for WiFi 2.4GHz and features a
chip antenna (Ignion's NN01-102) to minimize the board size (35mm x 20mm).

The PCB has been designed as per the hardware design guidelines from Espressief with one noticeable deviation on the thickness of the PCB (1.6mm instead of the recommended ~0.8mm)

RF part of the board has been tested with a VNA (Vector Network Analyser) and has been optimized with the help of a matching Pi-network. 

End result of the PCB is a 4 layer design and manufactured with the help of JLC PCB (https://jlcpcb.com) which according to their documentation has a 
dielectric constant of 4.5 for FR-4 type material.

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Front.png?raw=true)

The light sensor is mounted on the back side of the board as depicted below -

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Back.png?raw=true)

Here is a size reference for the end module - 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Euro_Reference.png?raw=true)

I went ahead and also designed a 3D model for the housing of this module for application - 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/3D_Enclosure.png?raw=true)

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/3D_SmartT.png?raw=true)

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Bond_in_enclosure.png?raw=true)

Add your own firmware and code to make use of the built-in sensor and indicator. Below is the System Context for this module highlighting 
the interfaces and interactions with this module. 

Also a blown up building block of functionality within.

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/oshw_license-2.png?raw=true)
