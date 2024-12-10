#include "console.h"
#include "lan.h"

/**
 * Instantiate classes
 */
LAN     console_lan;



extern String SW_VER_STRING;


void CONSOLE::init(void) 
{
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from EEPROM
    this -> user_selection = 99;

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

void CONSOLE::console ()
{


    float temporary_voltage_value = 0.0;
    uint8_t user_option = 0;

    Serial.println("1) Print SW version.");
    Serial.println("2) To send test email");
    Serial.println("3) View RH calibration values");
    Serial.println("4) View network parameters");
    Serial.println("5) View battery voltage");

    Serial.println("Enter a value: ");
    user_option = get_user_uint8t_value();  

    switch (user_option) 
    {
        /* Print the SW version */
        case 1:
            Serial.println("The SW version: " + SW_VER_STRING);
        break;

        /* Send test email */
        case 2:
            
            // TODO: do the following without hardcoding 
            // lan.WiFiConnect(buf_for_router_password, buf_for_router_ssid);
            console_lan.WiFiConnect("GlockHK23", "CJG_GbE_2G4");  
            console_lan.send_email();
            
            
            __asm__("nop\n\t");
        break;

        /* Vie RH calibration values */
        case 3:
            __asm__("nop\n\t");
        break;

        /* View network parameters */
        case 4:
            __asm__("nop\n\t");
        break;

        /* View battery voltage */
        case 5:
            __asm__("nop\n\t");
        break;

        /* Exit the application */
        case 99:
            __asm__("nop\n\t");
        break;

        default:
            __asm__("nop\n\t");

    }





}