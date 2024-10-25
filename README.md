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
The digital hygrometer utilizes an `ESP32-C3-MINI-1-N4` WiFi SOM.  For proper compilation, the target board shall be *ESP32C3 Dev Module*.  

### VERSIONS ###
* V0.0.1 -- This version compiles.  This is really just a skelton of what is needed and was initially put together for testing purposes.  Specific .h and .cpp files were successfully created (for the sensor).    

* v0.0.2 -- This could be a candidate image, but the LED doesn't blink as there is something wrong with the function that toggles the pin.  

* v0.0.3 -- Candidate test image.  The LED should toggle.  The SW string has been updated to reflect this version