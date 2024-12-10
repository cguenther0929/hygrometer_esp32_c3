/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/

#define TEMP_BUF_SIZE               64
char buf_temp[TEMP_BUF_SIZE];



// #define WIFI_SSID "CJG_GbE_2G4"
// #define WIFI_PASSWORD "GlockHK23"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
// #define SMTP_HOST "smtp.gmail.com"
// #define SMTP_PORT 465

//TODO: the following () lines need
//TODO: to be deleted completely
char humidity_1[]                   = "61";
char humidity_2[]                   = "62";
char temperature_1[]                = "71";
char temperature_2[]                = "72";
char battery_v[]                    = "11";


String email_message = "";
// char server[] = "mail.smtp2go.com";  



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

void state_handler( void )
{
  //TODO: need to define this routine
  __asm__("nop\n\t");
}


/**
 * @brief Get Battery Voltage
 */
float get_battery_voltage (void) 
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
  
  voltage_reading = (float)(voltage_reading*ADC_REFERENCE);
  voltage_reading = (float)(digital_reading/ADC_BIT_VALUE);
  
  return voltage_reading;
}




