
struct SensorRegister {
    uint8_t sensorRegister;
    bool matches( uint8_t value, uint8_t mask );
    bool push( uint8_t value, uint8_t mask );
}


