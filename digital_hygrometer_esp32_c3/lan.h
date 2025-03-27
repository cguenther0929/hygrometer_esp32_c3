#ifndef HYGROMETERLAN_H
#define HYGROMETERLAN_H

#include <Arduino.h>    //This likely defines wire.h
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "nvm.h"
#include "i2c.h"
#include "app.h"

// TODO: do we need to comment functions?

/**
 * WiFi and Email 
 * Related 
 */
#define WIFI_LOGGING            true        // Set to true if we want to see logs from the wifi/email routeins  
#define WIFI_CONNECT_TIMEOUT_S  5           //Timeout value in seconds
#define SMTP_HOST         "smtp.gmail.com"
#define SMTP_PORT         465

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
        bool email_enabled          = true;

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
        void send_email ( Preferences & pref );

};

#endif