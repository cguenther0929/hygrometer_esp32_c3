/** 
 *  @filename   :   functions.ino
 *  @brief      :   Functions for Hygrometer sketch
 *  @author     :   Clinton Guenther
 *
*/

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





uint8_t user_option = 0x00;



void state_handler( void )
{
  //TODO: need to define this routine
  __asm__("nop\n\t");
}




