#include "Lantern.h"
#include "Graph.h"
#include "Global.h"



// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.


Lantern::Lantern() {
// State machine for controlling lanterns.
  state = OUT;
  input = 0;
  output = 0;
  parent = nullptr;
}


bool Lantern::changeState() {
  output = NO_CHANGE;

  switch ( state ) {
    case OUT:                           //                     >>--( rising edge )--> INIT_UP
      return out();

    case GO_OUT:                        //                                      >>--> OUT
      return goOut();

    case IDLE:                          //                     >>--( rising edge )--> INIT_DOWN
      return idle();                    //                 >>--( brightness == 0 )--> GO_OUT

    case GO_IDLE:                       //                                      >>--> IDLE
      return goIdle();

    case INIT_DOWN:                     //                        >>--( released )--> FULL_DOWN
      return initDown();                //                 >>--( touched, medium )--> FLICKER_DOWN

    case INIT_UP:                       //                        >>--( released )--> FULL_UP
      return initUp();                  //                 >>--( touched, medium )--> FLICKER_UP

    case FULL_DOWN:                     //                                      >>--> GO_OUT
      return fullDown();                

    case FULL_UP:                       //                                      >>--> GO_IDLE
      return fullUp();

    case FLICKER_DOWN:                  //                        >>--( released )--> AUTO_DOWN
      return flickerDown();             //                   >>--( touched, long )--> ROOT_FULL_DOWN

    case FLICKER_UP:                    //                        >>--( released )--> AUTO_UP
      return flickerUp();               //                   >>--( touched, long )--> ROOT_FULL_UP

    case AUTO_DOWN:                     //               >>--( brightness == min )--> PAUSE_DOWN
      return autoDown();                //                     >>--( rising edge )--> PAUSE_DOWN

    case AUTO_UP:                       //               >>--( brightness == max )--> PAUSE_DOWN
      return autoUp();                  //                     >>--( rising edge )--> PAUSE_DOWN

    case PAUSE_DOWN:                    //                     >>--( rising edge )--> AUTO_UP
      return pauseDown();               //                 >>--( touched, medium )--> ROOT_FLICKER_DOWN
                                        //                  >>--( released, long )--> IDLE

    case PAUSE_UP:                      //                     >>--( rising edge )--> AUTO_DOWN
      return pauseUp();                 //                 >>--( touched, medium )--> ROOT_FLICKER_UP
                                        //                  >>--( released, long )--> IDLE

    case ROOT_FULL_DOWN:                //                                      >>--> FULL_DOWN
      return rootFullDown();

    case ROOT_FULL_UP:                  //                                      >>--> FULL_UP
      return rootFullUp();

    case ROOT_FLICKER_DOWN:             //    >>--( touched, long & falling edge )--> ROOT_AUTO_DOWN    
      return rootFlickerDown();         //                        >>--( released )--> AUTO_DOWN           

    case ROOT_FLICKER_UP:               //    >>--( touched, long & falling edge )--> ROOT_AUTO_UP    
      return rootFlickerUp();           //                        >>--( released )--> AUTO_UP           

    case ROOT_AUTO_DOWN:                //               >>--( brightness == min )--> PAUSE_DOWN
      return rootAutoDown();            //                     >>--( rising edge )--> PAUSE_DOWN
                                                                                                 
    case ROOT_AUTO_UP:                  //               >>--( brightness == max )--> PAUSE_DOWN
      return rootAutoUp();              //                     >>--( rising edge )--> PAUSE_DOWN

    case ROOT_PAUSE_DOWN:               //                     >>--( rising edge )--> ROOT_AUTO_UP
      return rootPauseDown();           //                  >>--( released, long )--> IDLE

    case ROOT_PAUSE_UP:                 //                     >>--( rising edge )--> ROOT_AUTO_DOWN
      return rootPauseUp();             //                  >>--( released, long )--> IDLE

    case FOLLOW:                        // 
      return follow();

    case WAIT:                          // 
      return wait();

    case WAIT_FULL_DOWN:                // 
      return waitFullDown();

    case WAIT_FULL_UP:                  // 
      return waitFullUp();
  }
}


bool Lantern::changeOutput() {
  switch ( output & 0xF000 ) {
    case NO_CHANGE:
      return 0;

    case FLICKER:
      uint16_t temp = output;
      temp ^= temp << 2; 
      temp ^= temp >> 7;
      setBrightness( temp );
      return 1;

    case PULSE:
      output++;
      return 1;

    case PULSE_END:
      output = NO_CHANGE;
      setBrightness( referenceBrightness );
      return 1;

    default:
      switch ( state & 0x10 ) {
        case UP_STATE:
          if ( referenceBrightness - getBrightness() < getRate() ) {
            output = NO_CHANGE;
            setBrightness( referenceBrightness );
            return 1;
          }
          output += getRate();
          return 1;

        case DOWN_STATE:
          if ( getBrightness() - referenceBrightness < getRate() ) {
            output = NO_CHANGE;
            setBrightness( referenceBrightness );
            return 1;
          }
          output -= getRate();
          return 1;
      }
  }
}


void Lantern::burnDown() { // TODO change so that lanterns stay near full brightness for longer.
// Expected value = -0.5
  if ( not getBrightness() ) {
    return 0;
  }
  int16_t newReference = referenceBrightness - Random::pull(3) + 3;
  if ( newReference < 0 ) {
    newReference = 0;
  }
  if ( newReference > 255 ) { 
    newReference = 255;
  }
  referenceBrightness = newReference;
}


void Lantern::setIndex(uint8_t value) {
  index = value;
}


uint8_t Lantern::getIndex() {
  return index;
}


void Lantern::setState(uint8_t value) {
  state = value;
}


uint8_t Lantern::getState() {
  return state;
}


uint8_t Lantern::getBrightness() {
  return output >> 4;
}


uint8_t Lantern::getReferenceBrightness() {
  return referenceBrightness;
}


void Lantern::pushInput(uint8_t value) {
  input <<= 1;
  input |= value;
}


uint8_t Lantern::getInput() {
  return input;
}


void Lantern::setDelay(uint8_t value) {
  delay = value;
}


uint8_t Lantern::getDelay() {
  return delay;
}


void Lantern::setParent(Lantern* pLantern) {
  parent = pLantern;
}


Lantern* Lantern::getParent() {
  return parent;
}


// ========================================================================================================================
// PRIVATE
// ========================================================================================================================


bool Lantern::isInput(uint8_t value, uint8_t mask=0x01) {
  return (mask & input) == (mask & value);
}


void Lantern::makeTree() {
// Create a spanning tree of the neighbour graph, starting from this lantern as root. 
  uint8_t parentList[16];
  Tree tree = Tree( parentList, 16 );
  uint16_t neighbourList[16] = { 0x421 , 0x4320, 0x5310, 0x521 ,
                                 0x7610, 0x7632, 0xC854, 0xC854,
                                 0xA976, 0xDBA8, 0xB98 , 0xFA9 ,
                                 0xED76, 0xFEC9, 0xFDC , 0xEDB 
                               };
  Graph neighbourGraph = Graph( neighbourList, 16 );                    
  neighbourGraph.makeSpanningTree( &tree, this->getIndex() );            
  for ( uint8_t i=0; i<16; i++ ) {
    Lantern* parent = getLantern( tree.getParent( i ) );
    Lantern* child = getLantern( i );
    child->setParent( parent );
    child->setState( WAIT );
    delay = 3 + Random::pull( 2 );
    child->setDelay( delay );
  }
}


void Lantern::raiseBrightness( uint8_t rate=2, uint8_t ceiling=BRIGHTNESS_MAX ) {
  if ( getBrightness() >= ceiling ) { 
    output = NO_CHANGE;
    return 0; 
  }
  setRate( rate );
  referenceBrightness = ceiling;
}


void Lantern::lowerBrightness( uint8_t rate=2, uint8_t floor=BRIGHTNESS_MIN ) {
  if ( getBrightness() <= floor ) { 
    output = NO_CHANGE;
    return 0; 
  }
  setRate( rate );
  referenceBrightness = floor;
}


void Lantern::setRate(uint8_t value) {
// Store rate as integer log2( value ) in bits 14-12 of 'output' variable.
  output &= ~0x7000;
  uint8_t exponent = 0;
  while ( value >>= 1 ) {
    exponent++;
  }
  output |= ( exponent & 0x7 ) << 12;
}


uint8_t Lantern::getRate() {
// Calculate and return rate = 1/2 * 2^ilog22x
  uint8_t exponent = ( output >> 12 ) & 0x7;
  return 1 << exponent;
}


void Lantern::setBrightness(uint8_t value) {
  output &= ~0x0FFF;
  output |= value << 4;
}


Lantern* Lantern::getLantern( uint8_t index ) {
  return this - this->getIndex() + index;
}


bool Lantern::out() {  
// Unlit, do nothing until lantern receives touch input.
//     touch: light lantern.
  if ( isInput( RISING, EDGE ) ) { 
    state = INIT_UP; 
    raiseBrightness( 32 );
    return 1;
  }
  return 0;
}


bool Lantern::goOut() {
// Transistion state on the way to out.
  state = OUT;
  referenceBrightness = 0;
  return 0;
}


bool Lantern::idle() {
// Default lit state. 
  if ( isInput( RISING, EDGE ) ) { 
    state = INIT_DOWN; 
    lowerBrightness( 32 );
    return 1;
  }
  if ( getBrightness() == 0 ) { 
    state = GO_OUT; 
    return 1;
  }
  // Vary brightness slowly, tending downward over a few hours until lantern goes out.
  if ( delay++ ) { 
    return 0; 
  } 
  if ( getBrightness() < referenceBrightness ) { 
    raiseBrightness( 1, referenceBrightness );
    return 0;
  } 
  if ( getBrightness() > referenceBrightness ) { 
    lowerBrightness( 1, referenceBrightness );
    return 0;
  }
  return 0;
}


bool Lantern::goIdle() {
// Transistion state on way to IDLE.
  state = IDLE;
  referenceBrightness = getBrightness();
  return 1;
}


bool Lantern::initDown() {
// Initiate increase in brightness immediately in response to a new touch so that user gets feedback. 
  if ( isInput( RELEASED ) ) {              // On short touch: light up to full brightness.
    state = FULL_DOWN; 
    lowerBrightness( 255, 0 );
    return 1;
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {       // On medium touch: make lantern flicker as feedback. From FLICKER can go to either AUTO or ROOT.
    state = FLICKER_DOWN; 
    output = FLICKER;
    referenceBrightness = getBrightness(); 
    return 1;
  }
  lowerBrightness( 32, 128 ); 
  return 0;
}


bool Lantern::initUp() {
// Initiate increase in brightness immediately in response to a new touch so that user gets feedback. 
  if ( isInput( RELEASED ) ) {              // Short duration touch: light up to FULL brightness. 
    state = FULL_UP; 
    raiseBrightness( 255 );
    return 1;
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {       // Medium duration touch: make lantern flicker as feedback. From FLICKER can go to either AUTO or ROOT.
    state = FLICKER_UP;
    output = FLICKER;
    referenceBrightness = getBrightness();
    return 1;
  }
  raiseBrightness( 32 );
  return 0;
}


bool Lantern::fullDown() {
// Decrease to zero brightness.
  state = GO_OUT; 
  setBrightness(0);
  return 1;
}


bool Lantern::fullUp() {
// Increase to full brightness.
  state = GO_IDLE; 
  setBrightness( BRIGHTNESS_MAX );
  return 1;
}


bool Lantern::flickerDown() {
// Make lantern flicker until decision is reached. 
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_DOWN;    
    setBrightness( referenceBrightness );
    return 1;
  }
  if ( isInput( TOUCHED, LONG ) and not getParent() ) {          // Go to ROOT_FULL_DOWN to put out all lanterns.
    state = ROOT_FULL_DOWN; 
    makeTree();
    setBrightness( 0 );
    return 1;
  }
  output = FLICKER;
  return 0;
}


bool Lantern::flickerUp() {
// Make lantern flicker until decision is reached. 
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_UP;                                                                                                             
    setBrightness( referenceBrightness );
    return 1;                                                                                                                    
  }                                                                                                                              
  if ( isInput( TOUCHED, LONG ) and not getParent() ) {          // Go to ROOT_FULL_UP to light all lanterns to full brightness.                           
    state = ROOT_FULL_UP; 
    makeTree();
    setBrightness( BRIGHTNESS_MAX );
    return 1;
  }
  output = FLICKER;
  return 0;
}


bool Lantern::autoDown() {
// Without touch, decrease brightness gradually. Pause and wait for input if it reaches minimum brightness or if lantern is touched again.
  if ( getBrightness() == BRIGHTNESS_MIN )  {           
    state = PAUSE_DOWN; 
    return 1;
  }
  if ( isInput( RISING, EDGE ) ) {                      
    state = PAUSE_DOWN; 
    output = PULSE_START;
    return 1;

  }
  lowerBrightness( 2 );
  return 0;
}


bool Lantern::autoUp() {
// Without touch, increase brightness gradually until it reaches maximum brightness or until lantern is touched again.
  if ( getBrightness() == BRIGHTNESS_MAX ) {            
    state = PAUSE_UP; 
    return 1;
  }
  if ( isInput( RISING, EDGE ) ) {                      
    state = PAUSE_UP; 
    output = PULSE_START;
    return 1;
  }
  raiseBrightness( 2 );
  return 0;
}


bool Lantern::pauseDown() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go to back to AUTO but change to AUTO_UP.
    state = AUTO_UP; 
    output = PULSE_START;
    return 1;
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {                   // Continued touch from AUTO: go to ROOT_FLICKER to allow fine control of all lanterns' brightness. 
    state = ROOT_FLICKER_DOWN;
    output = FLICKER;
    referenceBrightness = getBrightness();
    return 1;
  }
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 1;
  }
  return 0;
}


bool Lantern::pauseUp() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to AUTO but change to AUTO_DOWN.
    state = AUTO_DOWN;                                                                                                                                         
    output = PULSE_START;
    return 1;
  }                                                                                                                                                          
  if ( isInput( TOUCHED, MEDIUM ) ) {                   // Continued touch from AUTO: go to ROOT_FLICKER to allow fine control of all lanterns' brightness. 
    state = ROOT_FLICKER_UP;                                                                                                                                 
    output = FLICKER;
    referenceBrightness = getBrightness();
    return 1;
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 1;
  }
  return 0;
}


bool Lantern::rootFullDown() { 
// Put out this lantern, all other lanterns will follow in succession.
  state = FULL_DOWN;
  return 1;
}


bool Lantern::rootFullUp() { 
// Light to full brightness, all other lanterns will follow in succession.
  state = FULL_UP;
  return 1;
}


bool Lantern::rootFlickerDown() {
// Make lantern flicker until decision is reached. 
  if ( isInput( TOUCHED, LONG & FALLING ) and not getParent() ) {           // Go to ROOT_AUTO to allow fine control of all lanterns' brightness.
    state = ROOT_AUTO_DOWN; 
    setBrightness( referenceBrightness );
    makeTree();
    return 1;
  }
  if ( isInput( RELEASED ) ) {                                              // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_DOWN;                                                                                                             
    setBrightness( referenceBrightness );
    return 1;                                                                                                                    
  }                                                                                                                              
  output = FLICKER;
  return 0;
}


bool Lantern::rootFlickerUp() {
// Make lantern flicker until decision is reached. 
  if ( isInput( TOUCHED, LONG & FALLING ) and not getParent() ) {           // Go to ROOT_AUTO to allow fine control of all lanterns' brightness.
    state = ROOT_AUTO_UP; 
    setBrightness( referenceBrightness );
    makeTree();
    return 1;
  }
  if ( isInput( RELEASED ) ) {                                              // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_UP;                                                                                                             
    setBrightness( referenceBrightness );
    return 1;                                                                                                                    
  }                                                                                                                              
  output = FLICKER;
  return 0;
}


bool Lantern::rootAutoDown() {
// Gradually decrease in brightness, all other lanterns will follow in succession.
  if ( getBrightness() == BRIGHTNESS_MIN ) { 
    state = ROOT_PAUSE_DOWN; 
    return 1;
  }
  if ( isInput( RISING, EDGE ) ) { 
    state = ROOT_PAUSE_DOWN; 
    output = PULSE_START;
    return 1;
  }
  lowerBrightness();
  return 0;
}


bool Lantern::rootAutoUp() {
// Gradually increase in brightness, all other lanterns will follow in succession.
  if ( getBrightness() == BRIGHTNESS_MAX ) { 
    state = ROOT_PAUSE_UP; 
    return 1;
  }
  if ( isInput( RISING, EDGE ) ) { 
    state = ROOT_PAUSE_UP; 
    output = PULSE_START;
    return 1;
  }
  raiseBrightness();
  return 0;
}


bool Lantern::rootPauseDown() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to ROOT_AUTO but switch to UP.
    state = ROOT_AUTO_UP;                                                                                                                                  
    output = PULSE_START;
    return 1;
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 1;
  }
  return 0;
}


bool Lantern::rootPauseUp() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to ROOT_AUTO but switch to DOWN.
    state = ROOT_AUTO_DOWN;                                                                                                                                    
    output = PULSE_START;
    return 1;
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 1;
  }
  return 0;
}


bool Lantern::wait() {
// Wait before following parent lantern in tree.
  if ( parent->getState() == WAIT ) {
    return 0;
  }
  if ( ( parent->getState() & 0x1F ) == FULL_DOWN ) {       // Go to WAIT_FULL because the parent lantern may already have changed and be IDLE before delay is over.
    state = WAIT_FULL_DOWN;
    return 1;
  }
  if ( ( parent->getState() & 0x1F ) == FULL_UP ) {         // Go to WAIT_FULL because the parent lantern may already have changed and be IDLE before delay is over.
    state = WAIT_FULL_UP;
    return 1;
  }
  if ( delay-- ) { 
    return 0; 
  }
  state = FOLLOW; 
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  return 1;
}


bool Lantern::waitFullDown() {
// Wait before going out.
  if ( parent->getState() == WAIT_FULL_DOWN ) {
    return 0;
  }
  if ( ( parent->getState() & 0x1F ) == FULL_UP ) {                             // Go to WAIT_FULL_UP if parent's state has changed.
    state = WAIT_FULL_UP;
    return 1;
  }
  if ( parent->getState() == WAIT or parent->getState() == FOLLOW ) {           // Go back to WAIT if parent's state has changed.
    state = WAIT;
    return 1;
  }
  if ( delay-- ) { 
    return 0; 
  }
  state = FULL_DOWN; 
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  parent = nullptr;
  return 1;
}


bool Lantern::waitFullUp() {
// Wait before lighting to full brightness.
  if ( parent->getState() == WAIT_FULL_UP ) {
    return 0;
  }
  if ( ( parent->getState() & 0x1F ) == FULL_DOWN ) {                           // Go to WAIT_FULL_DOWN if parent's state has changed.
    state = WAIT_FULL_DOWN;                                                                                                          
    return 1;                                                                                                                        
  }                                                                                                                                  
  if ( parent->getState() == WAIT or parent->getState() == FOLLOW ) {           // Go back to WAIT if parent's state has changed.
    state = WAIT;
    return 1;
  }
  if ( delay-- ) { 
    return 0; 
  }
  state = FULL_UP; 
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  parent = nullptr;
  return 1;
}


bool Lantern::follow() { 
// Track parent lanterns brightness changes. Lag slightly behind so that changes propagate through all lanterns over a short time. 
  if ( getBrightness() > parent->getBrightness() ) { 
    lowerBrightness( 2, parent->getBrightness() ); 
    return 0;
  }
  if ( getBrightness() < parent->getBrightness() ) { 
    raiseBrightness( 2, parent->getBrightness() ); 
    return 0;
  }
  if ( parent->getState() == IDLE ) { 
    state = GO_IDLE; 
    parent = nullptr;
    return 1;
  }
  if ( parent->getState() == OUT ) { 
    state = GO_OUT; 
    parent = nullptr;
    return 1;
  }
  return 0;
}
