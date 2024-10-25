#ifndef HYGROMETERI2C_H
#define HYGROMETERI2C_H

#include <Arduino.h>
// #include <wire.h>

/**
 * Note, for I2C addresses, 
 * only the base address should be considered
 * as the Wire function will automatically add
 * the R/#W bit
 */
#define IOEXPANDER_7B_0_7_ADDRESS           0x68
#define IOEXPANDER_7B_8_15_ADDRESS          0x58

class I2C {
    private:
        int mux_data        = 0x00;
        int sensor_number   = 0;

    public:
        void init(void);

        /**
         * @brief Set IO expander output 
         * @details The io_num value that is passed 
         * in shall be one-based.  
         * @param \p io_num is the number (1-16) of the IO to set
         * @param \p level is the level (low or high) to set the output to  
         * @return nothing
         * 
         */
        void set_io_expander (uint8_t io_num, bool level);
        

        //TODO: need to comment
        void toggle_io_expander(uint8_t io_num); 
        

        /**
         * @brief Set mux so we are talking to the correct sensor
         * @details TODO:
         * @param[in] \p sensor_number (i.e. sen_1)
         * @return nothing 
         * 
         */
        void choose_sensor(int sensor_number);
        
        int get_humidity(void);

        int get_temperature(void);
};

#endif