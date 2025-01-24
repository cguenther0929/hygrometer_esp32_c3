/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/

#include <Arduino.h>    //This likely defines wire.h
#include <WiFi.h>
// #include <WifiClient.h> //TODO I don't think we need this
#include <ESP_Mail_Client.h>


#define WIFI_SSID         "CJG_GbE_2G4"
#define WIFI_PASSWORD     "GlockHK23"



// #define WIFI_SSID "CJG_GbE_2G4"
// #define WIFI_PASSWORD "GlockHK23"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
// #define SMTP_HOST "smtp.gmail.com"
// #define SMTP_PORT 465

//TODO: the following () lines need
//TODO: to be deleted completely

#define WIFI_LOGGING    true
#define WIFI_CONNECT_TIMEOUT_S        5

// String email_message = "";
// char server[] = "mail.smtp2go.com";  

bool WiFiConnect( const char * ssid, const char * password ) 
{
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();







  // if(WIFI_LOGGING) {
  //   Serial.println("Connecting to WIFI.");
  // }
  
  // int i = 0;
  // int timeout = (int)(WIFI_CONNECT_TIMEOUT_S/0.5);
  
  
  // if(WIFI_LOGGING) {
  //   Serial.println("Calling Wifi.begin().");
  // }
  // WiFi.begin(ssid, password);

  // /**
  //  * Print diagnostic data
  //  * for WiFi if logging
  //  * is enabled
  //  */
  // if(WIFI_LOGGING) {
  //   Serial.println("");
  //   Serial.println("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
  //   Serial.println("\tConnecting to SSID: "); Serial.println(ssid);
  //   Serial.println("\tSSID password: "); Serial.println(password);
  // }

  
  // if(WIFI_LOGGING) {
  //   Serial.println("\tWiFi Connecting\t");
  // }
  
  // // Wait for connection
  // while ((WiFi.status() != WL_CONNECTED) && i < timeout) {
  //   delay(500);
  //   i++;
  //   if(WIFI_LOGGING) {
  //     Serial.print('.');
  //   }
  // }
  
  // if(WIFI_LOGGING) {
  //   Serial.println("");
  // }

  // if(i == timeout){
  // #if defined(WIFI_LOGGING)
  //     Serial.println("\tWiFi Connection timeout!");
  //     return false;
  // }
  // #endif

  // if(WIFI_LOGGING) {
  //     Serial.println("\tWiFi connected!");
  //     Serial.println("\tMy local IP: ");
  //     Serial.println(WiFi.localIP());
  // }
    
  // // WiFi.setOutputPower(0);   //TODO: do we want this line in?

  return true;
}



uint8_t user_option = 0x00;



void state_handler( void )
{
  //TODO: need to define this routine
  __asm__("nop\n\t");
}




