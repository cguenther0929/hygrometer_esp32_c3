#include "app.h"


void APP::init(void) 
{
    __asm__("nop\n\t");
}

void APP::display_splash_screen( void ) 
{
    //TODO: need to define this function 
    __asm__("nop\n\t");
}

bool APP:: network_parameters_valid( void )
{

    network_info network_info;

    /**
     * If the first characters 
     * of the various network parameters are 
     * non-null, the routine will assume the parameters are 
     * valid
     */
    if (network_info.hyg_name[0] = NULL)
    {
        return false;
    }
    if (network_info.wifi_ssid[0] = NULL)
    {
        return false;
    }
    if (network_info.wifi_password[0] = NULL)
    {
        return false;
    }
    if (network_info.recipient_email_address[0] = NULL)
    {
        return false;
    }
    if (network_info.sender_email_address[0] = NULL)
    {
        return false;
    }
    return true;

}

/**
 * @brief Get Battery Voltage
 */
float APP::get_battery_voltage (void) 
{
  uint16_t  digital_reading   = 0;
  float voltage_reading       = 0.0;
  float temp_reading          = 0.0;
  float temp_sensor_xfer      = 0.0;

  /**
   * The ESP32-C3's A/D is 12 bit.
   * 
   * Other useful functions:
   * analogSetAttenuation(attenuation): sets the input
   * attenuation for all ADC pins. Default is ADC_11db.
   * Accepted values:
   * ADC_0db: sets no attenuation. ADC can measure up to
   * approximately 800 mV (1V input = ADC reading of 1088).
   * ADC_2_5db: The input voltage of ADC will be attenuated, 
   * extending the range of measurement to up to approx. 1100 mV. 
   * (1V input = ADC reading of 3722).
   * ADC_6db: The input voltage of ADC will be attenuated, 
   * extending the range of measurement to up to approx. 1350 mV.
   * (1V input = ADC reading of 3033).
   * ADC_11db: The input voltage of ADC will be attenuated, 
   * extending the range of measurement to up to approx. 2600 mV.
   * (1V input = ADC reading of 1575).
   * 
   */

  digital_reading = analogRead(ANALOG_BATT_PIN);
  
//   voltage_reading = (float)(digital_reading*ADC_REFERENCE); //TODO need to fix the following to replace the hardcoded lines
//   voltage_reading = (float)(voltage_reading/ADC_BIT_VALUE);
  voltage_reading = (float)(digital_reading*1.10);
  voltage_reading = (float)(voltage_reading/496.0);
  
  return voltage_reading;
}

void APP::sensor_power_on(void)
{
    digitalWrite(nSENSOR_PWR_EN , LOW);   
}


void APP::sensor_power_off(void)
{
    digitalWrite(nSENSOR_PWR_EN , HIGH);   
}




