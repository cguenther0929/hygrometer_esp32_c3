#include "i2c.h"
#include "app.h"
#include <Wire.h>

#define I2C_SDA     8
#define I2C_SCL     9

APP     i2c_app_functions;

void I2C::print8b_bin(uint8_t aByte)
 {
    for (int8_t aBit = 7; aBit >= 0; aBit--)
    {
        Serial.print(bitRead(aByte, aBit) ? '1' : '0');
    }
    
    Serial.print('\n');
}


void I2C::init(void) {
    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from NVM
    this -> sensor_number = 0;
    this -> rhoffset_1 = 0;
    this -> rhoffset_2 = 0;

    // The following is required to enable I2C lines
    Wire.begin(I2C_SDA, I2C_SCL);
    // Wire.setClock(10000); //TODO: should be able to remove


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
        Serial.print("^Mask --> ");
        print8b_bin(value_mask);
        Serial.println(" ");
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
        Serial.print("^GPIO Read --> ");
        print8b_bin(current_value);
        Serial.println(" ");
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

    int temp_address        = 0x00;
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
        Serial.print("^GPIO Read --> ");
        print8b_bin(current_value);
        Serial.println(" ");
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

uint8_t I2C::read_io_expander( void )
{
    uint8_t uint8_value = 0x00;

    /**
     * Perform a read/modify/write so only
     * the target value get modified 
     */
    // Wire.beginTransmission(IOEXPANDER_7B_8_15_ADDRESS); 
    Wire.requestFrom(IOEXPANDER_7B_8_15_ADDRESS, 1);    // Request 1 byte from the address

    uint8_value = Wire.read();

    Wire.endTransmission();

    #if defined(ENABLE_LOGGING)
        Serial.print("^GPIO Read --> ");
        print8b_bin(uint8_value);
        Serial.println(" ");
    #endif

    return (uint8_value);
}

bool I2C::charging_is_active( void )
{
    uint8_t uint8_value = 0x00;

    uint8_value = read_io_expander();

    uint8_value = (bool)(((uint8_value >> 3) & (0x01)) ^ 0x01); // Charging status is active low, so invert

    return uint8_value;
}

uint8_t I2C::get_hw_revision( void )
{
    uint8_t uint8_value = 0x00;

    uint8_value = read_io_expander();

    uint8_value = (uint8_t)(uint8_value & 0x07); // Low three bits are the HW revision

    return uint8_value;
}

void I2C::choose_sensor(uint8_t sensor_number)
{
    uint8_t channel_number = 0x00;
    this -> sensor_number = sensor_number;

    (sensor_number == 1) ? (channel_number = I2C_MUX_CH0_SELECT):(channel_number = I2C_MUX_CH1_SELECT);
    
    Wire.beginTransmission(I2C_MUX_ADDRESS); 
    Wire.write(channel_number); //Channel number is expected to be either one or two       
    Wire.endTransmission();
}

void I2C::disable_mux(void)
{
    Wire.beginTransmission(I2C_MUX_ADDRESS); 
    Wire.write(I2C_MUX_DISABLE_ALL);        
    Wire.endTransmission();
}

/**
 * A lot of effort was spent towards getting this to work.
 * In the end, the only thing that seemed to work (derived from an 
 * online example) was to read the sensor in NO-HOLD-MODE and insert
 * a read delay in this function.  This could be because the Arduino
 * Wire.h library does not support clock stretching.  
 * 
 * The following function will extract both temperature and 
 * humidity data from the SI 7020 sensor.
 * 
 * There are two modes in which data can be read 
 * back: a) hold master mode; and b) no-hold master 
 * mode.  The difference between the two, is that, 
 * in no-hold master mode, the process to read
 * back data is non-blocking, whereas in hold master
 * mode, there's a clock stretching mechanism introduced 
 * when reading back data.  For really time sensitive
 * applications, no-hold master mode should be 
 * considered, and we can "come back" and gra the data.
 * More information about these modes 
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
 * If reading the temperature just after taking 
 * a humidity measurement.  A person might do this 
 * for efficiency.  A temperature measurement has 
 * to be made when making a humidity measurement, so 
 * the data might as well be grabbed now.  
 * 
 *       +---+ +-------+ +---+ +---+ +---------+
 *       | S +->Slave  +-> W +-> A +->  0xE0   |
 *       +---+ |Address| +---+ +---+ +----+----+
 *             +-------+                  |
 *                                        |
 *         +------------------------------+
 *         |
 *       +-v-+ +----+ +---------+ +---+ +---+
 *       | A +-> Sr +-> Slave   +-> R +-> A |
 *       +---+ +----+ | Address | +---+ +-+-+
 *                    +---------+         |
 *            +---------------------------+
 *            |
 *       +---------+ +---+ +---------+
 *       | MS Byte +-> A +-> LS Byte |  
 *       +---------+ +---+ +----+----+
 *          |                  
 *       +--v--+ +---+         
 *       | NAK +-> P |          
 *       +-----+ +---+         
 *  
                         
 */ 
void I2C::get_sensor_data( void ) 
{
    uint8_t     lsb_byte        = 0x00;
    uint8_t     i               = 0;
    uint8_t     msb_byte        = 0x00;
    uint16_t    temp_uint16t    = 0x0000;  
    float       temp_float      = 0.0; 

    /**
     * Power the sensors on
     */
    i2c_app_functions.sensor_power_on();
    delay(50);

    for(i=1;i<=2;i++)
    {

        /**
         * Choose sensor #1
         */
        choose_sensor(i);

        /**
         * Start by measuring humidity...
         * Need to indicate to the sensor which 
         * register to read from 
         * 
        */
        Wire.beginTransmission(SI7020_BASE_ADDRESS); 
        Wire.write(SI7020_MEAS_HUM_NO_HOLD);
        Wire.endTransmission();
        delay(30);

        /**
         * Now extract the data from the sensor. 
         * requestFrom() will create the repeated start.  
        */
        Wire.requestFrom(SI7020_BASE_ADDRESS,2);    // Request 2 bytes from the address  

        msb_byte = Wire.read(); 
        lsb_byte = Wire.read(); 
        temp_uint16t = (uint16_t)(msb_byte << 8) | (lsb_byte); 


        /**
         * Assign the value to the correct member 
         * of the class 
         * i.e. sensor #1 or #2
         */
        if(this -> sensor_number == 1)
        {
            this -> hum_val1 =  (float)((125.0 * (uint16_t)temp_uint16t / 65536) - 6.0 + this -> rhoffset_1);
            this -> hum_val1 =  (char)(this -> hum_val1);
        }
        else 
        {
            this -> hum_val2 =  (float)((125.0 * (uint16_t)temp_uint16t / 65536) - 6.0 + this -> rhoffset_2);
            this -> hum_val2 =  (char)(this -> hum_val2);
        }
        
        /**
         * Get the temperature value that was 
         * automatically measured when measuring
         * humidity 
         * 
         * Need to indicate to the sensor which 
         * register to read from 
         * 
        */
        Wire.beginTransmission(SI7020_BASE_ADDRESS); 
        Wire.write(SI7020_TEMP_AFTER_HUM);
        Wire.endTransmission();

        /**
         * Now extract the data from the sensor. 
         * requestFrom() will create 
         * a repeated start condition on the bus.  
        */
        Wire.requestFrom(SI7020_BASE_ADDRESS,2);    // Request 2 byte from the address  

        msb_byte = Wire.read(); 
        lsb_byte = Wire.read(); 
        temp_uint16t = (uint16_t)(msb_byte << 8) | (lsb_byte); 

        /**
         * Assign the value to the correct member 
         * of the class 
         * i.e. humidity / temperature
         */
        if(this -> sensor_number == 1)
        {
            this -> temp_val1 =  (float)(temp_uint16t/207.1983 - 52.33);  //For Deg F
            this -> temp_val1 =  (char)(this -> temp_val1);  
        }
        else 
        {
            this -> temp_val2 =  (float)(temp_uint16t/207.1983 - 52.33);  // For deg F
            this -> temp_val2 =  (char)(this -> temp_val2); 
        }
    }

    i2c_app_functions.sensor_power_off();
    
}