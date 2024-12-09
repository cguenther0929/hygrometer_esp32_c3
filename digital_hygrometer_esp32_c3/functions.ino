/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/



             
// char buf_for_router_password[32];  //TODO: this is the line that we want in
// char buf_for_router_ssid[32];      //TODO: this is the line that we want in

//TODO: Need to add these line back in and allow user to
//TODO: define the parameters
// char buf_for_hyg_name[32]
// char buf_for_smtp2go_b64_account[64]
// char buf_for_smtp2go_b64_password[64]
// char buf_for_recipient_email[64]
// char buf_for_sender_email[64]

// TODO: The following is just for testing...
// TODO: We likely don't want to hardcode a lot of the WIFI-related stuff

// char buf_for_smtp2go_b64_account[]  = "Y2xpbnRvbi5ndWVudGhlckBnbWFpbC5jb20=";

// char buf_for_smtp2go_b64_account[]  = "Y2xpbnRvbi5ndWVudGhlckBnbWFpbC5jb20=";
// char buf_for_smtp2go_b64_password[] = "TUolZGZoKiY0NQ==";




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



// byte eRcv()
// {
//   byte respCode;
//   byte thisByte;
//   int loopCount = 0;

//   while (!client.available()) {
//     delay(1);
//     loopCount++;
//     // if nothing received for 10 seconds, timeout
//     if (loopCount > 10000) {
//       client.stop();
//       Serial.println(F("\r\nTimeout"));
//       return 0;
//     }
//   }

//   respCode = client.peek();
//   while (client.available())
//   {
//     thisByte = client.read();
//     Serial.write(thisByte);
//   }

//   if (respCode >= '4')
//   {
//     //  efail();
//     return 0;
//   }
//   return 1;
// }




/**
 * @brief Assemble the Email Message
 */
// void AssembleEmailMessage ( void ) {
//     /**
//      * NOTE: The body of the email message will 
//      * will look something like:
//      * Hygrometer's Name: "My Name is.."
//      * Humidity 1: 65.23
//      * Humidity 2: 67.25
//      * Temperature 1: 73.45
//      * Temperature 2: 73.44
//      * Battery voltage: 3.25
//      * Battery Low: false
//      */
//     email_message =     "Hygrometer: " + String(buf_for_hyg_name) + "\r\n";
//     email_message +=    "Humidity 1: " + String(humidity_1) + " % \r\n";
//     email_message +=    "Humidity 2: " + String(humidity_2) + " % \r\n";
//     email_message +=    "Temperature 1: " + String(temperature_1) + " \u00B0 F \r\n";
//     email_message +=    "Temperature 2: " + String(temperature_2) + " \u00B0 F \r\n";
//     email_message +=    "Battery Voltage: " + String(battery_v) + " V \r\n";
//     if(battery_too_low) {
//         email_message +=    "Battery Low: True \r\n";
//     }
//     else {
//         email_message +=    "Battery Low: False \r\n";
//     }

    
// }





/**
 * @brief Send the email
 * TODO: this is the original version
 * TODO: that is in for reference
 * 
 * TODO: need to delete the following note:
 * This seems like a great tutorial 
 * https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/#send-email
 * 
 * The code here compiled on 11/9/24
 * C:\Users\Neal Quackenbush\Dropbox\My Design Projects\Projects\Digital Hygrometer\DESIGN\
 * SW\Email Sender ESP32 NOV 2024\email_sender_esp32_nov_2024
 * 
 * An app password was added to the 
 * clinton.debug@gmail.com account
 * 
 * The app password given was:  gvik uoit bdja grxu
 * 
 * 
 */
// void send_email ( void )
// {

//   AssembleEmailMessage();

//   /**
//    * @brief Make a client connection
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Making client connection"));

//   if (client.connect(server, 2525) == 1) {
//       if(WIFI_LOGGING)
//           Serial.println(F("connected"));
      
//   } 
//   else {
//       if(WIFI_LOGGING)
//           Serial.println(F("connection failed"));
      
//       return;
//   }
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error connecting to the client"));
      
//       // current_state = DEEP_SLEEP;     //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Sending EHLO command
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Sending EHLO"));

//   client.println("EHLO www.example.com");
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error sending EHLO command."));
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Sending auth login command
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Sending auth login"));


//   client.println("auth login");
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error sending AUTH LOGIN command."));
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Send SMTP2GO User Account Credentials
//    */
//   if(WIFI_LOGGING)
//       Serial.print(F("Sending SMTP2GO B64 User Account Name: "));
//       Serial.println(buf_for_smtp2go_b64_account);


//   client.println(buf_for_smtp2go_b64_account); //B64 encoded SMTP2GO username
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.print(F("\t*** Error sending SMTP2GO Username: "));
//           Serial.println(buf_for_smtp2go_b64_account);
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Send SMTP2GO Password
//    * 
//    * THis should be the BASE64 password 
//    * for you SMTP2GO account
//    */
//   if(WIFI_LOGGING)
//       Serial.print(F("Sending B64 SMTP2GO Password: "));
//       Serial.println(buf_for_smtp2go_b64_password);


//   client.println(buf_for_smtp2go_b64_password);  
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error sending SMTP2GO password"));
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Command for MAIL From:
//    * i.e.  --> client.println(F("MAIL From: clinton.debug@gmail.com"));
//    * 
//    */
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "MAIL From: ");
//   strcat(buf_temp, buf_for_sender_email);
//   client.println(buf_temp);
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.print(F("\t*** Error on command: "));
//           Serial.println(buf_temp);
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Enter recipient address
//    * First, fill temp buffer with null characters
//    * i.e.  -->  client.println(F("RCPT To: clinton.guenther@gmail.com"));
//    * 
//    */
//   if(WIFI_LOGGING)
//       Serial.print(F("Sending To: "));
//       Serial.println(buf_for_recipient_email);


//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "RCPT To: ");
//   strcat(buf_temp, buf_for_recipient_email);

//   client.println(buf_temp);
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.print(F("\t*** Error on command: "));
//           Serial.println(buf_temp);
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Send DATA command
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Sending DATA"));


//   client.println(F("DATA"));
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error on command \"DATA\"."));
      
//       // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Sending To: command
//    * i.e.  --> client.println(F("To: clinton.guenther@gmail.com"));
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Sending email"));

//   //  client.println(F("To:  clinton.guenther@gmail.com"));
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "To: ");
//   strcat(buf_temp, buf_for_recipient_email);
//   client.println(buf_temp);


//   /**
//    * @brief Sending From: command
//    * i.e. -->  client.println(F("From: clinton.debug@gmail.com"));
//    */
//   // client.println(F("From: clinton.debug@gmail.com"));
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "From: ");
//   strcat(buf_temp, buf_for_sender_email);
//   client.println(buf_temp);


//   /**
//    * @brief Send the subject
//    */
//   client.println(F("Subject: Hygrometer Health Report\r\n"));
//   client.println(email_message);
//   client.println(F("."));
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error sending DOT to complete transaction"));
      
//       // current_state = DEEP_SLEEP;    //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Sending QUIT
//    * 
//    */
//   if(WIFI_LOGGING)
//       Serial.println(F("Sending QUIT"));

              
//   client.println(F("QUIT"));
//   if (!eRcv()){
//       if(WIFI_LOGGING)
//           Serial.println(F("\t*** Error sending \"QUIT\"."));
      
//       // current_state = DEEP_SLEEP;    //TODO: do we need a way to break out and sleep?  
//   }


//   /**
//    * @brief Disconnecting 
//    */
//   client.stop();

//   if(WIFI_LOGGING)
//     Serial.println(F("disconnected"));

// }








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




