#include "lan.h"

/**
 * Email related
 * 
 * Declare the global used SMTPSession 
 * object for SMTP transport 
 */
// SMTPSession smtp_session;

char server[] = "smtp.gmail.com";  

char buf_for_recipient_email[]      = "clinton.guenther@gmail.com";
char buf_for_sender_email[]         = "clinton.debug@gmail.com";
char buf_for_email_password[]       = "krrm ceex fwxm ubpk";
char buf_for_router_password[]      = "GlockHK23";
char buf_for_router_ssid[]          = "CJG_GbE_2G4";

char buf_for_hyg_name[]             = "CJG HYG";

void LAN::init(void) 
{
    //TODO: Lat's just put a print here with inclusion guards
    __asm__("nop\n\t");  //TODO: we need to do something different here

}


bool LAN::WiFiConnect( const char * ssid, const char * password ) 
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
    Serial.println("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
    Serial.println("\tConnecting to SSID: "); Serial.println(ssid);
    Serial.println("\tSSID password: "); Serial.println(password);
  }

  
  if(WIFI_LOGGING) {
    Serial.println("\tWiFi Connecting\t");
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
      Serial.println("\tMy local IP: ");
      Serial.println(WiFi.localIP());
      Serial.println("\tSeeting WiFi Transmit Power");
  }
    
  // WiFi.setOutputPower(0);   //TODO: do we want this line in?

  return true;
}


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
//   smtp_config.login.email = buf_for_sender_email;
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
//   // smtp_config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
//   // smtp_config.time.gmt_offset = 3;
//   // smtp_config.time.day_light_offset = 0;



//   /* Set the message headers */
//   message.sender.name = F("Hygrometer");
//   message.sender.email = buf_for_sender_email;
//   message.subject = F("ESP Test Email");
//   message.addRecipient(F("CJG"), buf_for_recipient_email);


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

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL      "clinton.debug@gmail.com"
#define AUTHOR_PASSWORD   "krrm ceex fwxm ubpk"
#define RECIPIENT_EMAIL "clinton.guenther@gmail.com"


void LAN::send_email ( void )
{



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
  // config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  // config.time.gmt_offset = 3;
  // config.time.day_light_offset = 0;


  /* Set the message headers */
  message.sender.name = F("ESP");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("ESP Test Email");
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
