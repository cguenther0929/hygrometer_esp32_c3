#ifndef HYGROMETERI2C_H
#define HYGROMETERI2C_H

#include <Arduino.h>    //This likely defines wire.h

// TODO: do we need to comment functions?

/**
 * Note, for I2C addresses, 
 * only the base address should be considered
 * as the Wire function will automatically add
 * the R/#W bit
 */
#define IOEXPANDER_7B_0_7_ADDRESS           0x68
#define IOEXPANDER_7B_8_15_ADDRESS          0x58
/**
 * Defines for I2C mux
 */
#define I2C_MUX_ADDRESS                     0xE0
#define I2C_DISABLE_ALL_MUX_CHANNELS        0x00
#define I2C_MUX_CH0_SELECT                  0x01        
#define I2C_MUX_CH1_SELECT                  0x02     

/**
 * Sensor parameters 
 */
#define SI7020_BASE_ADDRESS             0x80       

#define SI7020_MEAS_HUM_HOLD_MASTER     0xE5
#define SI7020_MEAS_NO_MASTER           0xF5
#define SI7020_MEAS_TMP_HOLD_MASTER     0xE3
#define SI7020_MEAS_TMP_NO_MASTER       0xF3
#define SI7020_MEAS_TMP_PREV_RH_MEAS    0xE0

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

class I2C {
    private:
        uint8_t sensor_number   = 0;
        uint8_t rhoffset_1      = 0;
        uint8_t rhoffset_2      = 0;

    public:
        /**
         * @brief I2C init function
         * @param \p none 
         * @return nothing
         */
        void init(void);

        /**
         * @brief Set IO expander output pin to defined level 
         * @details The io_num value that is passed 
         * in shall be one-based.  
         * @param \p io_num is the number (1-16) of the IO to set
         * @param \p level is the level (low or high) to set the output to  
         * @return nothing
         */
        void set_io_expander (uint8_t io_num, bool level);
        

        //TODO: need to comment
        void toggle_io_expander(uint8_t io_num); 
        

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
        void choose_sensor(int sensor_number);
        
        float get_humidity(void);

        float get_temperature(void);
};

#endif