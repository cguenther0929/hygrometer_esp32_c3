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

* v0.2.0 -- Code compiles.  Expanding on the capabilities of preferences routines, we can now store and retrieve floats.

* v0.2.1 -- Display post and refresh routines are working well.  Much focus is being put on developing the final application.  

* v0.2.2 -- Network parameters can be stored and recalled from NVM.  Emails are sent using network parameters that are recalled from NVM.  Code was compiled and tested.

* v0.2.3 -- Temp offset has been implemented and is working.  Code was compiled and tested.

* v0.2.4 -- The email message is now comprised of sensor data.  The bottom row that is printed on the display will print the percent sign after the battery percentage value.  The button press algorithm for long and short presses seems to be working fine.      

* v0.2.5 -- The deep sleep routine is working pretty well.  Need to better utilize the boot counter.  

* v0.2.6 -- The deep sleeping routine was improved.  The display is updated and emails are properly sent after allowing for an appropriate amount of sleeps.  Still need to get the calibration routine up and going, but this is very close to a first release.  

* v0.2.7 -- Most to all of the battery fuel gauge routines have been implemented.  Display power is now toggled on/off.  This version has not been fully tested.  In fact, this version was released before the B03 HW was even built up.  

* v0.2.8 -- The health LED is now pulsed in the startup routine.  There is now a delay after powering on the display.  When removing power from the display, it seemed the entire display wanted to be refreshed, therefore, a full refresh is issued instead of just updating a couple lines during the **update the display** routine.  Fixed a console bug that was related to viewing the charging bit.  There are now provisions to see the flags of the fuel gauge. Fixed the bug where we could get stuck in an infinite email sending loop if the email fails to send.  

* v0.2.9 -- Much cleanup has occurred, but there is plenty left to do.  Emails now include the SOC of the battery (this value is no longer hardcoded at 75%).  Email sender name and subject line were cleaned up.  Display and email delay times were set to something reasonable.  This is really close to a release candidate.  

* v1.0.0 -- All of the TODOs have been addressed.  Display and email delay times have bene updated.  A bug has been corrected with the logging feature (logging should be off, but was showing information related to GPIO reads). The SW version string has been updated to match this tag.    

* v1.0.1 -- The battery capacity can now be dynamically changed via the console interface.   

* v1.0.2 -- The debugging level in the email sending routine has been modified to zero to define no debugging (the value was one, which provided basic-level debugging).  The impetus behind this was to conserve more battery power.  The SW version string has been updated to match this tag.