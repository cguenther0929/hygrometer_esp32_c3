#include "hygrometeri2c.h"
#include <Wire.h>

void Sensor::init(void) {
    //TODO: this function is not supported.  Remove the NOP?
    __asm__("nop\n\t");

}

void Sensor::choose_sensor(int sensor_number){
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

int Sensor::get_humidity() {
    //TODO: need to define what to write here...
    
    Wire.beginTransmission(0xFF); 
    Wire.write(0xFF);        
    Wire.endTransmission();
    return 0xFF;

}

int Sensor::get_temperature() {
    //TODO: need to define what to write here...
    
    Wire.beginTransmission(0xFF); 
    Wire.write(0xFF);        
    Wire.endTransmission();
    return 0xFF;

}