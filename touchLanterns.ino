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
#if SERIAL_ON
uint32_t lastMonitorMillis = millis();
#endif
#if MONITOR_REGISTER_SIGNAL
uint8_t MONITORidx = 0;
uint16_t edgeAtMicrosMONITOR[32];
uint16_t signalMONITOR[32];
#endif


bool pollSensor() {
// Poll capacitive sensor
  if ( startupCounter > 0 ) {                       // Ignore sensors briefly after start up while they settle and calibrate themselves.
    sensor.output();
    startupCounter--;
    return 0;
  }
  return sensor.output();
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
  lantern.sense( idx, pollSensor() );
  lantern.update( idx );
  for ( uint8_t i=0; i<16; i++ ) {
    lantern.changeBrightness( i );
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

#if SERIAL_ON
void printIndices() {
  Serial.println();
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print( i, HEX ); Serial.print( "\t" );
  }
  Serial.println(); 
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print("========");
  }
  Serial.println();
}
#endif



#if SERIAL_ON
void printLantern( uint8_t idx ) {
  #if MONITOR_INPUT
    Serial.print( lantern.getInput( idx ), HEX ); 
  #endif
  #if MONITOR_STATE
    Serial.print( "s" ); 
    Serial.print( lantern.getState( idx ), HEX ); 
  #endif
  #if MONITOR_OUTPUT
    Serial.print( "o" ); 
    Serial.print( lantern.getOutput( idx ), HEX ); 
  #endif
}


void printLine() {
  bool chosenIndex = 0xF;
  bool all = 1;
  Serial.println();
  for ( uint8_t i = 0; i < 16; i++ ) { 
    if ( all ) { 
      uint8_t& idx = i;
      printLantern( idx );
    }
    if ( not all and i == 0 ) {
      uint8_t idx = chosenIndex;
      printLantern( idx );
    }
    #if MONITOR_BRIGHTNESS
      Serial.print( "b" ); 
      Serial.print( lantern.getBrightness( idx ) ); 
    #endif
    Serial.print( "\t" );
  }
}
#endif


#if MONITOR_PARENT
void printParent() {
  for ( uint8_t i=0; i<16; i++ ) { 
    if ( lantern.getParent( i ) ) {
      Serial.print( lantern[i].getParent()->getIndex(), HEX ); 
    }
    Serial.print( "\t" );
  }
  Serial.println();
}
#endif


#if MONITOR_DELAY
void printDelay() {
  Serial.println();
  for ( uint8_t i=0; i<16; i++ ) { Serial.print( lantern.getDelay( i ) ); Serial.print( " ds\t" ); }
}
#endif


#if MONITOR_REGISTER_SIGNAL
void printRegisterSignal() {
  Serial.print("\nInterval\tSignal\n"); 
  for ( uint8_t i=0; i<32; i++ ) {
    //Serial.print(microsMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\t");
    Serial.print(edgeAtMicrosMONITOR[MONITORidx & 0x1F]); Serial.print("\t");
    Serial.print(signalMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\n");
    MONITORidx++;
  }
}
#endif


#if MONITOR_TIMINGS
void printTimings() {
  Serial.print("Sensor input:\t"); 
  Serial.print(inputTimer.getMean()); Serial.print("\t"); 
  Serial.println(inputTimer.getMax());
  Serial.print("Lantern update:\t"); 
  Serial.print(updateTimer.getMean()); Serial.print("\t"); 
  Serial.println(updateTimer.getMax());
  Serial.print("Brightness:\t"); 
  Serial.print(brightnessTimer.getMean()); Serial.print("\t"); 
  Serial.println(brightnessTimer.getMax());
  Serial.println();
}
#endif

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
    #if MONITOR_REGISTER_SIGNAL
      for ( uint8_t i=0; i<16; i++ ) {
        edgeAtMicrosMONITOR[i] = 0;
        signalMONITOR[i] = 0;
        //microsMONITOR[i] = 0;
      }
    #endif
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

  #if SERIAL_ON
  if ( currentMillis - lastMonitorMillis >= MONITOR_INTERVAL ) {
    lastMonitorMillis = currentMillis;

    if ( millis() % (8 * MONITOR_INTERVAL) < MONITOR_INTERVAL ) {
      printIndices();
    }
    printLine();
    #if MONITOR_TIMINGS
    printTimings();
    #endif
    #if MONITOR_PARENT
    printParent();
    #endif
    #if MONITOR_DELAY
    printDelay();
    #endif
    #if MONITOR_PWM_LIST
    pwmSignal.printSignalList(); Serial.print("\n");
    #endif
    #if MONITOR_REGISTER_SIGNAL
    printRegisterSignal();
    #endif
  }
  #endif
}
