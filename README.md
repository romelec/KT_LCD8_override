# KT_LCD8_override

Project to override some values on a KT motor controller and LCD8 display, this might work on other displays but the CRC calculation is different.

Planned features:
- send the motor temperature to the display
- lower motor power when the temperature is hot
- deactivate regen when the batery is full

Hardware: Arduino 32u4 5V 16MHz (clone)

Uses an hardware serial port and a software one

Wiring:

Controler                   Arduino     LCD


--- red ----------------------------------------- red -----
             |-MIC5283-5.0YME-VOUT---RAW (or use 5v from the controller)
--- blue ---------------------------------------- blue ----
             |-1k----1k----GND
                   |-----------------A0
--- black ---------------------------GND--------- black ---
--- green ---------------------------RXI  TXO---- green ---
--- yelow ---------------------------D3   D2----- yelow ---

Resources:
- https://endless-sphere.com/forums/viewtopic.php?t=73475
- https://github.com/stancecoke/BMSBattery_S_controllers_firmware/tree/Master/documentation
