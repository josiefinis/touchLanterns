#include "Global.h"

#include "Sensor.h"
#include "LanternCollection.h"
#include "PWMSignal.h"
#include "Register.h"


// Create class instances.
Register shiftRegister;         // Writes to SIPO register chip.
PWMSignal pwm;            // Generates a pulse width modulation signal for each lantern corresponding to its brightness.
Sensor sensor = Sensor();       // Loops over 16 capacitive touch sensors with multiplexer. Takes raw sensor input and outputs binary 'is touched' signal.
const uint16_t neighbourList[16] = 
{ 
  0x421 , 0x4320, 0x5310, 0x521 ,
  0x7610, 0x7632, 0xC854, 0xC854,
  0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
  0xED76, 0xFEC9, 0xFDC , 0xEDB 
};

LanternCollection lantern = LanternCollection( 16, neighbourList );


uint8_t idx;
uint32_t pwmPhase0 = micros();
uint16_t edgeAtMicros = 0xFFFF;
uint32_t lastBurnMillis = millis();
uint8_t startupCounter = 96;


bool pollSensor() {
// Poll capacitive sensor
  if ( startupCounter > 0 ) {                       // Ignore sensors briefly after start up while they settle and calibrate themselves.
    sensor.output();
    startupCounter--;
    return 0;
  }
  return sensor.output();
}


void updateLanterns() {
  lantern.pushSensor( idx, pollSensor() );
  lantern.update( idx );
  for ( uint8_t i=0; i<16; i++ ) {
      pwm.changeDuty( i, lantern.getBrightness( i ) );
  }
}


void burnLanterns() {
// Burn down all lit lanterns. 
  for ( uint8_t i=0; i<16; i++ ) {
    lantern.burnDown( i ); 
  }
}


void pwmCycle1stHalf() {
  pwm.periodStart();
  shiftRegister.writeToStorageRegister( pwm.getSignal() ); // TODO change to shorter name
  lantern.pushSensor( idx, pollSensor() );
  lantern.update( idx );
  for ( uint8_t i=0; i<16; i++ ) {
      pwm.changeDuty( i, lantern.getBrightness( i ) );
  }
  pwm.nextEdge();
  edgeAtMicros = pwm.getTime();
  idx = sensor.nextMuxChannel();
}


void pwmCycle2ndHalf() {
// Second half of PWM cycle
  shiftRegister.writeToStorageRegister( pwm.getSignal() );
  pwm.nextEdge();
  edgeAtMicros = pwm.getTime();
}


void initialisePins() {
  // Set pins I/O.
  DDRB |= PIN_REGISTER_SER | PIN_REGISTER_NOT_OE | PIN_REGISTER_NOT_SRCLR | PIN_REGISTER_RCLK | PIN_REGISTER_SRCLK;
  DDRD |= PIN_MUX_S0 | PIN_MUX_S1 | PIN_MUX_S2 | PIN_MUX_S3;
  pinMode(PIN_SENSOR_RECEIVE, INPUT);
  pinMode(PIN_SENSOR_SEND, OUTPUT);
}


void setup() {
  initialisePins();
  shiftRegister.reset();

  #if SERIAL_ON
    Serial.begin(9600);		
    while(!Serial) {}
  #endif
}


void loop() {  
  uint32_t currentMillis = millis();
  uint32_t currentMicros = micros();
  
  if ( currentMicros - pwmPhase0 >= edgeAtMicros ) {
    pwmCycle2ndHalf();
  }
  if ( currentMicros - pwmPhase0 >= PWM_PERIOD ) {
    // Mark start of PWM cycle here. For all signals with duty cycle greater than 50% set bit to 1, otherwise set to 0. 
    // The signal is phase shifted by 180 degrees when its duty cycle crosses 50%. This ensures that there is no pulse edge and therefore no need to update the register for the first half of a cycle. All sensor polling, updates and calculations are done during this time. Only register updates at pre calculated times are done during the second half of the cycle.
    pwmPhase0 = currentMicros;
    pwmCycle1stHalf();
  }

  if ( currentMillis - lastBurnMillis >= BURN_INTERVAL ) {
    lastBurnMillis = currentMillis;
    burnLanterns();
  }
}
