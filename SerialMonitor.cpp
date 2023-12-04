#include "SerialMonitor.h"

SerialMonitor::SerialMonitor() { 
  #if MONITOR_EVENTS
    index[0] = 0xFF;
  #endif
}

    
#if MONITOR_EVENTS
bool SerialMonitor::isBufferFull() {
  return eventIdx >= BUFFER_SIZE;
}


void SerialMonitor::incrementIndex() {
  eventIdx++;
  index[eventIdx] = 0xFF;
  state[eventIdx] = 0xFF;
  input[eventIdx] = 0;
}


void SerialMonitor::storeState( uint8_t idx, uint8_t value ) {
  if ( isBufferFull() ) {
    Serial.println();
    Serial.print( "Warning: Buffer full. Could not store change of state event, index: " ); 
    Serial.print( idx, HEX ); Serial.print( " state: 0x" ); Serial.println( value );
    return 0;
  }
  index[eventIdx] = idx;
  state[eventIdx] = value;
}


void SerialMonitor::storeInput( uint8_t idx, uint16_t value ) {
  if ( isBufferFull() ) {
    Serial.println();
    Serial.print( "Warning: Buffer full. Could not store input event, index: " ); 
    Serial.print( idx, HEX ); Serial.print( " input: 0b" ); Serial.println( value );
    return 0;
  }
  index[eventIdx] = idx;
  input[eventIdx] = value;
}


void SerialMonitor::printBuffer() {
  uint8_t row = 0;
  uint8_t column = 0;
  while ( row < BUFFER_SIZE ) {
    if ( index[row] == 0xFF ) { 
      break; 
    }
    while ( column < index[row] ) {
      Serial.print( "\t" );
      column++;
    }
    Serial.print( index[row], HEX ); Serial.print( ". " );
    if ( state[row] != 0xFF ) {
      Serial.print( "0x" ); Serial.print( state[row], HEX ); 
    }
    Serial.print( "\t<" ); Serial.print( input[row], BIN ); Serial.print( "> " );
    Serial.println();
    index[row] = 0xFF;
    state[row] = 0xFF;
    input[row] = 0;
    row++;
    column = 0;
  }
  eventIdx = 0;
  index[0] = 0xFF;
}
#endif


      

#if MONITOR_PWM_SIGNAL
void SerialMonitor::storePWM( uint16_t signal, uint16_t time ) {
  pwmSignal[pwmIdx & 0x1F] = signal;
  pwmTime[pwmIdx & 0x1F] = time;
  pwmIdx++;
}


void SerialMonitor::printPWM() {
  Serial.print("\nTime\tSignal\n"); 
  for ( uint8_t i=0; i<32; i++ ) {
    Serial.print( pwmTime[pwmIdx & 0x1F] ); Serial.print("\t");
    Serial.print( pwmSignal[pwmIdx & 0x1F], BIN ); Serial.print("\n");
    pwmIdx++;
  }
}


void SerialMonitor::printSignalList( PWMSignal* pwmSignal ) {
  pwmSignal.printSignalList(); Serial.print("\n");
}
#endif


void SerialMonitor::printState( Lantern* lantern ) {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print( "0x" ); Serial.print( lantern[i].getState() ); Serial.print( "\t" ); 
  }
  Serial.println();
}

void SerialMonitor::printLanternInput( Lantern* lantern ) {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print( lantern[i].getInput(), BIN ); Serial.print( "\t" );
  }
  Serial.println();
}


void SerialMonitor::printBrightness( Lantern* lantern ) {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print( lantern[i].getBrightness() ); Serial.print( ":" ); 
    Serial.print( lantern[i].getBrightnessTarget() ); Serial.print( "\t" ); 
  }
  Serial.println();
}


#if MONITOR_TIMINGS
void SerialMonitor::printTimings() {
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


void SerialMonitor::printIndices() {
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print( i, HEX ); Serial.print( "\t" );
  }
  Serial.println(); 
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print("========");
  }
  Serial.println();
}


void SerialMonitor::printDelay( Lantern* lantern ) {
  for ( uint8_t i=0; i<16; i++ ) { Serial.print(lantern[i].getDelay()); Serial.print(" ds\t"); }
  Serial.println();
}


void SerialMonitor::print() {
  #if SERIAL_ON
    printIndices();
  #endif

  #if MONITOR_STATE
    printState();
  #endif
  
  #if MONITOR_LANTERN_INPUT
    printLanternInput();
  #endif
  
  #if MONITOR_BRIGHTNESS
    printBrightness();
  #endif
  
  #if MONITOR_DELAY
    printDelay();
  #endif
  
  #if MONITOR_SIGNAL_LIST
    printSignalList();
  #endif
  
  #if MONITOR_PWM_SIGNAL
    printPWM();
  #endif
}
