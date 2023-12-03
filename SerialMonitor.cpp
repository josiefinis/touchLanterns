#include "SerialMonitor.h"

SerialMonitor::SerialMonitor( Sensor* sensor, Lantern* lantern, PWMSignal* pwm, Register* reg ) {
  sensor = sensor;
  lantern = lantern;
  pwm = pwm;
  reg = reg;
}

    
void SerialMonitor::printState() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print("0x"); Serial.print(lantern[i].getState(), HEX); Serial.print("\t"); 
  }
  Serial.println();
}
      

#if MONITOR_PWM_SIGNAL
void SerialMonitor::storePWM( uint16_t signal, uint16_t time ) {
  signalMONITOR[MONITORidx & 0x1F] = signal;
  edgeAtMicrosMONITOR[MONITORidx & 0x1F] = edgeAtMicros;
  MONITORidx++;
}


void SerialMonitor::printPWM() {
  Serial.print("\nTime\tSignal\n"); 
  for ( uint8_t i=0; i<32; i++ ) {
    Serial.print(edgeAtMicrosMONITOR[MONITORidx & 0x1F]); Serial.print("\t");
    Serial.print(signalMONITOR[MONITORidx & 0x1F], BIN); Serial.print("\n");
    MONITORidx++;
  }
}


void SerialMonitor::printSignalList() {
  pwmSignal.printSignalList(); Serial.print("\n");
}
#endif


void SerialMonitor::printLanternInput() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print(lantern[i].getInput(), BIN); Serial.print("\t");
  }
  Serial.println();
}


void SerialMonitor::printBrightness() {
  for ( uint8_t i = 0; i < 16; i++ ) { 
    Serial.print(lantern[i].getBrightness()); Serial.print(":"); Serial.print(lantern[i].getBrightnessTarget()); Serial.print("\t"); 
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
    Serial.print(lantern[i].getIndex(), HEX); Serial.print("\t");
  }
  Serial.println(); 
  for ( uint8_t i=0; i<16; i++ ) {
    Serial.print("========");
  }
  Serial.println();
}


void SerialMonitor::printDelay() {
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
