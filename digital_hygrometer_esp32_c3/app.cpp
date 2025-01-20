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
  
  voltage_reading = (float)(digital_reading * HYG_ADC_REFERENCE);        // Internal reference of the ADC is ~1.1V
  voltage_reading = (float)(voltage_reading / HYG_ADC_BIT_VALUE);      // The ESP32-C3 ADC is 12bit
  voltage_reading = (float)(voltage_reading * HYG_PCB_ATTEN);       // To account for the attenuator on the PCB
  voltage_reading = (float)(voltage_reading * HYG_ESP32_INTERNAL_ATTEN);        //  ESP32-C3 internal attenuation (Empirically derived)
  
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

void APP::button_handler ( void )
{
  //TODO: do we want to do something with the following flag?
  //TODO:  "btn_interrupt_triggered "
  //TODO: if not, make sure this flag is cleared.

  /**
   * if the button is pushed, simply
   * update the counter
   */
  if(digitalRead(LOCAL_BTN_GPIO_PIN &&
      !btn_short_press_flag && 
      !btn_long_press_flag))
  {
    btn_press_ctr++;
  }
  else 
  {
    btn_press_ctr=0;
  }

  /**
   * Determine if we need to define 
   * a short or long press flag
   */
  if(btn_press_ctr >= SHORT_PRESS_50MS_EVENTS &&
    btn_press_ctr <= LONG_PRESS_50MS_EVENTS &&
    !btn_short_press_flag)
    {
      btn_short_press_flag  = true;
    }
  else if(btn_press_ctr >= LONG_PRESS_50MS_EVENTS &&
          !btn_short_press_flag)
    {
      btn_long_press_flag  = true;
      calibrate_sensors = true;
    }
}




