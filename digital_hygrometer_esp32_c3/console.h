#ifndef HYGROMETERCONSOLE_H
#define HYGROMETERCONSOLE_H

#include <Arduino.h>    //This likely defines wire.h

// TODO: do we need to comment functions?

class CONSOLE {
    private:
        uint8_t user_selection      = 0;

    public:
        /**
         * @brief Console init function
         * @param \p none 
         * @return nothing
         */
        void init(void);

        /**
         * @brief Console routine
         * @details Serves the CLI to the user
         * @param \p none 
         * @return nothing
         */
        void console (void);

        /**
         * @brief Get number input from user (0-255)
         * @param \p none 
         * @return users input
         */
        uint8_t get_user_uint8t_value ( void );

        /**
         * @brief Flush serial input buffer 
         * @param \p none 
         * @return nothing
         */
        void flush_serial_input_buffer( void );
        
};

#endif