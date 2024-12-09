#ifndef HYGROMETERLAN_H
#define HYGROMETERLAN_H

#include <Arduino.h>    //This likely defines wire.h
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// TODO: do we need to comment functions?

/**
 * Email related
 * 
 * Declare the global used SMTPSession 
 * object for SMTP transport 
 */
// SMTPSession smtp;

// char server[] = "smtp.gmail.com";  

// char buf_for_recipient_email[]      = "clinton.guenther@gmail.com";
// char buf_for_sender_email[]         = "clinton.guenther@gmail.com";
// char buf_for_email_password[]       = "TYgvik uoit bdja grxu";
// char buf_for_router_password[]      = "GlockHK23";
// char buf_for_router_ssid[]          = "CJG_GbE_2G4";

// char buf_for_hyg_name[]             = "CJG HYG";

/**
 * WiFi and Email 
 * Related 
 */
#define WIFI_CONNECT_TIMEOUT_S  5
#define WIFI_LOGGING            1         //TODO: do we want to comment this out? 



#define SMTP_PORT                   465

class LAN {
    // private:
    //     uint8_t user_selection      = 0;

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

        /**
         * @brief Callback function for sending email
         * @param \p none 
         * @return nothing
         */
        void smtpCallback(SMTP_Status status);



        // /**
        //  * @brief Console routine
        //  * @details Serves the CLI to the user
        //  * @param \p none 
        //  * @return nothing
        //  */
        // void console (void);

        // /**
        //  * @brief Get number input from user (0-255)
        //  * @param \p none 
        //  * @return users input
        //  */
        // uint8_t get_user_uint8t_value ( void );
        
};

#endif