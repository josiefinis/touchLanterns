/*
======================================================================================================================================================
                                      REGISTER
======================================================================================================================================================
*/
#include "Register.h"


#define HARDWARE_REGISTER_SIZE 16
Register::Register() 
{
    disableOutput();
}


void Register::reset( void ) 
// Clear everything.
{
    disableOutput();
    clearShiftRegister();
    copyToStorageRegister();
    enableOutput();
}


void Register::write( uint16_t word ) 
// Feed a word into the shift register and copy to the storage register.
{
    writeToShiftRegister(word);
    copyToStorageRegister();
}


void Register::writeToShiftRegister( uint16_t word ) 
// Feed word into shift register, least significant bit first.
{
    for ( uint8_t i = 0; i < HARDWARE_REGISTER_SIZE; i++ ) 
    {
        bool bit = word & 1;
        writeBit( bit );
        word >>= 1;
    }
}


void Register::copyToStorageRegister( void ) 
{ 
    storageRegisterClockPulse(); 
}


void Register::writeBit( bool bit ) 
// Write a single bit to the shift register, shifting previously written bits down one place.
{
    if ( bit ) 
    { 
        PORTB |= PIN_REGISTER_DS; 
    }
    else 
    { 
        PORTB &= ~PIN_REGISTER_DS; 
    }
    shiftRegisterClockPulse();
}


void Register::enableOutput( void ) 
{ 
    PORTB &= ~PIN_REGISTER_NOT_OE; 
}


void Register::disableOutput( void ) 
{ 
    PORTB |= PIN_REGISTER_NOT_OE;
}


void Register::clearShiftRegister( void ) 
{ 
    PORTB &= ~PIN_REGISTER_NOT_MR;
    PORTB |= PIN_REGISTER_NOT_MR;
}


void Register::shiftRegisterClockPulse( void ) 
{
    PORTB |= PIN_REGISTER_SH_CP;
    PORTB &= ~PIN_REGISTER_SH_CP;
}


void Register::storageRegisterClockPulse( void ) 
{
    PORTB |= PIN_REGISTER_ST_CP;
    PORTB &= ~PIN_REGISTER_ST_CP;
}
