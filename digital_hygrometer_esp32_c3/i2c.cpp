#include "i2c.h"
#include "app.h"
#include <Wire.h>

#define I2C_SDA     8
#define I2C_SCL     9

APP     i2c_app_functions;
NVM     i2c_nvm_functions; //TODO I think we can remove this

I2C::I2C() : _batt_sen_address(BQ72441_I2C_ADDRESS), _batt_sen_seal_flag(false), _batt_sen_usr_ctrl(false)
{
    /* THIS IS THE CONSTRUCTOR FOR THIS CLASS */
}

void I2C::init( void ) {
    //TODO Can we remove any of these lines?  How about move them to the constructor?  

    //TODO: Do we want to modify these lines?
    //TODO: for example, do we instead want to grab 
    //TODO: rhoffset values from NVM
    this -> sensor_number = 0;
    this -> rhoffset_1 = 0;
    this -> rhoffset_2 = 0;
    
    // The following is required to enable I2C lines
    Wire.begin(I2C_SDA, I2C_SCL);

}

/**
 * the following is related to
 * the Fuel Gauge
 * This function is to determine if the 
 * battery IC is valid
 */
bool I2C::batt_sen_is_valid(void)
{
	uint16_t deviceID = 0;
	
	deviceID = get_batt_sen_id(); // Read get_batt_sen_id from BQ27441
	
	if (deviceID == BQ27441_DEVICE_ID)
	{
		return true; // If device ID is valid, return true
	}
	
	return false; // Otherwise return false
}

// Read the device type - should be 0x0421
uint16_t I2C::get_batt_sen_id(void)
{
	return get_batt_sen_ctrl_word(BQ27441_CONTROL_DEVICE_TYPE);
}

uint16_t I2C::get_batt_sen_ctrl_word(uint16_t function)
{
	uint8_t subCommandMSB = (function >> 8);
	uint8_t subCommandLSB = (function & 0x00FF);
	uint8_t command[2] = {subCommandLSB, subCommandMSB};
	uint8_t data[2] = {0, 0};
	
	batt_sen_write_bytes((uint8_t) 0, command, 2);
	
	if (batt_sen_read_bytes((uint8_t) 0, data, 2))
	{
		return ((uint16_t)data[1] << 8) | data[0];
	}
	
	return false;
}

uint16_t I2C::batt_sen_write_bytes(uint8_t subAddress, uint8_t * src, uint8_t count)
{
	Wire.beginTransmission(_batt_sen_address);
	Wire.write(subAddress);
	for (int i=0; i<count; i++)
	{
		Wire.write(src[i]);
	}	
	Wire.endTransmission(true);
	
	return true;	
}

uint16_t I2C::batt_sen_read_bytes(uint8_t subAddress, uint8_t * dest, uint8_t count)
{
	int16_t timeout = BQ72441_I2C_TIMEOUT;	
	Wire.beginTransmission(_batt_sen_address);
	Wire.write(subAddress);
	Wire.endTransmission(true);
	
	Wire.requestFrom(_batt_sen_address, count);
	
	for (int i=0; i<count; i++)
	{
		dest[i] = Wire.read();
	}
	
	return timeout;
}

bool I2C::batt_sen_set_capacity(uint16_t capacity)
{
	// Write to STATE subclass (82) of BQ27441 extended memory.
	// Offset 0x0A (10)
	// Design capacity is a 2-byte piece of data - MSB first
	// Unit: mAh
	uint8_t capMSB = capacity >> 8;
	uint8_t capLSB = capacity & 0x00FF;
	uint8_t capacityData[2] = {capMSB, capLSB};
	return batt_sen_write_ext_data(BQ27441_ID_STATE, 10, capacityData, 2);
}

bool I2C::batt_sen_write_ext_data(uint8_t classID, uint8_t offset, uint8_t * data, uint8_t len)
{
	if (len > 32)
		return false;
	
	if (!_batt_sen_usr_ctrl)
    {
        batt_sen_enter_config();
    } 
	
	if (!batt_sen_block_data_control()) // // enable block data memory control
		return false; // Return false if enable fails
	if (!batt_sen_block_data_class(classID)) // Write class ID using DataBlockClass()
		return false;
	
	batt_sen_block_data_offset(offset / 32); // Write 32-bit block offset (usually 0)
	batt_sen_compute_checksum(); // Compute checksum going in
	uint8_t oldCsum = batt_sen_get_block_checksum();

	// Write data bytes:
	for (int i = 0; i < len; i++)
	{
		// Write to offset, mod 32 if offset is greater than 32
		// The blockDataOffset above sets the 32-bit block
		batt_sen_write_block_data((offset % 32) + i, data[i]);
	}
	
	// Write new checksum using BlockDataChecksum (0x60)
	uint8_t newCsum = batt_sen_compute_checksum(); // Compute the new checksum
	batt_sen_write_block_checksum(newCsum);

	if (!_batt_sen_usr_ctrl) batt_sen_exit_config(false);
	
	return true;
}

bool I2C::batt_sen_enter_config( void )
{
	// if (userControl) _batt_sen_usr_ctrl = true;
	_batt_sen_usr_ctrl = true;
	
	if (batt_sen_sealed())
	{
		_batt_sen_seal_flag = true;
		batt_sen_unseal(); // Must be unsealed before making changes
	}
	
	if (batt_sen_exe_control_word(BQ27441_CONTROL_SET_CFGUPDATE))
	{
		int16_t timeout = BQ72441_I2C_TIMEOUT;
		while ((timeout--) && (!(batt_sen_get_flags() & BQ27441_FLAG_CFGUPMODE)))
			delay(1);
		
		if (timeout > 0)
			return true;
	}
	
	return false;
}

// Issue a DataClass() command to set the data class to be accessed
bool I2C::batt_sen_block_data_class(uint8_t id)
{
	return batt_sen_write_bytes(BQ27441_EXTENDED_DATACLASS, &id, 1);
}

bool I2C::batt_sen_block_data_control(void)
{
	uint8_t enableByte = 0x00;
	return batt_sen_write_bytes(BQ27441_EXTENDED_CONTROL, &enableByte, 1);
}

bool I2C::batt_sen_sealed(void)
{
	uint16_t stat = batt_sen_status();
	return stat & BQ27441_STATUS_SS;
}

// Read the CONTROL_STATUS subcommand of control()
uint16_t I2C::batt_sen_status(void)
{
	return batt_sen_read_ctrl_word(BQ27441_CONTROL_STATUS);
}

uint16_t I2C::batt_sen_read_ctrl_word(uint16_t function)
{
	uint8_t subCommandMSB = (function >> 8);
	uint8_t subCommandLSB = (function & 0x00FF);
	uint8_t command[2] = {subCommandLSB, subCommandMSB};
	uint8_t data[2] = {0, 0};
	
	batt_sen_write_bytes((uint8_t) 0, command, 2);
	
	if (batt_sen_read_bytes((uint8_t) 0, data, 2))
	{
		return ((uint16_t)data[1] << 8) | data[0];
	}
	
	return false;
}

bool I2C::batt_sen_exit_config(bool resim)
{
	// There are two methods for exiting config mode:
	//    1. Execute the EXIT_CFGUPDATE command
	//    2. Execute the SOFT_RESET command
	// EXIT_CFGUPDATE exits config mode _without_ an OCV (open-circuit voltage)
	// measurement, and without resimulating to update unfiltered-SoC and SoC.
	// If a new OCV measurement or resimulation is desired, SOFT_RESET or
	// EXIT_RESIM should be used to exit config mode.
	if (resim)
	{
		if (batt_sen_soft_reset())
		{
			int16_t timeout = BQ72441_I2C_TIMEOUT;
			while ((timeout--) && ((batt_sen_get_flags() & BQ27441_FLAG_CFGUPMODE)))
				delay(1);
			if (timeout > 0)
			{
				if (_batt_sen_seal_flag) batt_sen_seal(); // Seal back up if we IC was sealed coming in
				return true;
			}
		}
		return false;
	}
	else
	{
		return batt_sen_exe_control_word(BQ27441_CONTROL_EXIT_CFGUPDATE);
	}	
}

bool I2C::batt_sen_block_data_offset(uint8_t offset)
{
	return batt_sen_write_bytes(BQ27441_EXTENDED_DATABLOCK, &offset, 1);
}

// Read all 32 bytes of the loaded extended data and compute a 
// checksum based on the values.
uint8_t I2C::batt_sen_compute_checksum(void)
{
	uint8_t data[32];
	batt_sen_read_bytes(BQ27441_EXTENDED_BLOCKDATA, data, 32);

	uint8_t csum = 0;
	for (int i=0; i<32; i++)
	{
		csum += data[i];
	}
	csum = 255 - csum;
	
	return csum;
}

uint8_t I2C::batt_sen_get_block_checksum(void)
{
	uint8_t csum;
	batt_sen_read_bytes(BQ27441_EXTENDED_CHECKSUM, &csum, 1);
	return csum;
}

// Use BlockData() to write a byte to an offset of the loaded data
bool I2C::batt_sen_write_block_data(uint8_t offset, uint8_t data)
{
	uint8_t address = offset + BQ27441_EXTENDED_BLOCKDATA;
	return batt_sen_write_bytes(address, &data, 1);
}

// UNseal the LIPO-G1A
bool I2C::batt_sen_unseal(void)
{
	// To unseal the BQ27441, write the key to the control
	// command. Then immediately write the same key to control again.
	if (batt_sen_read_ctrl_word(BQ27441_UNSEAL_KEY))
	{
		return batt_sen_read_ctrl_word(BQ27441_UNSEAL_KEY);
	}
	return false;
}

// Execute a subcommand() from the BQ27441-G1A's control()
bool I2C::batt_sen_exe_control_word(uint16_t function)
{
	uint8_t subCommandMSB = (function >> 8);
	uint8_t subCommandLSB = (function & 0x00FF);
	uint8_t command[2] = {subCommandLSB, subCommandMSB};
	uint8_t data[2] = {0, 0};
	
	if (batt_sen_write_bytes((uint8_t) 0, command, 2))
		return true;
	
	return false;
}

// Issue a soft-reset to the BQ27441-G1A
bool I2C::batt_sen_soft_reset(void)
{
	return batt_sen_exe_control_word(BQ27441_CONTROL_SOFT_RESET);
}

// Read the flags() command
uint16_t I2C::batt_sen_get_flags(void)
{
	return batt_sen_read_word(BQ27441_COMMAND_FLAGS);
}

// Read a 16-bit command word from the BQ27441-G1A
uint16_t I2C::batt_sen_read_word(uint16_t subAddress)
{
	uint8_t data[2];
	batt_sen_read_bytes(subAddress, data, 2);
	return ((uint16_t) data[1] << 8) | data[0];
}

// Seal the BQ2742
bool I2C::batt_sen_seal(void)
{
	return batt_sen_read_ctrl_word(BQ27441_CONTROL_SEALED);
}

// Use the BlockDataCheckSum() command to write a checksum value
bool I2C::batt_sen_write_block_checksum(uint8_t csum)
{
	return batt_sen_write_bytes(BQ27441_EXTENDED_CHECKSUM, &csum, 1);	
}

uint16_t I2C::batt_sen_soc(soc_measure type)
{
	uint16_t socRet = 0;
	switch (type)
	{
	case FILTERED:
		socRet = batt_sen_read_word(BQ27441_COMMAND_SOC);
		break;
	case UNFILTERED:
		socRet = batt_sen_read_word(BQ27441_COMMAND_SOC_UNFL);
		break;
	}
	
	return socRet;
}

/*******************************************************************
 * The following functions are for 
 * the GPIO expander
 ****************************************************************/

void I2C::print8b_bin(uint8_t aByte)
 {
    for (int8_t aBit = 7; aBit >= 0; aBit--)
    {
        Serial.print(bitRead(aByte, aBit) ? '1' : '0');
    }
    
    Serial.print('\n');
}

void I2C::print16b_bin(uint16_t aByte)
 {
    for (int16_t aBit = 15; aBit >= 0; aBit--)
    {
        Serial.print(bitRead(aByte, aBit) ? '1' : '0');
    }
    
    Serial.print('\n');
}

//TODO need to remove
// float I2C::new_battery_health( void ){

// }

// void I2C::io_set_o_port_to_inputs( void )
// {

//     /**
//      * Start the transaction with the 
//      * IO expander -- the "O" ports are
//      * defined as [8:15]
//      */
//     Wire.beginTransmission(IOEXPANDER_7B_8_15_ADDRESS); 
    
//     /**
//      * Set all outputs to high
//      * in order to set these IO to 
//      * inputs. 
//     */
//    Wire.write(0xFF);        
//    Wire.endTransmission();

// }

void I2C::set_io_expander(uint8_t io_num, bool level) 
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
        if(temp_address = IOEXPANDER_7B_0_7_ADDRESS )
        {
            Serial.print("^[7:0] GPIO Read --> ");
            print8b_bin(current_value);
            Serial.println(" ");
        }
        else
        {
            Serial.print("^[15:8] GPIO Read --> ");
            print8b_bin(current_value);
            Serial.println(" ");
        }
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
void I2C::get_sensor_data( Preferences & pref ) 
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
    delay(300);

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
        this -> temp_offset = i2c_nvm_functions.nvm_get_float(pref,PREF_TEMP_OFFSET1);
        
        if(this -> sensor_number == 1)
        {

            
            this -> temp_val1 =  (float)(temp_uint16t/207.1983 - 52.33);  //For Deg F
            this -> temp_val1 -= (float)(this -> temp_offset); 
            this -> temp_val1 =  (char)(this -> temp_val1);  
        }
        else 
        {
            
            this -> temp_val2 =  (float)(temp_uint16t/207.1983 - 52.33);  // For deg F
            this -> temp_val2 -=  (float)(this -> temp_offset);  
            this -> temp_val2 =  (char)(this -> temp_val2); 
        }
    }

    i2c_app_functions.sensor_power_off();
    
}