<h1>ESP32-C3 based Light sensor</h1>

<h2>Description</h2>
This tiny module is designed around Espressief's ESP32-C3 System On a Chip. Purpose of this module is simple, measure the ambient or incident
light and communicate the reading through the chip's built-in WiFi interface. It uses AMS OSRAM TSL2591 sensor to measure the incident light.

Primary challenge has been to design an RF (Radio Frequency) optimised PCB (Printed Circuit Board) to accommodate this electronics. This designed is optimised for WiFi 2.4GHz and features a
chip antenna (Ignion's NN01-102) to minimize the board size (35mm x 20mm).

The PCB has been designed as per the hardware design guidelines from Espressief with one noticeable deviation on the thickness of the PCB (1.6mm instead of the recommended ~0.8mm)

RF part of the board has been tested with a VNA (Vector Network Analyser) and has been optimized with the help of a matching Pi-network. 
Antenna traces are also designed based upon the datasheet specification and recommended trace pattern.

<h2>The Module</h2>
End result of the PCB is a 4 layer design and manufactured with the help of JLC PCB (https://jlcpcb.com) which according to their documentation has a 
dielectric constant of 4.5 for FR-4 type material. 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Front.png?raw=true)

The light sensor is mounted on the back side of the board as depicted below -

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Back.png?raw=true)

Here is a size reference for the end module - 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Board_Euro_Reference.png?raw=true)

<h2>Enclosure</h2>
I went ahead and also designed a 3D model for the housing of this module for application - 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/3D_Enclosure.png?raw=true)

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/3D_SmartT.png?raw=true)

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Bond_in_enclosure.png?raw=true)

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/SensorSlot.png?raw=true)

<h2>Programming</h2>
Add your own firmware and code to make use of the built-in sensor and indicator. This module can be programmed using Arduino or ESP-IDF IDE. This module has a 
close resemblance with LOLIN C3 Mini or Adafruit C3 QTPy, hence those core board definitions are compatible with this module.

<h2>Design and Architecture</h2>
Below is the System Context for this module highlighting 
the interfaces and interactions with this module. 

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Bond_Documentation-SystemContext.drawio.png?raw=true)

Also a blown up building block of functionality within.

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Bond_Documentation-BlockDiagram.drawio.png?raw=true)

<h2>Applications</h2>
One implementation that I personally use this module is for the purpose of controlling the lights on my balcony and eventually also festive lights during Christmas.
Following a possible application diagram -

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/Bond_Documentation-Application.drawio.png?raw=true)

<h2>Open Source Hardware</h2>
This board is an OSHWA approved design: <a href="https://certification.oshwa.org/nl000015.html">NL000015</a>

![alt text](https://github.com/tanmoydutta/lightsensorwithwifi/blob/main/images/oshw_license-2.png?raw=true)
