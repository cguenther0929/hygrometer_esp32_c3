#include "i2c.h"
#include <Wire.h>

void I2C::init(void) {
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from EEPROM
    this -> sensor_number = 0;
    this -> rhoffset_1 = 0;
    this -> rhoffset_2 = 0;


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
    uint8_t channel_number = 0x00;
    this -> sensor_number = sensor_number;

    (sensor_number == 1) ? (channel_number = I2C_MUX_CH0_SELECT):(channel_number = I2C_MUX_CH1_SELECT);
    
    Wire.beginTransmission(I2C_MUX_ADDRESS); 
    Wire.write(channel_number);        
    Wire.endTransmission();
}

float I2C::get_humidity() {
    uint16_t    temp_uint16t    = 0x0000;  
    float       temp_float      = 0.0; 

    Wire.beginTransmission(SI7020_BASE_ADDRESS); 
    temp_uint16t = Wire.requestFrom(SI7020_MEAS_HUM_HOLD_MASTER, 1);    // Request 1 byte from the address  
    Wire.endTransmission();
    //TODO: need to define the offsets
    temp_float = (float)((125.0 * temp_uint16t / 65536) - 6 + 0);
    
    return temp_float;

}

float I2C::get_temperature() {
    uint16_t    temp_uint16t    = 0x0000;  
    float       temp_float      = 0.0; 

    Wire.beginTransmission(SI7020_BASE_ADDRESS); 
    temp_uint16t = Wire.requestFrom(SI7020_MEAS_TMP_PREV_RH_MEAS, 1);    // Request 1 byte from the address  
    Wire.endTransmission();
    
    /**
     * Value is in fahrenheit
     */
    temp_float = (uint8_t)((temp_uint16t / 207.1952) - 52.24); 
    
    return temp_float;

}