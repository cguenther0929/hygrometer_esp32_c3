/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/



/**
 * WiFi and Email 
 * Related 
 */
#define WIFI_CONNECT_TIMEOUT_S  5
#define WIFI_LOGGING            1         //TODO: do we want to comment this out?              
// char buf_for_router_password[32];  //TODO: this is the line that we want in
// char buf_for_router_ssid[32];      //TODO: this is the line that we want in

//TODO: Need to add these line back in and allow user to
//TODO: define the parameters
// char buf_for_hyg_name[32]
// char buf_for_smtp2go_b64_account[64]
// char buf_for_smtp2go_b64_password[64]
// char buf_for_recipient_email[64]
// char buf_for_sender_email[64]

//TODO: The following is just for testing...
char buf_for_router_password[]      = "GlockHK23";
char buf_for_router_ssid[]          = "CJG_GbE_2G4";

char buf_for_hyg_name[]             = "CJG HYG";
// char buf_for_smtp2go_b64_account[]  = "Y2xpbnRvbi5ndWVudGhlckBnbWFpbC5jb20=";
char buf_for_email_password[]       = "TYgvik uoit bdja grxu";
// char buf_for_smtp2go_b64_account[]  = "Y2xpbnRvbi5ndWVudGhlckBnbWFpbC5jb20=";
// char buf_for_smtp2go_b64_password[] = "TUolZGZoKiY0NQ==";
char buf_for_recipient_email[]      = "clinton.guenther@gmail.com";
char buf_for_sender_email[]         = "clinton.guenther@gmail.com";

#define SMTP_PORT                   465

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
char server[] = "smtp.gmail.com";  


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

bool WiFiConnect( const char * ssid, const char * password ) 
{
  int i = 0;
  int timeout = (int)WIFI_CONNECT_TIMEOUT_S/0.5;
  
  
  WiFi.begin(ssid, password);

  /**
   * Print diagnostic data
   * for WiFi if logging
   * is enabled
   */
  if(WIFI_LOGGING) {
    Serial.println("");
    Serial.print("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
    Serial.print("\tConnecting to SSID: "); Serial.println(ssid);
    Serial.print("\tSSID password: "); Serial.println(password);
  }

  
  if(WIFI_LOGGING) {
    Serial.print("\tWiFi Connecting\t");
  }
  
  // Wait for connection
  while ((WiFi.status() != WL_CONNECTED) && i < timeout) {
    delay(500);
    i++;
    if(WIFI_LOGGING) {
      Serial.print('.');
    }
  }
  
  if(WIFI_LOGGING) {
    Serial.println("");
  }

  if(i == timeout){
  #if defined(WIFI_LOGGING)
      Serial.println("\tWiFi Connection timeout!");
      return false;
  }
  #endif

  if(WIFI_LOGGING) {
      Serial.println("\tWiFi connected!");
      Serial.print("\tMy local IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("\tSeeting WiFi Transmit Power");
  }
    
  // WiFi.setOutputPower(0);   //TODO: do we want this line in?

  return true;
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
void send_email ( void )
{

  /* Callback function to get the Email sending status */
  void smtpCallback(SMTP_Status status);

  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = server;
  config.server.port = SMTP_PORT;
  config.login.email = buf_for_sender_email;
  config.login.password = buf_for_email_password;
  config.login.user_domain = "";

  /**
  * Set the NTP config time
  * For times east of the Prime Meridian use 0-12
  * For times west of the Prime Meridian add 12 to the offset.
  * Ex. American/Denver GMT would be -6. 6 + 12 = 18
  * See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  * 
  * TODO: DO I NEED THIS?  
   */
  // config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  // config.time.gmt_offset = 3;
  // config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("Hygrometer");
  message.sender.email = buf_for_sender_email;
  message.subject = F("ESP Test Email");
  message.addRecipient(F("CJG"), buf_for_recipient_email);

  
  //Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
  

/* Connect to the server */
if (!smtp.connect(&config))
{
  if(WIFI_LOGGING)
  {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
  }
  return;
}

if (!smtp.isLoggedIn() && WIFI_LOGGING)
{
  Serial.println("\nNot yet logged in.");
}
else
{
  if (smtp.isAuthenticated() && WIFI_LOGGING)
    Serial.println("\nSuccessfully logged in.");
  else
    Serial.println("\nConnected with no Auth.");
}

/* Start sending Email and close the session */
if (!MailClient.sendMail(&smtp, &message))
{
  ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}

  
  
  // AssembleEmailMessage();

  /**
   * @brief Sending EHLO command
   */
  // if(WIFI_LOGGING)
  //     Serial.println(F("Sending EHLO"));

  // client.println("EHLO www.example.com");


  /**
   * @brief Sending auth login command
   */
  // if(WIFI_LOGGING)
  //     Serial.println(F("Sending auth login"));

  // client.println("auth login");
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.println(F("\t*** Error sending AUTH LOGIN command."));
      
      // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


  /**
   * @brief Send SMTP2GO User Account Credentials
   */
  // if(WIFI_LOGGING)
  //     Serial.print(F("Sending SMTP2GO B64 User Account Name: "));
  //     Serial.println(buf_for_smtp2go_b64_account);


  // client.println(buf_for_smtp2go_b64_account); //B64 encoded SMTP2GO username
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.print(F("\t*** Error sending SMTP2GO Username: "));
  //         Serial.println(buf_for_smtp2go_b64_account);
      
  //     // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


  /**
   * @brief Send SMTP2GO Password
   * 
   * THis should be the BASE64 password 
   * for you SMTP2GO account
   */
  // if(WIFI_LOGGING)
  //     Serial.print(F("Sending B64 SMTP2GO Password: "));
  //     Serial.println(buf_for_smtp2go_b64_password);


  // client.println(buf_for_smtp2go_b64_password);  
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.println(F("\t*** Error sending SMTP2GO password"));
      
  //     // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


  /**
   * @brief Command for MAIL From:
   * i.e.  --> client.println(F("MAIL From: clinton.debug@gmail.com"));
   * 
   */
  // memset(buf_temp, 0, TEMP_BUF_SIZE);
  // strcpy(buf_temp, "MAIL From: ");
  // strcat(buf_temp, buf_for_sender_email);
  // client.println(buf_temp);
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.print(F("\t*** Error on command: "));
  //         Serial.println(buf_temp);
      
  //     // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


  /**
   * @brief Enter recipient address
   * First, fill temp buffer with null characters
   * i.e.  -->  client.println(F("RCPT To: clinton.guenther@gmail.com"));
   * 
   */
  // if(WIFI_LOGGING)
  //     Serial.print(F("Sending To: "));
  //     Serial.println(buf_for_recipient_email);


  // memset(buf_temp, 0, TEMP_BUF_SIZE);
  // strcpy(buf_temp, "RCPT To: ");
  // strcat(buf_temp, buf_for_recipient_email);

  // client.println(buf_temp);
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.print(F("\t*** Error on command: "));
  //         Serial.println(buf_temp);
      
  //     // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


  /**
   * @brief Send DATA command
   */
  // if(WIFI_LOGGING)
  //     Serial.println(F("Sending DATA"));


  // client.println(F("DATA"));
  // if (!eRcv()){
  //     if(WIFI_LOGGING)
  //         Serial.println(F("\t*** Error on command \"DATA\"."));
      
  //     // current_state = DEEP_SLEEP;   //TODO: do we need a way to break out and sleep?  
  // }


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

} /* END SEND EMAIL*/




/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  //TODO: do we want to do anything in this function?
  __asm__("nop\n\t");

  /* Print the current status */
  // Serial.println(status.info());

  /* Print the sending result */
  // if (status.success()){
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    // Serial.println("----------------");
    // ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    // ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    // Serial.println("----------------\n");

    // for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    // {
    //   /* Get the result item */
    //   SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
      
    //   ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
    //   ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
    //   ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
    //   ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
    //   ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    // }
    // Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }


/**
 * @brief Send the email
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




