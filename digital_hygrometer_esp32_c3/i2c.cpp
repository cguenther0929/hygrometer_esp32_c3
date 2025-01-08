#include "i2c.h"
#include <Wire.h>

#define I2C_SDA     8
#define I2C_SCL     9

void I2C::init(void) {
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from NVM
    this -> sensor_number = 0;
    this -> rhoffset_1 = 0;
    this -> rhoffset_2 = 0;

    // The following is required to enable I2C lines
    Wire.begin(I2C_SDA, I2C_SCL);


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
    
    //TODO: we can remove the following two lines
    // Serial.print("\t\t***DEBUG CHAN #: ");
    // Serial.println(channel_number);

    Wire.beginTransmission(I2C_MUX_ADDRESS); 
    Wire.write(channel_number);        
    Wire.endTransmission();
}

void I2C::disable_mux(void)
{
    Wire.beginTransmission(I2C_MUX_ADDRESS); 
    Wire.write(I2C_MUX_DISABLE_ALL);        
    Wire.endTransmission();
}

/**
 * The following function is responsible 
 * for collecting humidity data from 
 * the SI 7020 sensor.
 * 
 * This function is quite specific to how the 
 * SI 7020 sensor returns data. 
 * 
 * There are two modes in which data can be read 
 * back: a) hold master mode; and b) no-hold master 
 * mode.  The difference between the two, is that, 
 * in no-hold master mode, the process to read
 * back data is non-blocking, whereas in hold master
 * mode, there's a clock stretching mechanism introduced 
 * when reading back data.  For really time sensitive
 * applications, no-hold master mode should be 
 * considered.  More information about these modes 
 * can be ascertained by looking at datasheet p.20 of 35.
 * 
 * The key for the following diagram is as follows
 * 
 * S: Start
 * W: Write bit in address is set
 * A: Acknowledge
 * Sr: Repeated start condition 
 * R: Read bit in address is set
 * NAK: Not acknowledge
 * P: Stop condition
 *
 * 
 *       +---+ +-------+ +---+ +---+ +---------+
 *       | S +->Slave  +-> W +-> A +-> Measure |
 *       +---+ |Address| +---+ +---+ | Command |
 *             +-------+             +----+----+
 *                                        |
 *         +------------------------------+
 *         |
 *       +-v-+ +----+ +---------+ +---+ +---+
 *       | A +-> Sr +-> Slave   +-> R +-> A |
 *       +---+ +----+ | Address | +---+ +-+-+
 *                    +---------+         |
 *            +---------------------------+
 *            |
 *       +----v----+ +---------+ +---+ +---------+
 *       | Clock   +-> MS Byte +-> A +-> LS Byte |
 *       | Stretch | +---------+ +---+ +----+----+
 *       +---------+                        |
 *          +-----------OR------------------+
 *          |                               |
 *       +--v--+ +---+                      v
 *       | NAK +-> P |               Optional Checksum
 *       +-----+ +---+               Collection
 *                                   Datasheet p20/35
 *       
 *   
 */ 
float I2C::get_humidity() {
    uint8_t     lsb_byte        = 0x00;
    uint8_t     msb_byte        = 0x00;
    uint16_t    temp_uint16t    = 0x0000;  
    float       temp_float      = 0.0; 

    /**
     * Need to indicate to the sensor which 
     * register to read from 
     * 
    */
    Wire.beginTransmission(SI7020_BASE_ADDRESS); 
    Wire.write(SI7020_MEAS_HUM_HOLD_MASTER);

    /**
     * Now extract the data from the sensor 
     * requestFrom() will create the repeated start.  
    */
    Wire.requestFrom(SI7020_BASE_ADDRESS,2);    // Request 1 byte from the address  

    msb_byte = Wire.read(); 
    lsb_byte = Wire.read(); 
    temp_uint16t = (uint16_t)(msb_byte << 8) | (lsb_byte); 

    /**
     * Get ready of any remaining garbage
     **/
    while(Wire.available())
    {
        lsb_byte = Wire.read();
    }

    /**
     *  End the I2C transaction  
    */
    Wire.endTransmission();
    
    temp_float = (float)((125.0 * temp_uint16t / 65536) - 6.0 + 0.0);
    
    return temp_float;
}

/**
 * The following function is responsible 
 * for collecting temperature data from 
 * the SI 7020 sensor.
 * 
 * This function is quite specific to how the 
 * SI 7020 sensor returns data. 
 * 
 * There are two modes in which data can be read 
 * back: a) hold master mode; and b) no-hold master 
 * mode.  The difference between the two, is that, 
 * in no-hold master mode, the process to read
 * back data is non-blocking, whereas in hold master
 * mode, there's a clock stretching mechanism introduced 
 * when reading back data.  For really time sensitive
 * applications, no-hold master mode should be 
 * considered.  More information about these modes 
 * can be ascertained by looking at datasheet p.20 of 35.
 * 
 * The key for the following diagram is as follows
 * 
 * S: Start
 * W: Write bit in address is set
 * A: Acknowledge
 * Sr: Repeated start condition 
 * R: Read bit in address is set
 * NAK: Not acknowledge
 * P: Stop condition
 *
 * 
 *       +---+ +-------+ +---+ +---+ +---------+
 *       | S +->Slave  +-> W +-> A +-> Measure |
 *       +---+ |Address| +---+ +---+ | Command |
 *             +-------+             +----+----+
 *                                        |
 *         +------------------------------+
 *         |
 *       +-v-+ +----+ +---------+ +---+ +---+
 *       | A +-> Sr +-> Slave   +-> R +-> A |
 *       +---+ +----+ | Address | +---+ +-+-+
 *                    +---------+         |
 *            +---------------------------+
 *            |
 *       +----v----+ +---------+ +---+ +---------+
 *       | Clock   +-> MS Byte +-> A +-> LS Byte |
 *       | Stretch | +---------+ +---+ +----+----+
 *       +---------+                        |
 *          +-----------OR------------------+
 *          |                               |
 *       +--v--+ +---+                      v
 *       | NAK +-> P |               Optional Checksum
 *       +-----+ +---+               Collection
 *                                   Datasheet p20/35
 *       
 *   
 */ 

float I2C::get_temperature() {
    uint8_t     lsb_byte        = 0x00;
    uint8_t     msb_byte        = 0x00;
    uint16_t    temp_uint16t    = 0x0000;  
    float       temp_float      = 0.0; 

    /**
     * Need to indicate to the sensor which 
     * register to read from 
     * 
    */
    Wire.beginTransmission(SI7020_BASE_ADDRESS); 
    Wire.write(SI7020_MEAS_TMP_HOLD_MASTER);

    /**
     * Now extract the data from the sensor 
     * requestFrom() will create the repeated start.  
    */
    Wire.requestFrom(SI7020_BASE_ADDRESS,2);    // Request 1 byte from the address  

    msb_byte = Wire.read(); 
    lsb_byte = Wire.read(); 
    temp_uint16t = (uint16_t)(msb_byte << 8) | (lsb_byte); 

    /**
     * Get ready of any remaining garbage
     **/
    while(Wire.available())
    {
        lsb_byte = Wire.read();
    }

    /**
     *  End the I2C transaction  
    */
    Wire.endTransmission();
    
    /**
     * Value is in fahrenheit
     */
    temp_float = (float)((temp_uint16t / 207.1952) - 52.24); 
    
    return temp_float;
}