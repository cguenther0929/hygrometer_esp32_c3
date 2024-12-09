#include "console.h"

void CONSOLE::init(void) {
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from EEPROM
    this -> user_selection = 99;

}

/**
 * @brief Get Number Input From User
 */
uint8_t CONSOLE::get_user_uint8t_value ( void )  //TODO: need to implement this function
{
  __asm__("nop\n\t");
  return 0xFF;
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
    user_option = get_user_uint8t_value();   //TODO: need to implement

    switch (user_option) 
    {
        /* Print the SW version */
        case 1:
            Serial.print("The SW version: ");
            // Serial.println(SW_VER_STRING);       //TODO: this is the line we want, but we keep getting errors 
            Serial.println("CORRECT THIS LINE!");
        break;

        /* Send test email */
        case 2:
            
            // TODO: we need to get the following to work
            // WiFiConnect(buf_for_router_password, buf_for_router_ssid);
            // send_email();
            
            
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