# Digital Hygrometer, ESP32-C3 #
<br>
TODO: This is a temporary note.  Example code can be located here:<br> C:\Users\Neal Quackenbush\Dropbox\My Design Projects\Projects\Digital Hygrometer\Design\SW\Archive\epd1in54
<br>

TODO: this is a temporary note for showing us how to make library files: https://rishabhdevyadav.medium.com/create-your-own-arduino-library-h-and-cpp-files-62ab456453e0
<br>

This repository contains the source that shall run on the ESP32-C3 SOM as part of the Digital Hygrometer. <br><br>

The SOM is responsible for main processing tasks, updating the display, and sending emails to the user over WiFi.  There is not a co-processor running in tandem with the SOM.  

### ARCHITECTURE ###
The ESP32-C3 SOM can be bootloaded over the USB-to-serial interface.  In addition to bootloading, said USB-to-serial interface will serve up a CLI that will allow access to basic debugging routines.  The entire application was written and will be maintained in the Arduino IDE. <br>

### TOOL SUITE ###

The source file(s) are to be compiled using Arduino IDE v1.8.8, or newer.   <br>

### COMPILING ###
The digital hygrometer utilizes an `ESP32-C3-MINI-1-N4` WiFi SOM.  For proper compilation, the target board shall be *ESP32C3 Dev Module*.  <br><br>

It is necessary to install the ESP Mail Client library in order for proper compilation.  The following instructions can be followed to install the library.  
Go to `Sketch > Include Library > Manage Libraries` and search for **ESP Mail Client**. Install the ESP Mail Client library which was written by **Mobizt**.

### VERSIONS ###
* V0.0.1 -- This version compiles.  This is really just a skelton of what is needed and was initially put together for testing purposes.  Specific .h and .cpp files were successfully created (for the sensor).    

* v0.0.2 -- This could be a candidate image, but the LED doesn't blink as there is something wrong with the function that toggles the pin.  

* v0.0.3 -- Candidate test image.  The LED should toggle.  The SW string has been updated to reflect this version.

* v0.0.4 -- Issues related to interrupt routines have been addressed.

* v0.0.5 -- A feature to read an analog value has been added in.  This version has gotten a good start on adding in the console version.  

* v0.0.6 -- Many features were added.  Features included, but are not necessarily limited to: printing a string at the bottom of the display; reading temperature; reading humidity; selecting the sensor; fsm state enumeration; sw string updated; general cleanup; etc. 

* v0.0.7 -- Timer routines have been updated for the ESP 3.0x API.  The sending of emails has been completely revamped.  The console routine has been implemented by way of separate h and cpp files.  This version compiles, but there is a lot of testing/work that needs to occur.  

* v0.0.8 --  Code compiles.  Much EEPROM-related information has been added.  A good amount of network stuff has been added.  There is now an application header and source file (kind of like a functions ino file).

* v0.0.9 --  Code compiles and seems to run okay on B02 HW.  The LED is blinking and the console can be accessed.  

*v0.1.0 -- Code compiles and I2C functions seem to be working. Will need to update the routine for how temp data is grabbed (grab while making the RH measurement).  Choosing a sensor is working well, too.

*v0.1.1 -- Code compiles.  The temperature and humidity values can be read reliably.  