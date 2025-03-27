#include "app.h"

extern RTC_DATA_ATTR int rtc_boot_ctr;

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
I2C     app_i2c;
APP     app_functions;
LAN     app_lan;
NVM     app_nvm;

char        app_wifi_pass[PREF_BUFF_ELEMENTS]       = {NULL};
char        app_ssid[PREF_BUFF_ELEMENTS]            = {NULL};


/**
 * Display parameters
 */    
char app_temp_buffer[32]; 

void APP::init(void) 
{
  this -> state = STATE_SLEEP; 
}

void APP::state_handler(Preferences & pref, APP & app_instance ) 
{
  switch(app_instance.state) 
  {
    case STATE_SLEEP:
      if(ENABLE_LOGGING)
      {
        Serial.println("^In state **sleep**");
      }
      
      if(app_instance.bool_update_display == true)
      {
        app_instance.bool_update_display = false;
        if(ENABLE_LOGGING)
        {
          Serial.println("^Transitioning to state update display");
        }
        this -> state = STATE_UPDATE_DISPLAY; 
        app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
        
      }
      
      //                                         Value in uS  
      //                                           |  
      esp_sleep_enable_timer_wakeup((uint64_t)(SLEEP_TIME_MICROS));     // This value is a uint64_t
      if(ENABLE_LOGGING)
      {
        Serial.println("^Going to sleep");
      }
      
      /**
       * According to the doc
       * the serial buffer has to be flushed 
       * before deep sleep. 
       */
      Serial.flush();
      
      esp_deep_sleep_start();  //This will put the module into deep sleep

      /**
       * Any code after this won't be accessed 
       */
    break;
    
    //TODO This state is not being called, for now.  
    //TODO The data is read when updating the 
    //TODO display or sending an email 
    case STATE_READ_DATA:   
      if(ENABLE_LOGGING)
      {
        Serial.println("^In state **read data**");
      }
      
      app_i2c.get_sensor_data(pref);    //This will get the data from both sensors.  Values are stored into class variables
      
      app_functions.get_battery_health(); // Will store into class variable.
    
    
      this -> state = STATE_UPDATE_DISPLAY;
      app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
    break;
    
    case STATE_UPDATE_DISPLAY:
      if(ENABLE_LOGGING)
      {
        Serial.println("^In state **update display**");
      }

      if(app_functions.calibration_just_occurred) 
      {
        app_functions.calibration_just_occurred = false;
        app_functions.full_screen_refresh(pref);
      }
      else{
        app_functions.update_display(pref);
      }
    
    
      if(app_instance.bool_send_email == true) 
      {
        if(ENABLE_LOGGING)
        {
          Serial.println("^Transitioning to state send email");
        }

        app_instance.bool_send_email = false;
        this -> state = STATE_SEND_EMAIL;
        app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
      }
      else{
        this -> state = STATE_SLEEP;
        app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
      }
    break;
    
    case STATE_SEND_EMAIL:
      if(ENABLE_LOGGING)
      {
        Serial.println("^In state **send email**");
      }
      
      if(app_lan.email_enabled && app_nvm.network_valid(pref))
      {
        /**
         * Clear the temporary
         * character buffer just to be 
         * safe
         */
        memset(app_ssid, NULL, sizeof(app_ssid));
        
        /**
         * Get the SSID of the router 
         */
        app_nvm.nvm_read_string(pref, PREF_WIFI_SSID, app_ssid);
        
        
        /**
         * Clear the temporary
         * character buffer just to be 
         * safe
         */
        memset(app_wifi_pass, NULL, sizeof(app_wifi_pass));
        
        /**
         * Get the WIFI password of the router 
         */
        app_nvm.nvm_read_string(pref, PREF_WIFI_PASSWORD, app_wifi_pass);
        
        if(ENABLE_LOGGING)
        {
          Serial.print("^SSID: ");
          Serial.println(app_ssid);

          Serial.print("^WIFI Password: ");
          Serial.println(app_wifi_pass);
        }
       
        app_lan.WiFiConnect(app_ssid,app_wifi_pass);  
        app_lan.send_email(pref);
      }
      
      this -> state = STATE_SLEEP;
      app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
    break;
    
    default:
      this -> state = STATE_SLEEP;
      app_nvm.nvm_store_byte(pref, PREF_STATE, app_instance.state);
    break;
  }
  
}

void APP::display_post_message( void )
{
  if(ENABLE_LOGGING)
  {
    Serial.println("^POST message.");
  }

  app_functions.display_power_on();
  
  paint.SetWidth(200);
  paint.SetHeight(36);
  
  epd.LDirInit();          
  epd.Display(IMAGE_DATA);   
  
  paint.SetWidth(84);       // 7 pixels wide * 12 characters
  paint.SetHeight(12);   
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Hyg Starting", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 12, 116, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(77);           //7 pixels wide * 11 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "C. Guenther", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 12, 130, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(35);           //7 pixels wide * 5 characters
  paint.SetHeight(12);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, SW_VER_STRING, &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 12, 144, paint.GetWidth(), paint.GetHeight());
  
  epd.DisplayFrame();
  
  app_functions.display_power_off();
  
  epdif.hyg_spi_end();
  
}

void APP::full_screen_refresh( Preferences & pref ) 
{
  if(ENABLE_LOGGING)
  {
    Serial.println("^Full screen refresh.");
    Serial.print("^Calibration value is: ");
    Serial.println(app_nvm.nvm_read_int(pref, PREF_CAL_KEY));
  }
  app_i2c.get_sensor_data(pref);

  epdif.hyg_spi_start();

  app_functions.display_power_on();

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

  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  sprintf(app_temp_buffer,"%02d",(int)(app_i2c.temp_val1));
  
  if(ENABLE_LOGGING)
  {
    Serial.print("^The temperature value is:");
    Serial.println(app_temp_buffer);
  }
      
  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, app_temp_buffer, &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), TEMP_X_START, TEMP_Y_START, paint.GetWidth(), paint.GetHeight());
  
  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  sprintf(app_temp_buffer,"%02d",(int)app_i2c.hum_val1);
  if(ENABLE_LOGGING)
  {
    Serial.print("^The humidity value is:");
    Serial.println(app_temp_buffer);
  }
      
  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, app_temp_buffer, &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());
  
  /**
   * Font 12 is seven pixels wide.  Therefore, we can
   * have a total of 28 characters, as this will yield 
   *  28*7 (196) pixels of width
   */
  paint.eink_put_string_twoup(SW_VER_STRING);

  /**
   * Routein to update the 
   * string at the bottom of
   * the display 
   */
  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  // get_battery_health();

  
  if(app_nvm.nvm_read_int(pref, PREF_CAL_KEY) == VALID_CAL_VALUE)
  {
    // sprintf(app_temp_buffer,"BAT: %0.2f%% -- VALID CAL",this -> battery_charge_percentage);
    sprintf(app_temp_buffer,"BAT: %02d%% -- VALID CAL",app_i2c.batt_sen_soc(FILTERED));
  }
  else 
  {
    // sprintf(app_temp_buffer,"BAT: %0.2f%% -- INVALID CAL",this -> battery_charge_percentage);
    sprintf(app_temp_buffer,"BAT: %02d%% -- INVALID CAL",app_i2c.batt_sen_soc(FILTERED));
  }
  paint.eink_put_string_bottom(app_temp_buffer);
  
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

  app_functions.display_power_off();
}
        
void APP::update_display( Preferences & pref )
{
  if(ENABLE_LOGGING)
  {
    Serial.println("^Updating the display.");
  }

  app_i2c.get_sensor_data(pref);

  epdif.hyg_spi_start();

  app_functions.display_power_on();

  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  sprintf(app_temp_buffer,"%02d",(int)app_i2c.temp_val1);
  if(ENABLE_LOGGING)
  {
    Serial.print("^The temperature value is:");
    Serial.println(app_temp_buffer);
  }
  
  epd.LDirInit();               //This is needed in here.
  
  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, app_temp_buffer, &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), TEMP_X_START, TEMP_Y_START, paint.GetWidth(), paint.GetHeight());
  
  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  sprintf(app_temp_buffer,"%02d",(int)app_i2c.hum_val1);
  if(ENABLE_LOGGING)
  {
    Serial.print("^The humidity value is:");
    Serial.println(app_temp_buffer);
  }

  paint.SetWidth(64);           // 32 pixels wide x 2 characters = 64 
  paint.SetHeight(36);          // 36 pixels tall
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, app_temp_buffer, &SevenSeg_Font36, COLORED);
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());
  epd.SetFrameMemory(paint.GetImage(), HUM_X_START, HUM_Y_START, paint.GetWidth(), paint.GetHeight());

  /**
   * Routein to update the 
   * string at the bottom of
   * the display 
   */
  memset(app_temp_buffer, NULL, sizeof(app_temp_buffer));
  // get_battery_health();

  // app_i2c.batt_sen_soc(FILTERED);
  
  if(app_nvm.nvm_read_int(pref, PREF_CAL_KEY) == VALID_CAL_VALUE)
  {
    // sprintf(app_temp_buffer,"BAT: %0.2f%% -- VALID CAL",this -> battery_charge_percentage);
    sprintf(app_temp_buffer,"BAT: %02d%% -- VALID CAL",app_i2c.batt_sen_soc(FILTERED));
  }
  else 
  {
    // sprintf(app_temp_buffer,"BAT: %0.2f%% -- INVALID CAL",this -> battery_charge_percentage);
    sprintf(app_temp_buffer,"BAT: %02d%% -- INVALID CAL",app_i2c.batt_sen_soc(FILTERED));
  }
  paint.eink_put_string_bottom(app_temp_buffer);

  epd.DisplayFrame();
  
  epdif.hyg_spi_end();

  epd.Sleep();

  app_functions.display_power_off();

}
        
bool APP::network_parameters_valid( void )
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
//TODO: this may need to move to the I2C routine
//TODO can this go away now that we use the I2C routine?
void APP::get_battery_health (void) 
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

  voltage_reading = (float)(digital_reading * HYG_ADC_REFERENCE);        // Internal reference of the ADC is ~1.1V
  voltage_reading = (float)(voltage_reading / HYG_ADC_BIT_VALUE);      // The ESP32-C3 ADC is 12bit
  voltage_reading = (float)(voltage_reading * HYG_ESP32_INTERNAL_ATTEN);        //  ESP32-C3 internal attenuation (Empirically derived)
  voltage_reading = (float)(voltage_reading * HYG_PCB_ATTEN);       // To account for the attenuator on the PCB
  
  // this -> battery_charge_percentage = voltage_reading;
  this -> battery_charge_percentage = 75.33;  //TODO hardcoded number, for now

}

void APP::sensor_power_on(void)
{
    digitalWrite(nSENSOR_PWR_EN , LOW);   
}

void APP::sensor_power_off(void)
{
    digitalWrite(nSENSOR_PWR_EN , HIGH);   
}

void APP::display_power_on( void )
{
  app_i2c.set_io_expander(1, false);    // Power EN is active low 
}

void APP::display_power_off( void )
{
  app_i2c.set_io_expander(1, true);     // Power EN is active low 

}

void APP::button_handler ( void )
{

  pinMode(LOCAL_BTN_GPIO_PIN,INPUT);

  // if(digitalRead(LOCAL_BTN_GPIO_PIN))
  // {
  //   Serial.println("\t***BTN PRESSED");
  //   if(!btn_short_press_flag && !btn_long_press_flag) {
  //     Serial.println("\t***FLAGS ARE CLEARED");
  //   }
  // }


  /**
   * if the button is pushed, simply
   * update the counter
   */
  if(digitalRead(LOCAL_BTN_GPIO_PIN) &&
     !btn_short_press_flag && 
     !btn_long_press_flag
    )
    {
      btn_press_ctr++;
      // Serial.println("\t***DEBUG INCREMENTING COUNTER");
    }

    else 
    {
      if(
         ENABLE_LOGGING && btn_press_ctr > 5
        )
      {
        Serial.print("^Button counter before clearing: ");
        Serial.println(btn_press_ctr);
      }

      /**
       * Determine if we need to define 
       * a short or long press flag
       * before we clear the counter
       */
      if(btn_press_ctr >= SHORT_PRESS_50MS_EVENTS &&
         btn_press_ctr < LONG_PRESS_50MS_EVENTS &&
         !btn_short_press_flag && !digitalRead(LOCAL_BTN_GPIO_PIN)
        )
        {
          btn_short_press_flag  = true;
        }
      else if (btn_press_ctr >= LONG_PRESS_50MS_EVENTS &&
               !btn_long_press_flag && !btn_short_press_flag &&
               !digitalRead(LOCAL_BTN_GPIO_PIN)
              )
        {
          btn_long_press_flag  = true;
        }
      
      btn_press_ctr=0;
    }
    
    
  /**
   * Determine what to do based on 
   * press duration flags that were
   * set above
   */
  if(btn_short_press_flag) 
  {
    btn_short_press_flag = false;
    if(ENABLE_LOGGING)
    {
      Serial.println("^Button short press has been triggered");
    }
  }
  
  if(btn_long_press_flag) 
  {
    btn_long_press_flag = false;
    calibrate_sensors = true;
    if(ENABLE_LOGGING)
    {
      Serial.println("^Button long press has been triggered -- calibrate sensors.");
    }
  }
}




