#include "i2c.h"
#include <Wire.h>

void I2C::init(void) {
    //TODO: this function is not supported.  Remove the NOP?
    __asm__("nop\n\t");

}

void I2C::set_io_expander(uint8_t io_num, bool level) 
{
    int temp_address    = 0x00;
    uint8_t current_value   = 0x00;
    uint8_t shift_value     = 0x00;
    uint8_t value_mask      = 0x00;
    
    /**
     * The io_num should never be 
     * greater than 16
     * 
     */
    if(io_num > 16) {
        io_num = 1;
    }


    /**
     * Set the address according to 
     * which IO needs to be set
     */
    (io_num > 8)?(temp_address = IOEXPANDER_7B_8_15_ADDRESS):(temp_address = IOEXPANDER_7B_0_7_ADDRESS);

    /**
     * The IO number shall be 
     * 0 though 7, and the address
     * will determine if we're in the 
     * upper deck (i.e. 8 though 15)
     */
    if(io_num > 8) {
        shift_value = io_num - 9;
    }
    else {
        shift_value = io_num - 1;
    }

    /* Determine the value_mask */
    value_mask = (0b00000001) << (shift_value);
    #if defined(ENABLE_LOGGING)
        Serial.print("Mask --> ");
        print8b_bin(value_mask);
    #endif


    /**
     * Set the value mask based 
     * on the set level
     * Flip the bits of the value mask if the 
     * bit of interest needs to be cleared
     */
    if(level == false) 
    {
        value_mask ^= 0xFF;  
    }
   
    /**
     * Perform a read/modify/write so only
     * the target value get modified 
     */
    Wire.beginTransmission(temp_address); 
    Wire.requestFrom(temp_address, 1);    // Request 1 byte from the address

    current_value = Wire.read();  

    #if defined(ENABLE_LOGGING)
        Serial.print("GPIO Read --> ");
        print8b_bin(current_value);
    #endif

    /**
     * Modify the value read from the
     * GPIO expander so the only the value
     * of interest is modified
    */
    if(level == false)
    {
        current_value = (value_mask) & current_value;
    }
    else 
    {
        current_value = (value_mask) | current_value;
    }

    /**
     * Write the modified IO value back 
     * to the IO expander 
    */
    Wire.write(current_value);        // The value has to be sent twice to guarantee a submittal
    Wire.endTransmission();


}

void I2C::toggle_io_expander(uint8_t io_num) 
{

    int temp_address    = 0x00;
    uint8_t current_value   = 0x00;
    uint8_t shift_value     = 0x00;
    uint8_t value_mask      = 0x00;
    
    /**
     * The io_num should never be 
     * greater than 16
     * 
     */
    if(io_num > 16) {
        io_num = 1;
    }

    /**
     * The IO number shall be 
     * 0 though 7, and the address
     * will determine if we're in the 
     * upper deck (i.e. 8 though 15)
     */
    if(io_num > 8) {
        shift_value = io_num - 9;
    }
    else {
        shift_value = io_num - 1;
    }

    /**
     * Set the address according to 
     * which IO needs to be set
     */
    (io_num > 8)?(temp_address = IOEXPANDER_7B_8_15_ADDRESS):(temp_address = IOEXPANDER_7B_0_7_ADDRESS);

    /* Determine the value_mask */
    value_mask = (0b00000001) << (shift_value);

    /**
     * Perform a read/modify/write so only
     * the target value get modified 
     */
    Wire.beginTransmission(temp_address); 
    Wire.requestFrom(temp_address, 1);    // Request 1 byte from the address

    current_value = Wire.read();  

    #if defined(ENABLE_LOGGING)
        Serial.print("GPIO Read --> ");
        print8b_bin(current_value);
    #endif

    /**
     * Modify the value read from the
     * GPIO expander so the only the value
     * of interest is modified
    */
    current_value = (value_mask) ^ current_value;

    /**
     * Write the modified IO value back 
     * to the IO expander 
    */
    Wire.write(current_value);        // The value has to be sent twice to guarantee a submittal
    Wire.endTransmission();



}

void I2C::choose_sensor(int sensor_number){
    //TODO: need to define what to write here...
    this -> sensor_number = sensor_number;

    if(sensor_number == 1) {
        mux_data = 0xAA;
        __asm__("nop\n\t"); //TODO: need to set the sensor's address accordingly
    }
    else {
        mux_data = 0xFF;
        __asm__("nop\n\t"); //TODO: need to set the sensor's address accordingly
    }

    Wire.beginTransmission(0xFF); 
    Wire.write(0xFF);        
    Wire.endTransmission();
}

int I2C::get_humidity() {
    //TODO: need to define what to write here...
    
    Wire.beginTransmission(0xFF); 
    Wire.write(0xFF);        
    Wire.endTransmission();
    return 0xFF;

}

int I2C::get_temperature() {
    //TODO: need to define what to write here...
    
    Wire.beginTransmission(0xFF); 
    Wire.write(0xFF);        
    Wire.endTransmission();
    return 0xFF;

}