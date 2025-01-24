#include "lan.h"

/**
 * Email related
 * 
 * Declare the global used SMTPSession 
 * object for SMTP transport 
 */
// SMTPSession smtp_session;

// WiFiClient Client;

#define TEMP_BUF_SIZE             64    // Size of the temporary buffer
char buf_temp[TEMP_BUF_SIZE];           // Temporary buffer that can be used for building strings

char server[] = "smtp.gmail.com";  

char buf_recipient_email[]      = "clinton.guenther@gmail.com";
char buf_sender_email[]         = "clinton.debug@gmail.com";
//Base 64 encoded results for clinton.debug@gmail.com == Y2xpbnRvbi5kZWJ1Z0BnbWFpbC5jb20=
char buf_b64_encoded_sender_email[] = "Y2xpbnRvbi5kZWJ1Z0BnbWFpbC5jb20=";

char buf_email_password[]       = "krrm ceex fwxm ubpk";
char buf_b64_email_password[]       = "a3JybSBjZWV4IGZ3eG0gdWJwaw==k";

char buf_router_ssid[]          = "CJG_GbE_2G4";
char buf_router_password[]      = "GlockHK23";

char buf_for_hyg_name[]             = "CJG HYG";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL      "clinton.debug@gmail.com"
#define AUTHOR_PASSWORD   "krrm ceex fwxm ubpk"
#define RECIPIENT_EMAIL "clinton.guenther@gmail.com"

void LAN::init(void) 
{
    //TODO: Let's just put a print here with inclusion guards
    /* Declare the global used SMTPSession object for SMTP transport */
    // SMTPSession smtp;
    __asm__("nop\n\t");  //TODO: we need to do something different here

}


// bool LAN::WiFiConnect( const char * ssid, const char * password ) 
// {
 
//   if(WIFI_LOGGING) {
//     Serial.println("Connecting to WIFI.");
//   }
  
//   int i = 0;
//   int timeout = (int)WIFI_CONNECT_TIMEOUT_S/0.5;
  
  
//   if(WIFI_LOGGING) {
//     Serial.println("Calling Wifi.begin().");
//   }
//   WiFi.begin(ssid, password);

//   /**
//    * Print diagnostic data
//    * for WiFi if logging
//    * is enabled
//    */
//   if(WIFI_LOGGING) {
//     Serial.println("");
//     Serial.println("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
//     Serial.println("\tConnecting to SSID: "); Serial.println(ssid);
//     Serial.println("\tSSID password: "); Serial.println(password);
//   }

  
//   if(WIFI_LOGGING) {
//     Serial.println("\tWiFi Connecting\t");
//   }
  
//   // Wait for connection
//   while ((WiFi.status() != WL_CONNECTED) && i < timeout) {
//     delay(500);
//     i++;
//     if(WIFI_LOGGING) {
//       Serial.print('.');
//     }
//   }
  
//   if(WIFI_LOGGING) {
//     Serial.println("");
//   }

//   if(i == timeout){
//   #if defined(WIFI_LOGGING)
//       Serial.println("\tWiFi Connection timeout!");
//       return false;
//   }
//   #endif

//   if(WIFI_LOGGING) {
//       Serial.println("\tWiFi connected!");
//       Serial.println("\tMy local IP: ");
//       Serial.println(WiFi.localIP());
//       Serial.println("\tSeeting WiFi Transmit Power");
//   }
    
//   // WiFi.setOutputPower(0);   //TODO: do we want this line in?

//   return true;
// }


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

  // for (size_t i = 0; i < smtp_session.sendingResult.size(); i++)
  // {
  //   /* Get the result item */
  //   SMTP_Result result = smtp_session.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp_session.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
      
  //   ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
  //   ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
  //   ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
  //   ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
  //   ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
  // }
  //TODO: we can remmove or edit the following line

  // You need to clear sending result as the memory usage will grow up.
  // smtp_session.sendingResult.clear();
} /* END smtpCallback*/


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
 * An app password was has been added to the 
 * clinton.debug@gmail.com account
 * thus allowing Gmail to be used.
 * 
 * The app password given was:  gvik uoit bdja grxu
 * 
 * 
 */
// void LAN::send_email ( void )  //TODO this is the function we want in
// {

//   SMTPSession smtp_session;

//   /* Declare the message class */
//   SMTP_Message message;  

//   /*  Set the network reconnection option */
//   MailClient.networkReconnect(true);

//   /** Enable the debug via Serial port
//    * 0 for no debugging
//    * 1 for basic level debugging
//    *
//    * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
//    * 
//    * TODO: do we want to put inclusion guards around this?
//    */
//   smtp_session.debug(1);

//   /* Callback function to get the Email sending status */
//   // void smtpCallback(SMTP_Status status); //TODO: just remove this?
  
//   /* Set the callback function to get the sending results */
//   // smtp_session.callback(smtpCallback);  //TODO: we might just get rid of this?

//   /* Declare the Session_Config for user defined session credentials */
//   Session_Config smtp_config;

//   /* Set the session smtp_config */
//   smtp_config.server.host_name = server;
//   smtp_config.server.port = SMTP_PORT;
//   smtp_config.login.email = buf_sender_email;
//   smtp_config.login.password = buf_for_email_password;
//   smtp_config.login.user_domain = "";


//   /**
//    * Set the NTP smtp_config time
//    * For times east of the Prime Meridian use 0-12
//    * For times west of the Prime Meridian add 12 to the offset.
//    * Ex. American/Denver GMT would be -6. 6 + 12 = 18
//    * See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
//    * 
//    * TODO: DO I NEED THIS?  
//    */
//   // smtp_config.time.ntp_server = "pool.ntp.org,time.nist.gov");
//   // smtp_config.time.gmt_offset = 3;
//   // smtp_config.time.day_light_offset = 0;



//   /* Set the message headers */
//   message.sender.name = "Hygrometer");
//   message.sender.email = buf_sender_email;
//   message.subject = "ESP Test Email");
//   message.addRecipient("CJG"), buf_recipient_email);


//   //Send raw text message
//   String textMsg = "Hello World! - Sent from ESP board";
//   message.text.content = textMsg.c_str();
//   message.text.charSet = "us-ascii";
//   message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

//   message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
//   message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


//   /* Connect to the server */
//   if (!smtp_session.connect(&smtp_config))
//   {
//     if(WIFI_LOGGING)
//     {
//         ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp_session.statusCode(), smtp_session.errorCode(), smtp_session.errorReason().c_str());
//     }
//     return;
//   }

//   if (!smtp_session.isLoggedIn() && WIFI_LOGGING)
//   {
//     Serial.println("\nNot yet logged in.");
//   }
//   else
//   {
//   if (smtp_session.isAuthenticated() && WIFI_LOGGING)
//       Serial.println("\nSuccessfully logged in.");
//   else
//       Serial.println("\nConnected with no Auth.");
//   }

//   //TODO worked up to heres
  
//   /* Start sending Email and close the session */
//   if (!MailClient.sendMail(&smtp_session, &message))
//   {
//     Serial.println("Error sending email.");
//     // ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp_session.statusCode(), smtp_session.errorCode(), smtp_session.errorReason().c_str());
//   }
// }




void LAN::send_email ( void )
{

  if(WIFI_LOGGING) {
    Serial.println("Now sending email.");
  }

  /* Declare the global used SMTPSession object for SMTP transport */
  SMTPSession smtp;

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;
  
  /* Declare the message class */
  SMTP_Message message;
  
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
  // smtp.callback(smtpCallback);

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  // config.time.ntp_server = "pool.ntp.org,time.nist.gov");
  // config.time.gmt_offset = 3;
  // config.time.day_light_offset = 0;


  /* Set the message headers */
  message.sender.name =  F("ESP");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP Test Email";
  message.addRecipient(F("CJG"), RECIPIENT_EMAIL);
    
  /*Send HTML message*/
  /*String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;*/

   
  //Send raw text message
  String textMsg = "Hello World! - Sent from HYGROMETER A01";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)){
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()){
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  //TODO the following is the original
  //TODO uncommenting the following will cause the SOM to reset.
  // if (!MailClient.sendMail(&smtp, &message))
  // {
  //   ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
  // }




} /* END SEND EMAIL*/



uint8_t LAN::eRcv( void )
{
  // uint8_t respCode;
  // uint8_t thisByte;
  // int loopCount = 0;

  // while (!client.available()) {
  //   delay(1);
  //   loopCount++;
  //   // if nothing received for 10 seconds, timeout
  //   if (loopCount > 10000) {
  //     client.stop();
  //     Serial.println("\r\nTimeout");
  //     return 0;
  //   }
  // }

  // respCode = client.peek();
  // while (client.available())
  // {
  //   thisByte = client.read();
  //   Serial.write(thisByte);
  // }

  // if (respCode >= '4')
  // {
  //   //  efail();
  //   return 0;
  // }
  return 1;
}



// void LAN::send_email ( void )
// {

//   Serial.println("Now sending email");

//   // WiFiConnect(buf_router_ssid, buf_router_password);
//   WiFiConnect("CJG_GbE_2G4", "GlockHK23");


//   // AssembleEmailMessage();

//   /**
//    * @brief Make a client connection
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Making client connection");
  
//   // if (client.connect(server, 2525) == 1) {
//   if (client.connect(server, SMTP_PORT) == 1) {
//       // if(WIFI_LOGGING)
//       Serial.println("connected");
      
//   } 
//   else 
//   {
//       // if(WIFI_LOGGING)
//     Serial.println("connection failed");
//   }
//   if (!eRcv()){
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error connecting to the client");
//     return;
//     // FlushSerialRXBuffer( );
//     // break;
//   }


//   /**
//    * @brief Sending EHLO command
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Sending EHLO");
  
//   client.println("EHLO www.example.com");
//   if (!eRcv())
//   {
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error sending EHLO command.");
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     return;
//     // break;
//   }
  

//   /**
//    * @brief Sending auth login command
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Sending auth login");
//   client.println("auth login");
  
//   if (!eRcv()){
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error sending AUTH LOGIN command.");
    
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     return;
//     // break;
//   }

  
//   /**
//    * @brief Send SMTP2GO User Account Credentials
//    */
//   // if(WIFI_LOGGING)
//   Serial.print("Sending Gmail B64 Username: ");
//   Serial.println(buf_b64_encoded_sender_email);
  
  
//   client.println(buf_b64_encoded_sender_email); //B64 encoded SMTP2GO username
//   if (!eRcv())
//   {
//       // if(WIFI_LOGGING)
//     Serial.print("\t*** Error sending Gmail Username: ");
//     Serial.println(buf_b64_encoded_sender_email);
//     return;
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     // break;
//   }


//   /**
//    * @brief Send SMTP2GO Password
//    * 
//    * THis should be the BASE64 password 
//    * for you SMTP2GO account
//    */
//   // if(WIFI_LOGGING)
//   Serial.print("Sending B64 SMTP2GO Password: ");
//   Serial.println(buf_b64_email_password);
  

//   client.println(buf_b64_email_password);  
//   if (!eRcv())
//   {
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error sending SMTP2GO password");
//     return;
      
//       // FlushSerialRXBuffer( );
//       // current_state = DEEP_SLEEP;
//       // break;
//   }
  

//   /**
//    * @brief Command for MAIL From:
//    * i.e.  --> client.println("MAIL From: clinton.debug@gmail.com"));
//    * 
//    */
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "MAIL From: ");
//   strcat(buf_temp, buf_sender_email);
//   client.println(buf_temp);

//   if (!eRcv())
//   {
//     // if(WIFI_LOGGING)
//     Serial.print("\t*** Error on command: ");
//     Serial.println(buf_temp);
//     return;
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     // break;
//   }
  

//   /**
//    * @brief Enter recipient address
//    * First, fill temp buffer with null characters
//    * i.e.  -->  client.println("RCPT To: clinton.guenther@gmail.com"));
//    * 
//    */
//   // if(WIFI_LOGGING)
//   Serial.print("Sending To: ");
//   Serial.println(buf_recipient_email);
  
  
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "RCPT To: ");
//   strcat(buf_temp, buf_recipient_email);
  
//   client.println(buf_temp);
//   if (!eRcv())
//   {
//     // if(WIFI_LOGGING)
//     Serial.print("\t*** Error on command: ");
//     Serial.println(buf_temp);
//     return;
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     // break;
//   }
  

//   /**
//    * @brief Send DATA command
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Sending DATA");
  

//   client.println("DATA");
//   if (!eRcv())
//   {
//       // if(WIFI_LOGGING)
//     Serial.println("\t*** Error on command \"DATA\".");
//     return;
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     // break;
//   }


//   /**
//    * @brief Sending To: command
//    * i.e.  --> client.println("To: clinton.guenther@gmail.com"));
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Sending email");
  
//   //  client.println("To:  clinton.guenther@gmail.com"));
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "To: ");
//   strcat(buf_temp, buf_recipient_email);
//   client.println(buf_temp);


//   /**
//    * @brief Sending From: command
//    * i.e. -->  client.println("From: clinton.debug@gmail.com"));
//    */
//   // client.println("From: clinton.debug@gmail.com");
//   memset(buf_temp, 0, TEMP_BUF_SIZE);
//   strcpy(buf_temp, "From: ");
//   strcat(buf_temp, buf_sender_email);
//   client.println(buf_temp);


//   /**
//    * @brief Send the subject
//    */
//   client.println("Subject: Hygrometer Health Report\r\n");
//   client.println("Just a test");
//   client.println(".");
//   if (!eRcv())
//   {
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error sending DOT to complete transaction");
//     return;
      
//       // FlushSerialRXBuffer( );
//       // current_state = DEEP_SLEEP;
//       // break;
//   }


//   /**
//    * @brief Sending QUIT
//    * 
//    */
//   // if(WIFI_LOGGING)
//   Serial.println("Sending QUIT");
  
              
//   client.println("QUIT");
//   if (!eRcv()){
//     // if(WIFI_LOGGING)
//     Serial.println("\t*** Error sending \"QUIT\".");
//     return;
      
//     // FlushSerialRXBuffer( );
//     // current_state = DEEP_SLEEP;
//     // break;
//   }
  
  
//   /**
//    * @brief Disconnecting 
//    */
//   client.stop();
  
//   // if(WIFI_LOGGING)
//   Serial.println("disconnected");
  

//   // current_state = DEEP_SLEEP;



  
// } /* END SEND EMAIL*/



