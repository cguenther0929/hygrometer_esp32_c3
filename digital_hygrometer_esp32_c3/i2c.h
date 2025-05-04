#ifndef HYGROMETERI2C_H
#define HYGROMETERI2C_H

#include <Arduino.h>    //This likely defines wire.h
#include <Preferences.h>
#include "bq2742.h"


/**
 * Note, for I2C addresses, 
 * only the base address should be considered
 * as the Wire function will automatically append
 * the necessary R/#W bit
 * 
 * It needs to be realized that the arduino left shifts
 * the address and then appends the R/#W bit.  Therefore,
 * the address shall first be right shifted (thus removing
 * the R/#W bit) when recordering the address below.  For 
 * example, the fuel gauge used in this project is addressed
 * via 0x1010101 + R/#W, but the address that shall be passed
 * to the arduino function can be defined as 0101_0101 or
 * 0x55
 */
#define IOEXPANDER_7B_0_7_ADDRESS           0x6D
#define IOEXPANDER_7B_8_15_ADDRESS          0x5D

/**
 * Fuel gauge
 */
#define FUEL_GAUGE_ADDRESS                  0x55

/**
 * Defines for I2C mux
 */
#define I2C_MUX_ADDRESS                     0x70
#define I2C_DISABLE_ALL_MUX_CHANNELS        0x00
#define I2C_MUX_DISABLE_ALL                 0x00        
#define I2C_MUX_CH0_SELECT                  0x01        
#define I2C_MUX_CH1_SELECT                  0x02     

/**
 * Sensor parameters 
 */
#define SI7020_BASE_ADDRESS             0x40       // This is right-shifted before divulging the address

#define SI7020_MEAS_HUM_HOLD_MASTER     0xE5        // Allows clock stretching
#define SI7020_MEAS_HUM_NO_HOLD         0xF5
#define SI7020_TEMP_AFTER_HUM           0xE0
#define SI7020_MEAS_TMP_HOLD_MASTER     0xE3        // Allows clock stretching
#define SI7020_MEAS_TMP_NO_HOLD         0xF3

#define SI7020_RESET_COMMAND            0xFE
#define SI7020_WRT_USR_REG1             0xE6
#define SI7020_READ_USR_REG1            0xE7

#define SI7020_WRT_HEATER_CTRL_REG      0x51
#define SI7020_READ_HEATER_CTRL_REG     0x11

#define SI7020_ID_BYTE1_1               0xFA            // Reading the ID byte require the CRC byte be read!  p19/35 in datasheet
#define SI7020_ID_BYTE1_2               0x0F

#define SI7020_ID_BYTE2_1               0xFC
#define SI7020_ID_BYTE2_2               0xC9

#define SI7020_READ_FW_VER_1            0x84
#define SI7020_READ_FW_VER_2            0xB8

#define I2C_WRITE_BIT                   0x00            // OR address with 0x00 to write
#define I2C_READ_BIT                    0x01            // OR address with 0x01 to read

#define SI7020_HTRE_3D09MA              0x00            // Heater control register on p26/38 -- Heater bits are D3:D0 (lowest nibble)
#define SI7020_HTRE_9D18MA              0x01
#define SI7020_HTRE_15D24MA             0x02
#define SI7020_HTRE_27D37MA             0x04
#define SI7020_HTRE_51D69MA             0x08
#define SI7020_HTRE_94D20MA             0x0F

// Parameters for the batt_sen_soc() function
typedef enum {
	FILTERED,  // State of Charge Filtered (DEFAULT)
	UNFILTERED // State of Charge Unfiltered
} soc_measure;

class I2C {
    private:
        uint8_t sensor_number   = 0;
        uint8_t _batt_sen_address;  // Stores the BQ27441-G1A's I2C address
	    bool _batt_sen_seal_flag; // Global to identify that IC was previously sealed
	    bool _batt_sen_usr_ctrl; // Global to identify that user has control over 
	                         // entering/exiting config

    public:
        
        float    hum_val1           = 0.0;
        float    temp_val1          = 0.0;
        float    hum_val2           = 0.0;
        float    temp_val2          = 0.0;
        float    temp_offset        = 0.0;
        float    rh_offset          = 0.0;

        /**
         * @brief I2C constructor function
         * @param \p none 
         * @return nothing
         */
        I2C();  
        
        /**
         * @brief I2C init function
         * @param \p none 
         * @return nothing
         */
        void init( void );
        
        /**
         * @brief Determine if the battery sensor is valid
         * @param \p none 
         * @return True or False
         */
        bool batt_sen_is_valid( void );
        
        /**
         * @brief Get the ID of the battery sensor
         * @param \p none 
         * @return Battery sensor's UINT16 ID 
         */
        uint16_t get_batt_sen_id(void );
        
        /**
         * @brief Get the control word of the battery sensor
         * @param \p none 
         * @return Battery sensor's UINT16 control word
         */
        uint16_t get_batt_sen_ctrl_word( uint16_t function );
        
        /**
         * @brief Wrapper function for sending bytes to the battery sensor
         * @param \p Address \p Data Array \p Number of bytes to send  
         * @return True if successful
         */
        bool batt_sen_write_bytes( uint8_t subAddress, uint8_t * src, uint8_t count );
        
        /**
         * @brief Wrapper function for reading bytes from the battery sensor
         * @param \p Address \p Data Array \p Number of bytes to read  
         * @return True if successful
         */
        bool batt_sen_read_bytes( uint8_t subAddress, uint8_t * dest, uint8_t count );
        
        /**
         * @brief Set the capacity of the battery
         * @param \p Capacity 
         * @return True if successful
         */
        bool batt_sen_set_capacity( uint16_t capacity );
        
        /**
         * @brief Write extended data to the fuel gauge
         * @param \p Class ID \p Data Offset \p Poiter to Data \p Length of data  
         * @return True if successful
         */
        bool batt_sen_write_ext_data( uint8_t classID, uint8_t offset, uint8_t * data, uint8_t len );
        
        /**
         * @brief Enter into the configuration mode of the battery
         * @param \p none 
         * @return True if successful
         */
        bool batt_sen_enter_config( void );
        
        /**
         * @brief Tell the fuel gauge which class we wish to communicate with
         * @param \p Class ID 
         * @return True if successful
         */
        bool batt_sen_block_data_class( uint8_t id );
        
        /**
         * @brief Enable writing block data to the fuel gauge
         * @param \p none 
         * @return True if successful
         */
        bool batt_sen_block_data_control( void );
        
        /**
         * @brief Seal the battery sensor
         * @param \p none 
         * @return True if successful
         */
        bool batt_sen_sealed(void);
        
        /**
         * @brief Get the status word from the fuel gauge
         * @param \p none 
         * @return Status word
         */
        uint16_t batt_sen_status(void);
        
        /**
         * @brief Read the contorl word from the fuel gauge
         * @param \p Function -- type of status to read 
         * @return Control Word (which one is defined by user)
         */
        uint16_t batt_sen_read_ctrl_word(uint16_t function);
        
        /**
         * @brief Write extended control word to the fuel gauge.  
         * @param \p Function -- type of status to write 
         * @return True if successful
         */
        bool batt_sen_exe_control_word( uint16_t function );
        
        /**
         * @brief Exit the configuration mode of the battery sensor
         * @param \p none 
         * @return True if successful
         */
        bool batt_sen_exit_config( bool resim = true );
        
        /**
         * @brief Define the offset amount before writing to the fuel gauge
         * @param \p Offset -- data offset value 
         * @return True if successful
         */
        bool batt_sen_block_data_offset(uint8_t offset);
        
        /**
         * @brief Compute checksum of data that is to be written to the fuel gauge
         * @param \p none 
         * @return checksum 
         */
        uint8_t batt_sen_compute_checksum(void);
        
        /**
         * @brief Compute checksum on a block of data within the fuel gauge
         * @param \p none 
         * @return checksum 
         */
        uint8_t batt_sen_get_block_checksum(void);
        
        /**
         * @brief Write a block of data to the fuel gauge
         * @param \p Offset \p data 
         * @return True if successful
         */
        bool batt_sen_write_block_data( uint8_t offset, uint8_t data );
        
        /**
         * @brief Unseal the fuel gauge so its memory can be written to
         * @param \p none
         * @return True if successful
         */
        bool batt_sen_unseal( void );
        
        /**
         * @brief Seal / protect the fuel gauge's memory 
         * @param \p none
         * @return True if successful
         */
        bool batt_sen_seal( void );
        
        /**
         * @brief Command the fuel gauge to perform a software reset 
         * @param \p none
         * @return True if successful
         */
        bool batt_sen_soft_reset( void );
        
        /**
         * @brief Ask the fuel gauge to report its status flags
         * @param \p none
         * @return Word containing status flags
         */
        uint16_t batt_sen_get_flags( void );
        
        /**
         * @brief Read a word of data from the fuel gauge
         * @param \p Address 
         * @return Word of data 
         */
        uint16_t batt_sen_read_word( uint16_t subAddress );
        
        /**
         * @brief Write the block checksum to the fuel gauge
         * @param \p uint8_t checksum value 
         * @return True if successful
         */
        bool batt_sen_write_block_checksum( uint8_t csum );
        
        /**
         * @brief Ask the fuel gauge to report the stat-of-charge
         * @param \p Type of measurement to perfor 
         * @return uint16_t The Value of the State-of-Charge
         */
        uint16_t batt_sen_soc(soc_measure type);
        
        /**
         * @brief Set IO expander output pin to defined level 
         * @details The io_num value that is passed 
         * in shall be one-based.  
         * @param \p io_num is the number (1-16) of the IO to set
         * @param \p level is the level (low or high) to set the output to  
         * @return nothing
         */
        void set_io_expander (uint8_t io_num, bool level);
        
        /**
         * @brief Toggle IO expander
         * @details The io_num value that is passed 
         * in shall be one-based.  
         * @param \p io_num is the number (1-16) of the IO to set
         * @return nothing
         */
        void toggle_io_expander(uint8_t io_num); 

        /**
         * @brief Read inputs form the GPIO expander
         * @details One of the ports (eight bits) has been
         * designated for inputs.  It is these eight bits that we 
         * want to read.    
         * @return eight bits from the IO expander 
         */
        uint8_t read_io_expander( void );
        
        /**
         * @brief Determine if the battery is charging
         * @return True if we are charging 
         */
        bool charging_is_active( void );

        /**
         * @brief Determine HW revision 
         * @return The revision of HW 
         */
        uint8_t get_hw_revision( void );

        /**
         * @brief Set I2C mux channel 
         * @details 
         * 
         * The following describes the message format 
         * when sending a command to the PCA9546 bus
         * switch.  
         * 
         * There is only the one command register
         * thus there's no reason to address a 
         * particular register.  
         * 
         * The key for the following diagram is as follows
         * 
         *    S   : Start
         *    #W  : Active low write enable (0)
         *    ACK : I2C Bus Acknowledge 
         *    P: Stop condition
         * 
         *   +---+ +-------+ +----+ +----------------+
         *   | S +->Slave  +-> #W +-> ACK from Slave |
         *   +---+ |Address| +----+ +-------+--------+
         *         +-------+                |
         *                                  |
         *        +-------------------------+
         *        |
         *   +----v----+                    +-----------+
         *   | Write   | +----------------+ | Master    |
         *   | Command +-> ACK from Slave +-> Generates |
         *   | Byte    | +----------------+ | Stop      |
         *   +---------+                    +-----------+
         *
         *
         * @param[in] \p sensor_number (i.e. sen_1)
         * @return nothing 
         * 
         */ 
        void choose_sensor(uint8_t sensor_number);

        /**
         * @brief Disable the I2C multiplexer
         * @param \p none
         * @return nothing
         */
        void disable_mux(void);
        
        /**
         * @brief Get data from both sensors
         * @param \p Instance of Preferences
         * @return nothing
         */
        void get_sensor_data(Preferences & pref);
        
        /**
         * @brief Print uint8_t number (i.e. 10101010)
         * @param \p Byte to print
         * @return nothing
         */
        void print8b_bin(uint8_t aByte);
        
        /**
         * @brief Print uint16_t number (i.e. 1010101010101010)
         * @param \p Integer to print
         * @return nothing
         */
        void print16b_bin(uint16_t aByte);

};

#endif