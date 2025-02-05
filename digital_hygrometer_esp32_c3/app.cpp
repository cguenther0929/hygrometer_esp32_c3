#include "app.h"

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];   
Paint   paint(image, 0, 0);    // width should be the multiple of 8 
Epd     epd;
EpdIf   epdif;


/**
 * Display parameters
 */    
char bottom_of_disp_string[32]; 

void APP::init(void) 
{
    this -> state = STATE_READ_DATA;
}

void APP::state_handler( State current_state ) 
{
  switch(current_state) 
  {
    case STATE_SLEEP:
      if(ENABLE_LOGGING)
      {
        Serial.println("In state sleep");
      }
      this -> state = STATE_READ_DATA;
    break;
    case STATE_READ_DATA:
      if(ENABLE_LOGGING)
      {
        Serial.println("In state read data");
      }
      this -> state = STATE_UPDATE_DISPLAY;
    break;
    case STATE_UPDATE_DISPLAY:
      if(ENABLE_LOGGING)
      {
        Serial.println("In state update display");
      }
      this -> state = STATE_SEND_EMAIL;
    break;
    case STATE_SEND_EMAIL:
      if(ENABLE_LOGGING)
      {
        Serial.println("In state send email");
      }
      this -> state = STATE_SLEEP;
    break;
    default:
     this -> state = STATE_SLEEP;
    break;
  }

}

void APP::display_splash_screen( void ) 
{
  paint.SetWidth(200);
  paint.SetHeight(36);
  
  epd.LDirInit();            
  epd.Display(IMAGE_DATA);   


  paint.SetWidth(77);         //7 pixels wide * 11 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Temperature", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 12, 112, paint.GetWidth(), paint.GetHeight());
  
  
  paint.SetWidth(56);           //7 pixels wide * 8 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Humidity", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 112, 112, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "75", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), TEMP_X_START, TEMP_Y_START, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "68", &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());



  /**
  * Font 12 is seven pixels wide.  Therefore, we can
  * have a total of 28 characters, as this will yield 
  *  28*7 (196) pixels of width
  */
  get_battery_voltage();
  sprintf(bottom_of_disp_string,"BAT: %0.2fV",this -> battery_voltage);
  paint.eink_put_string_bottom(bottom_of_disp_string);

  /** 
   * Print the divider line 
   */
  paint.SetWidth(4);
  paint.SetHeight(132);
  paint.Clear(UNCOLORED);
  paint.DrawLine(0, 0, 1, 132, COLORED);
  paint.DrawLine(1, 0, 2, 132, COLORED);
  paint.DrawLine(2, 0, 3, 132, COLORED);
  paint.DrawLine(3, 0, 4, 132, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 100, 100, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();

  epdif.hyg_spi_end();

  epd.Sleep();
}

bool APP:: network_parameters_valid( void )
{

    // network_info network_info;

    /**
     * If the first characters 
     * of the various network parameters are 
     * non-null, the routine will assume the parameters are 
     * valid
     */
    // if (network_info.hyg_name[0] = NULL)
    // {
    //     return false;
    // }
    // if (network_info.wifi_ssid[0] = NULL)
    // {
    //     return false;
    // }
    // if (network_info.wifi_password[0] = NULL)
    // {
    //     return false;
    // }
    // if (network_info.recipient_email_address[0] = NULL)
    // {
    //     return false;
    // }
    // if (network_info.sender_email_address[0] = NULL)
    // {
    //     return false;
    // }
    return true;

}

/**
 * @brief Get Battery Voltage
 */
float APP::get_battery_voltage (void) 
{
  uint16_t    digital_reading       = 0;
  float       voltage_reading       = 0.0;
  float       temp_reading          = 0.0;
  float       temp_sensor_xfer      = 0.0;

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
   * ADC_6db: The input voltage of ADC will be attenuated, 
   * ADC_11db: The input voltage of ADC will be attenuated, 
   */

  analogSetAttenuation(ADC_11db);

  digital_reading = analogRead(ANALOG_BATT_PIN);

  // Serial.print("****DEBUG raw ADC digital reading: "); //TODO we can remove these lines
  // Serial.println(digital_reading);
  
  voltage_reading = (float)(digital_reading * HYG_ADC_REFERENCE);        // Internal reference of the ADC is ~1.1V
  voltage_reading = (float)(voltage_reading / HYG_ADC_BIT_VALUE);      // The ESP32-C3 ADC is 12bit
  voltage_reading = (float)(voltage_reading * HYG_ESP32_INTERNAL_ATTEN);        //  ESP32-C3 internal attenuation (Empirically derived)
  voltage_reading = (float)(voltage_reading * HYG_PCB_ATTEN);       // To account for the attenuator on the PCB
  
  this -> battery_voltage = voltage_reading;

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




