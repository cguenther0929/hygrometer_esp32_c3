#include "app.h"


void APP::init(void) 
{
    //TODO: need to define this function 
    __asm__("nop\n\t");
}

bool APP:: network_parameters_valid( void )
{

    network_info network_info;

    /**
     * If the first characters 
     * of the various network parameters are 
     * non-null, the routine will assume the parameters are 
     * valid
     */
    if (network_info.hyg_name[0] = NULL)
    {
        return false;
    }
    if (network_info.wifi_ssid[0] = NULL)
    {
        return false;
    }
    if (network_info.wifi_password[0] = NULL)
    {
        return false;
    }
    if (network_info.recipient_email_address[0] = NULL)
    {
        return false;
    }
    if (network_info.sender_email_address[0] = NULL)
    {
        return false;
    }
    return true;

}

