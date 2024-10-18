#ifndef HYGROMETERI2C_H
#define HYGROMETERI2C_H

#include <Arduino.h>
// #include <wire.h>


class Sensor {
    private:
        int mux_data        = 0x00;
        int sensor_number   = 0;

    public:
        void init(void);
        
        void choose_sensor(int sensor_number);
        
        int get_humidity(void);

        int get_temperature(void);
};

#endif