# Digital Hygrometer, ESP32-C3 #

This repository contains the source that shall run on the ESP32-C3 SOM as part of the Digital Hygrometer. <br><br>

The SOM is responsible for main processing tasks, updating the display, and sending emails to the user over WiFi.  There is not a co-processor running in tandem with the SOM.  

### ARCHITECTURE ###
The ESP32-C3 SOM can be bootloaded over the USB-to-serial interface.  In addition to bootloading, said USB-to-serial interface will serve up a CLI that will allow access to basic debugging routines.  The entire application was written and will be maintained in the Arduino IDE. <br>

### TOOL SUITE ###

The source file(s) are to be compiled using Arduino IDE v1.8.8, or newer.   <br>

### COMPILING ###
The digital hygrometer utilizes an `ESP32-C3-MINI-1-N4` WiFi SOM.  For proper compilation, the target board shall be *ESP32C3 Dev Module*.  <br><br>

It is necessary to install the ESP Mail Client library in order for proper compilation.  The following instructions can be followed to install the library.  
Go to `Sketch > Include Library > Manage Libraries` and search for **ESP Mail Client** (not ESP**32**). Install the ESP Mail Client library which was written by **Mobizt**.  The reader shall take caution not to confuse **ESP** Mail Client with ESP**32** Mail Client.  

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

* v0.1.0 -- Code compiles and I2C functions seem to be working. Will need to update the routine for how temp data is grabbed (grab while making the RH measurement).  Choosing a sensor is working well, too.

* v0.1.1 -- Code compiles.  The temperature and humidity values can be read reliably.  

* v0.1.2 -- Code compiles.  Text, lines, and images can all be printed to the display with success.    

* v0.1.3 -- Code compiles.  Analog readings function.  Features have been added to the console.  Some cleanup has occurred.  

* v0.1.4 -- Code compiles.  Processor goes into deep sleep and can be woken up via timeout or button press.  Reads from the IO expander are functional.  Trying to send an email will cause the SOM to reset.  

* v0.1.5 -- Code compiles.  We can send emails, but at the detriment of commenting out some of the display stuff (i.e. LDirInit).  

* v0.1.6 -- Code compiles.  Email sending finally works.  The issue was that the SPI transactions (initialized by the display) weren't being stopped, so the module would freeze on the part of sending emails.  

* v0.1.7 -- Code compiles.  Email sending works and the routines are again separated into their respective library (opposed to email stuff just living in the setup routine).  Although this next one is just related to HW, the module no longer resets when powered by battery now that there is a 100uF tantalum cap near the MCU.

* v0.1.8 -- Code compiles and seems to run fine.  Release cleanup has commenced, and the code is starting to take the form of the final application.  The display splash screen has moved, and it appears the device is properly cycling through the states. Many parameters have moved into app.h

* v0.1.9 -- Code compiles.  Tagging since reading a string back from the preferences class is finally working. It is working if the array is passed by reference.  It does not seem to work if the array is passed by a pointer.  Much cleanup is needed.  