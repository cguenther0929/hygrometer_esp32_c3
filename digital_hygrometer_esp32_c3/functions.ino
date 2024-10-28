/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/

/**
 * Analog related parameters
 */
#define ANALOG_BATT_PIN           0
float ADC_REFERENCE     = 1.10;         // ESP32-C3 ADC reference
float ADC_BIT_VALUE     = 4096.0;       // ESP32-C3 bit value (12 bit ADC)

uint8_t user_option = 0x00;

void button_handler ( void )
{
  //TODO: do we want to do something with this flag?
  //TODO:  "btn_interrupt_triggered "

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

void state_handler( void )
{
  //TODO: need to define this routine
  __asm__("nop\n\t");
}

float get_battery_voltage (void) 
{
  uint8_t  digital_reading  = 0;
  float voltage_reading     = 0.0;
  float temp_reading        = 0.0;
  float temp_sensor_xfer    = 0.0;

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
  
  voltage_reading = (float)(voltage_reading*ADC_REFERENCE);
  voltage_reading = (float)(digital_reading/ADC_BIT_VALUE);
  
  return voltage_reading;
}


uint8_t get_uint8_value ( void )
{
  __asm__("nop\n\t");
  return 0xFF;
}

void console ( void )
{
  float temporary_voltage_value = 0.0;

  Serial.println("1) Print SW version.");
  Serial.println("2) To send test email");
  Serial.println("3) View RH calibration values");
  Serial.println("4) View network parameters");
  Serial.println("5) View battery voltage");

  Serial.println("Enter a value: ");
  user_option = get_uint8_value();

  switch (user_option) 
  {
    /* Print the SW version */
    case 1:
      Serial.print("The SW version: ");
      Serial.println(SW_VER_STRING);
    break;
    
    /* Send test email */
    case 2:
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


