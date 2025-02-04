#ifndef HYGROMETERLAN_H
#define HYGROMETERLAN_H

#include <Arduino.h>    //This likely defines wire.h
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// TODO: do we need to comment functions?

/**
 * WiFi and Email 
 * Related 
 */
#define WIFI_LOGGING            true        // Set to true if we want to see logs from the wifi/email routeins  
#define WIFI_CONNECT_TIMEOUT_S  5           //Timeout value in seconds
#define SMTP_HOST         "smtp.gmail.com"
#define SMTP_PORT         465


#define PREF_EMAIL_AUTHOR_KEY           "auth"
#define PREF_EMAIL_RECIPIENT_KEY        "recipient"
#define PREF_EMAIL_AUTHOR_PASSWORD_KEY  "auth_pass"
#define PREF_WIFI_SSID                  "wifi_ssid"
#define PREF_WIFI_PASSWORD              "wifi_password"

#define PREF_CALIBRATION_STATUS_KEY     "cal_stat"
#define PREF_NETWORK_VALID_KEY          "net_valid"



// TODO Remove the following after capturing via preferences
#define AUTHOR_EMAIL      "clinton.debug@gmail.com"
#define AUTHOR_PASSWORD   "krrm ceex fwxm ubpk"

#define RECIPIENT_EMAIL   "clinton.guenther@gmail.com"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT                   465

#define WIFI_SSID         "CJG_GbE_2G4"  //TODO need to remove these once this is captured via PREFERENCES 
#define WIFI_PASSWORD     "GlockHK23"




class LAN {

    public:
        /**
         * @brief Wifi init function
         * @param \p none 
         * @return nothing
         */
        void init(void);

        /**
         * @brief Wifi connect function
         * @param \p none 
         * @return true or false 
         */
        bool WiFiConnect ( const char * ssid, const char * password  );

        /**
         * @brief Email sending function
         * @param \p none 
         * @return nothing
         */
        void send_email ( void );

};

#endif