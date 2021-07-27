# thermometer-arduino-type-k
An adjustable Arduino thermometer that I was "commisioned" to make

<b>Features</b>
<ul>
  <li>A type K thermocouple combined with a MAX6675 module</li>
  <li>A seven segment display for viewing the temperature and settings</li>
  <li>An adjustable temperature limit for controlling loads (in my case a red/green LED)</li>
  <li>EEPROM for saving temperature values even when Arduino is off</li>
</ul>

<b>Schematics</b>
There are schematics enclosed in this repository. In my case, I used an LED as my load but feel free to add a relay or MOSFET to control something more high power.

<b>Usage</b>
Make sure the thermocouple is connected properly, otherwise the Arduino will display random values. To set a temperature limit, press both buttons at the same time, wait 5 seconds until the display shows the default value of 100. Then, adjust the temperature in increments of five (min. 0 degrees C, max 995 degrees C). To save the value, press both buttons at the same time and wait 5 seconds. The value is stored into the EEPROM of the Arduino, so there's no need to set it every time you power the circuit.
