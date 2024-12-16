#include "console.h"


/**
 * Instantiate classes
 */
LAN     console_lan;
APP     app_function;
I2C     i2c_function;
NVM     nvm_function;


extern String SW_VER_STRING;

/**
 * Set to true to 
 * enable logging
 */
#define ENABLE_LOGGING                true


void CONSOLE::init(void) 
{
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from EEPROM
    this -> user_selection = 99;

}


 void CONSOLE::flush_serial_input_buffer( void )
 {
    while (Serial.available() > 0) {
        Serial.read();             //gets one byte from serial buffer
    }
 }


/**
 * @brief Get Number Input From User
 */
uint8_t CONSOLE::get_user_uint8t_value ( void )  
{
    String user_input = "";
    uint16_t return_number = 0;
    
    while (Serial.available() <= 0);  //Pause until we start receiving data
    
    
    while (Serial.available() > 0) {
        char c = Serial.read();             //gets one byte from serial buffer
        user_input += c;                    
        delay(1);                           //Allow buffer to fill with next character
    }

    return_number = user_input.toInt();

    if(return_number > 255) 
    {
        return_number = 255;
    }

    return (return_number);
}

void CONSOLE:: insert_line_feeds( uint8_t spaces ) 
{
    uint16_t i = 0;         //Use this as a counter
	for(i=0;i<spaces;i++) {
		Serial.println('\r');      // Return the cursor
	}
}

void CONSOLE::clear_screen( void ) 
{
    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");         //Cursor home command
}

void CONSOLE:: cursor_top_left( void ) 
{
    Serial.print("[H");     //Send the rest of the sequence to clear the screen
}

void CONSOLE::insert_line_emphasis( void ) 
{
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~");     //Send the rest of the sequence to clear the screen
}

void CONSOLE::console ( void )
{

    uint8_t user_option = 0;
    uint8_t temp_uint8t = 0;    
    uint8_t temp_float = 0;    
    float   temporary_voltage_value = 0.0;
    uint8_t save_0x55 = 0x55;

    clear_screen(); //Don't want to run insid the while
    
    while(user_option != 99)
    {


        Serial.println("1)  Print SW version.");
        Serial.println("2)  To send test email.");
        Serial.println("3)  View RH calibration values.");
        Serial.println("4)  View network parameters.");
        Serial.println("5)  View battery voltage.");
        Serial.println("6)  View sensor readings.");
        Serial.println("7)  Perform EEPROM test.");
        Serial.println("99) To exit the console.");

        Serial.print("Enter a value: ");

        flush_serial_input_buffer();

        user_option = get_user_uint8t_value();  

        if(ENABLE_LOGGING)      // TODO:  I think we want to make this a variable up in main .ino so we can use extern here...
        {
            Serial.print("User entered option: ");
            Serial.println(user_option);
        }

        switch (user_option) 
        {
            /* Print the SW version */
            case 1:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                Serial.println("The SW version: " + SW_VER_STRING);
                insert_line_emphasis();
            break;

            /* Send test email */
            case 2:
                __asm__("nop\n\t");
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();

                //TODO: we want these functions to be put back in
                // console_lan.WiFiConnect("GlockHK23", "CJG_GbE_2G4");  //TODO: don't want to hardcode these like this
                // console_lan.send_email();  //TODO: 12/13/24 uncommenting this causes boot messages to blow up.  Something is wrong here.  
            break;

            /* View RH calibration values */
            case 3:
                __asm__("nop\n\t");
            break;

            /* View network parameters */
            case 4:
                __asm__("nop\n\t");
            break;

            /* View battery voltage */
            case 5:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                Serial.println("Printing battery voltage.");

                //TODO: we want these functions back in
                Serial.print("The battery voltage is:  ");
                Serial.print(app_function.get_battery_voltage());
                Serial.println("V");
            break;

            /* Read values from sensors */
            case 6:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                Serial.println("~~~~~!! NOT DEFINED !!~~~~~");
                insert_line_emphasis();
            break;
            
            /* Perform NVM test */
            case 7:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                Serial.println("Writing 0x55 to NVM");  //TODO: need cleanup here
                
                nvm_function.nvm_store_byte("nvm_test",0x55);

                delay(100);
                
                temp_uint8t = nvm_function.nvm_read_byte("nvm_test");

                Serial.print("Value read back from NVM:");
                Serial.println(temp_uint8t,HEX);

                insert_line_emphasis();
                insert_line_feeds(2);
            break;


            /* Exit the application */
            case 99:
                clear_screen();
                 Serial.print("Exiting terminal...");
            break;

            default:
                __asm__("nop\n\t");
            break;

    }


    }



}