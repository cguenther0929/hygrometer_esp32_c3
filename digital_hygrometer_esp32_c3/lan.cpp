#include "lan.h"

/**
 * Email related
 * 
 * Declare the global used SMTPSession 
 * object for SMTP transport 
 */
// SMTPSession smtp_session;

// WiFiClient Client;

char  lan_buffer[PREF_BUFF_ELEMENTS]             = {NULL};

NVM   lan_nvm;
LAN   lan_class;
I2C   lan_i2c;
APP   lan_app;



void LAN::init(void) 
{
    //TODO: Let's just put a print here with inclusion guards
    __asm__("nop\n\t");  //TODO: we need to do something different here

}

bool LAN::WiFiConnect( const char * ssid, const char * password ) 
{
 
  int i = 0;
  int timeout = (int)WIFI_CONNECT_TIMEOUT_S/0.5;
  
  
  if(WIFI_LOGGING) {
    Serial.println("Calling Wifi.begin().");
  }
  WiFi.begin(ssid, password);

  /**
   * Print diagnostic data
   * for WiFi if logging
   * is enabled
   */
  if(WIFI_LOGGING) {
    Serial.println(' ');
    Serial.print("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
    Serial.print("\tConnecting to SSID: "); Serial.println(ssid);
    Serial.print("\tSSID password: "); Serial.println(password);
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
  }
    
  // WiFi.setOutputPower(0);   //TODO: do we want this line in?

  return true;
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
 * An app password was has been added to the 
 * clinton.debug@gmail.com account
 * thus allowing Gmail to be used.
 * 
 * The app password given was:  gvik uoit bdja grxu
 * 
 * 
 */
void LAN::send_email ( Preferences & pref )
{


  lan_i2c.get_sensor_data(pref);  //TODO since the variables are now defined as static, I don't think we need this.  
  lan_app.get_battery_health();

  SMTPSession smtp; 


  if(WIFI_LOGGING) {
    Serial.println("Now sending email.");
  }

  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(1);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name   = SMTP_HOST;
  config.server.port        = SMTP_PORT;
  
  /**
   * Clear the temporary buffer
   */
  memset(lan_buffer, NULL, sizeof(lan_buffer));
  
  // config.login.email        = AUTHOR_EMAIL;
  lan_nvm.nvm_read_string(pref, PREF_EMAIL_AUTHOR_KEY, lan_buffer);
  config.login.email        = lan_buffer;
  
  
  /**
   * Clear the temporary buffer
   */
  memset(lan_buffer, NULL, sizeof(lan_buffer));
  // config.login.password     = AUTHOR_PASSWORD;
  lan_nvm.nvm_read_string(pref, PREF_EMAIL_AUTHOR_PASSWORD_KEY, lan_buffer);
  config.login.password     = lan_buffer;
  config.login.user_domain  = "";
  
  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
 config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
 config.time.gmt_offset = 3;
 config.time.day_light_offset = 0;
 
 /* Declare the message class */
 SMTP_Message message;
 
 
 /**
  * Clear the temporary buffer
  */
 memset(lan_buffer, NULL, sizeof(lan_buffer));
 lan_nvm.nvm_read_string(pref, PREF_EMAIL_AUTHOR_KEY, lan_buffer);
 
 /* Set the message headers */
 message.sender.name = F("Hygrometer Test");
 message.sender.email = lan_buffer;
 message.subject = F("ESP Test Email");
 
 /**
  * Clear the temporary buffer
  */
 memset(lan_buffer, NULL, sizeof(lan_buffer));
 lan_nvm.nvm_read_string(pref, PREF_EMAIL_RECIPIENT_KEY, lan_buffer);
 message.addRecipient(F("CJG"), lan_buffer);
 
 
 //Send raw text message
 String textMsg  = "";
 textMsg += "Humidity 1:" + String(lan_i2c.hum_val1) + " %\n";
 textMsg += "Humidity 2:" + String(lan_i2c.hum_val2) + " %\n";
 textMsg += "Temp 1:" + String(lan_i2c.temp_val1) + " F \n";
 textMsg += "Temp 2:" + String(lan_i2c.temp_val2) + " F \n";
 textMsg += "Battery:" + String(lan_app.battery_charge_percentage) + " %\n";

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
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());


} /* END SEND EMAIL*/
