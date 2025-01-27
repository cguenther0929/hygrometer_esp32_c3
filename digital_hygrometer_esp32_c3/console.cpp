#include "console.h"


/**
 * Instantiate classes
 */
LAN     console_lan;
APP     app_function;
I2C     i2c_function;
NVM     nvm_function;


// const char SW_VER_STRING[] = "temp.temp.temp";  //TODO need to fix this!  Do not want to hardcode like this.

/**
 * Set to true to 
 * enable logging
 */
#define ENABLE_LOGGING                true

bool WiFiConnect( const char * ssid, const char * password );


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
    
    while (Serial.available() <= 0);    // Pause until we start receiving data
    
    Serial.setTimeout(3000);            // Value is in milli-seconds

    char c = Serial.parseInt();         // Parse integer value input by user
    
    return_number = int(c);

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

// void CONSOLE::console ( Preferences & pref )  //TODO this was the original line
void CONSOLE::console ()  
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
        Serial.println("11) Print HW version.");
        Serial.println("2)  To send test email.");
        Serial.println("3)  View RH calibration values.");
        Serial.println("4)  View network parameters.");
        Serial.println("5)  Vew battery voltage.");
        Serial.println("6)  View sensor readings.");
        Serial.println("7)  Perform EEPROM test.");
        Serial.println("8)  Read the state of the push button.");
        Serial.println("9)  Read the busy input as sourced by the display.");
        Serial.println("10) Look at charge enable bit.");
        Serial.println("11) Not implemented.");

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
                Serial.println("The SW version: FIX ME");
                insert_line_emphasis();
            break;

            /* Report the HW version */
            case 11:            //TODO need to change this to 2
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();

                Serial.print("HW revision: ");
                Serial.println(i2c_function.get_hw_revision()); 


                insert_line_emphasis();
                insert_line_emphasis();
            break;
            
            /* Send test email */
            case 2:
                __asm__("nop\n\t");
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();

                //TODO: we want these functions to be put back in
                console_lan.WiFiConnect("CJG_GbE_2G4","GlockHK23");  //TODO we don't want to hardcode these values like this
                // WiFiConnect("CJG_GbE_2G4","GlockHK23");
                console_lan.send_email();  //TODO: 12/13/24 uncommenting this causes boot messages to blow up.  Something is wrong here.  
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

                temporary_voltage_value = app_function.get_battery_voltage();
                Serial.print("Battery voltage: ");
                Serial.println(temporary_voltage_value);

                insert_line_emphasis();
                insert_line_feeds(2);
            break;

            /* Read values from sensors */
            case 6:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();

                i2c_function.get_sensor_data(); 

                Serial.print("Humidity #1: ");
                Serial.println(i2c_function.hum_val1);
                Serial.print("Temperature #1: ");
                Serial.println(i2c_function.temp_val1);
                Serial.println(' ');

                insert_line_emphasis();

                Serial.print("Humidity #2: ");
                Serial.println(i2c_function.hum_val2);
                Serial.print("Temperature #2: ");
                Serial.println(i2c_function.temp_val2);

                insert_line_emphasis();
                insert_line_feeds(2);
            break;
            
            /* Perform NVM test */  //TODO we need to put something back in here
            case 7:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                // Serial.println("Writing 0x55 to NVM");  //TODO: need cleanup here
                
                // nvm_function.nvm_store_byte(pref, "nvm_test",0x55);

                // delay(100);
                
                // temp_uint8t = nvm_function.nvm_read_byte(pref,"nvm_test");

                // Serial.print("Value read back from NVM:");
                // Serial.println(temp_uint8t,HEX);

                insert_line_emphasis();
                insert_line_feeds(2);
            break;
            
            /* Read the state of the push button */
            case 8:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                
                pinMode(LOCAL_BTN_GPIO_PIN,INPUT);


                Serial.print("The current state of the push button is: ");  
                
                if(digitalRead(LOCAL_BTN_GPIO_PIN)) 
                {
                    Serial.println("HIGH.");  
                }
                else
                {
                    Serial.println("LOW.");  
                }

                insert_line_emphasis();
                insert_line_feeds(2);
            break;
            
            /* Read the display's busy flag */
            case 9:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                
                pinMode(BUSY_PIN,INPUT);


                Serial.print("The current state of the busy pin is: ");  
                
                if(digitalRead(BUSY_PIN)) 
                {
                    Serial.println("HIGH.");  
                }
                else
                {
                    Serial.println("LOW.");  
                }

                insert_line_emphasis();
                insert_line_feeds(2);
            break;
            
            /* Look at charging bit */
            case 10:
                clear_screen();
                insert_line_feeds(2);
                insert_line_emphasis();
                
                pinMode(BUSY_PIN,INPUT);


                Serial.print("The current state of the busy pin is: ");  
                
                if(digitalRead(BUSY_PIN)) 
                {
                    Serial.println("HIGH.");  
                }
                else
                {
                    Serial.println("LOW.");  
                }

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