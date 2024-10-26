/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/

uint8_t user_option = 0x00;

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


