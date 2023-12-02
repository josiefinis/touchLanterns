#include "Lantern.h"
#include "Global.h"


uint8_t Lantern::nTreeNodes = 0;
Lantern* Lantern::root = nullptr;


Lantern::Lantern() {
// State machine for controlling lanterns.
  state = OUT;
  parent = nullptr;
}

// TODO Monitor timings and optimise.

bool Lantern::update() {
  // TODO Consider making this a tree and separate states by brightness up/down.

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


bool Lantern::out() {  
// Unlit, do nothing until lantern receives touch input.
//     touch: light lantern.
  if ( isInput( RISING, EDGE ) ) { 
    state = INIT_UP; 
    return raiseBrightness( 32 );
  }
  return 0;
}


bool Lantern::goOut() {
// Transistion state on the way to out.
  state = OUT;
  brightnessTarget = 0;
  return 0;
}


bool Lantern::idle() {
// Default lit state. 
  if ( isInput( RISING, EDGE ) ) { 
    state = INIT_DOWN; 
    lowerBrightness( 32 );
    return 0;
  }
  if ( getBrightness() == 0 ) { 
    state = GO_OUT; 
    return 0;
  }
  // Vary brightness slowly, tending downward over a few hours until lantern goes out.
  if ( delay++ ) { return 0; } 
  if ( getBrightness() < brightnessTarget ) { 
    return raiseBrightness( 1, brightnessTarget );
  } 
  if ( getBrightness() > brightnessTarget ) { 
    return lowerBrightness( 1, brightnessTarget);
  }
  return 0;
}


bool Lantern::goIdle() {
// Transistion state on way to IDLE.
  state = IDLE;
  brightnessTarget = getBrightness();
  return 0;
}


bool Lantern::initDown() {
// Initiate increase in brightness immediately in response to a new touch so that user gets feedback. 
  if ( isInput( RELEASED ) ) {              // On short touch: light up to full brightness.
    state = FULL_DOWN; 
    return lowerBrightness( 255, 0 );
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {       // On medium touch: make lantern flicker as feedback. From FLICKER can go to either AUTO or ROOT.
    state = FLICKER_DOWN; 
    input = 1;
    return flickerBrightness();
  }
  return lowerBrightness( 32, 128 ); 
}


bool Lantern::initUp() {
// Initiate increase in brightness immediately in response to a new touch so that user gets feedback. 
  if ( isInput( RELEASED ) ) {              // Short duration touch: light up to FULL brightness. 
    state = FULL_UP; 
    return raiseBrightness( 255 );
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {       // Medium duration touch: make lantern flicker as feedback. From FLICKER can go to either AUTO or ROOT.
    state = FLICKER_UP;
    input = 1;
    return flickerBrightness();
  }
  return raiseBrightness( 32 );
}


bool Lantern::fullDown() {
// Decrease to zero brightness.
  state = GO_OUT; 
  return setBrightness(0);
}


bool Lantern::fullUp() {
// Increase to full brightness.
  state = GO_IDLE; 
  return setBrightness( BRIGHTNESS_MAX );
}


bool Lantern::flickerDown() {
// Make lantern flicker until decision is reached. 
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_DOWN;    
    return 0;
  }
  if ( isInput( TOUCHED, LONG ) and not nTreeNodes ) {          // Go to ROOT_FULL_DOWN to put out all lanterns.
    state = ROOT_FULL_DOWN; 
    buildTree();
    return setBrightness(0);
  }
  return flickerBrightness();
}


bool Lantern::flickerUp() {
// Make lantern flicker until decision is reached. 
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_UP;                                                                                                             
    return 0;                                                                                                                    
  }                                                                                                                              
  if ( isInput( TOUCHED, LONG ) and not nTreeNodes ) {          // Go to ROOT_FULL_UP to light all lanterns to full brightness.                           
    state = ROOT_FULL_UP; 
    buildTree();
    return setBrightness( BRIGHTNESS_MAX );
  }
  return flickerBrightness();
}


bool Lantern::autoDown() {
// Without touch, decrease brightness gradually. Pause and wait for input if it reaches minimum brightness or if lantern is touched again.
  if ( getBrightness() == BRIGHTNESS_MIN )  {           
    state = PAUSE_DOWN; 
    return 0;
  }
  if ( isInput( RISING, EDGE ) ) {                      
    state = PAUSE_DOWN; 
    return pulseBrightness();

  }
  return lowerBrightness( 2 );
}


bool Lantern::autoUp() {
// Without touch, increase brightness gradually until it reaches maximum brightness or until lantern is touched again.
  if ( getBrightness() == BRIGHTNESS_MAX ) {            
    state = PAUSE_UP; 
    return 0;
  }
  if ( isInput( RISING, EDGE ) ) {                      
    state = PAUSE_UP; 
    return pulseBrightness();
  }
  return raiseBrightness( 2 );
}


bool Lantern::pauseDown() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go to back to AUTO but change to AUTO_UP.
    state = AUTO_UP; 
    return pulseBrightness();
  }
  if ( isInput( TOUCHED, MEDIUM ) ) {                   // Continued touch from AUTO: go to ROOT_FLICKER to allow fine control of all lanterns' brightness. 
    state = ROOT_FLICKER_DOWN;
    return flickerBrightness();
  }
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 0;
  }
}


bool Lantern::pauseUp() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to AUTO but change to AUTO_DOWN.
    state = AUTO_DOWN;                                                                                                                                         
    return pulseBrightness();
  }                                                                                                                                                          
  if ( isInput( TOUCHED, MEDIUM ) ) {                   // Continued touch from AUTO: go to ROOT_FLICKER to allow fine control of all lanterns' brightness. 
    state = ROOT_FLICKER_UP;                                                                                                                                 
    return flickerBrightness();                                                                                                                              
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    return 0;
  }
}


bool Lantern::rootFullDown() { 
// Put out this lantern, all other lanterns will follow in succession.
  state = FULL_DOWN;
  neighbourList.shuffle(); 
  neighbourList.resetIndex();
}


bool Lantern::rootFullUp() { 
// Light to full brightness, all other lanterns will follow in succession.
  state = FULL_UP;
  neighbourList.shuffle();
  neighbourList.resetIndex();
}


bool Lantern::rootFlickerDown() {
// Make lantern flicker until decision is reached. 
  if ( isInput( TOUCHED, LONG & FALLING ) and not nTreeNodes ) {          // Go to ROOT_AUTO to allow fine control of all lanterns' brightness.
    state = ROOT_AUTO_DOWN; 
    buildTree();
    return 0;
  }
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_DOWN;                                                                                                             
    return 0;                                                                                                                    
  }                                                                                                                              
  return flickerBrightness();
}


bool Lantern::rootFlickerUp() {
// Make lantern flicker until decision is reached. 
  if ( isInput( TOUCHED, LONG & FALLING ) and not nTreeNodes ) {          // Go to ROOT_AUTO to allow fine control of all lanterns' brightness.
    state = ROOT_AUTO_UP; 
    buildTree();
    return 0;
  }
  if ( isInput( RELEASED ) ) {                                  // Go to AUTO to allow fine control of this lantern's brightness.
    state = AUTO_UP;                                                                                                             
    return 0;                                                                                                                    
  }                                                                                                                              
  return flickerBrightness();
}


bool Lantern::rootAutoDown() {
// Gradually decrease in brightness, all other lanterns will follow in succession.
  if ( getBrightness() == BRIGHTNESS_MIN ) { 
    state = ROOT_PAUSE_DOWN; 
    return 0;
  }
  if ( isInput( RISING, EDGE ) ) { 
    state = ROOT_PAUSE_DOWN; 
    return pulseBrightness();
  }
  return lowerBrightness();
}


bool Lantern::rootAutoUp() {
// Gradually increase in brightness, all other lanterns will follow in succession.
  if ( getBrightness() == BRIGHTNESS_MAX ) { 
    state = ROOT_PAUSE_UP; 
    return 0;
  }
  if ( isInput( RISING, EDGE ) ) { 
    state = ROOT_PAUSE_UP; 
    return pulseBrightness();
  }
  return raiseBrightness();
}


bool Lantern::rootPauseDown() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to ROOT_AUTO but switch to UP.
    state = ROOT_AUTO_UP;                                                                                                                                  
    return pulseBrightness();
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    neighbourList.shuffle();
    neighbourList.resetIndex();
    return 0;
  }
}


bool Lantern::rootPauseUp() {
// Maintain steady brightness and wait for input.
  if ( isInput( RISING, EDGE ) ) {                      // New touch: go back to ROOT_AUTO but switch to DOWN.
    state = ROOT_AUTO_DOWN;                                                                                                                                    
    return pulseBrightness();
  }                                                                                                                                                          
  if ( isInput( RELEASED, LONG ) ) {                    // Not touched for a LONG duration: go to IDLE.
    state = GO_IDLE; 
    neighbourList.shuffle();
    neighbourList.resetIndex();
    return 0;
  }
}


bool Lantern::wait() {
// Wait before following parent lantern in tree.
  if ( ( parent->getState() & 0x1F ) == FULL_DOWN ) {       // Go to WAIT_FULL because the parent lantern may already have changed and be IDLE before delay is over.
    state = WAIT_FULL_DOWN;
    return 0;
  }
  if ( ( parent->getState() & 0x1F ) == FULL_UP ) {         // Go to WAIT_FULL because the parent lantern may already have changed and be IDLE before delay is over.
    state = WAIT_FULL_UP;
    return 0;
  }
  if ( delay-- ) { return 0; }
  state = FOLLOW; 
  neighbourList.shuffle(); 
  neighbourList.resetIndex();
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  return 0;
}


bool Lantern::waitFullDown() {
// Wait before going out.
  if ( delay-- ) { return 0; }
  state = FULL_DOWN; 
  neighbourList.shuffle(); 
  neighbourList.resetIndex();
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  parent = nullptr;
  nTreeNodes--;
  return 0;
}


bool Lantern::waitFullUp() {
// Wait before lighting to full brightness.
  if ( delay-- ) { return 0; }
  state = FULL_UP; 
  neighbourList.shuffle(); 
  neighbourList.resetIndex();
  delay = micros(); // delay counter has another use marking periodic brightness adjustments. Good enough random number here ensures lantern adjustments are staggered.
  parent = nullptr;
  nTreeNodes--;
  return 0;
}


bool Lantern::follow() { 
// Track parent lanterns brightness changes. Lag slightly behind so that changes propagate through all lanterns over a short time. 
  if ( getBrightness() > parent->getBrightness() ) { 
    return lowerBrightness( 2, parent->getBrightness() ); 
  }
  if ( getBrightness() < parent->getBrightness() ) { 
    return raiseBrightness( 2, parent->getBrightness() ); 
  }
  if ( parent->getState() == IDLE ) { 
    state = GO_IDLE; 
    parent = nullptr;
    nTreeNodes--;
    return 0;
  }
  if ( parent->getState() == OUT ) { 
    state = GO_OUT; 
    parent = nullptr;
    nTreeNodes--;
    return 0;
  }
  return 0;
}


bool Lantern::raiseBrightness( uint8_t rate=2, uint8_t ceiling=BRIGHTNESS_MAX ) {
  if ( brightness >= ceiling ) { return 0; }
  while ( not brightnessQueue.isEmpty() ) { 
    brightnessQueue.dequeue();
  }
  for ( uint8_t i=1; i<=16; i++ ) {
    uint16_t step = i * rate / 16;
    if ( step < ceiling - brightness ) {
      brightnessQueue.enqueue(brightness + step);
    }
    else {
      brightnessQueue.enqueue(ceiling);
      break;
    }
  }
  return 1;
}


bool Lantern::lowerBrightness( uint8_t rate=2, uint8_t floor=BRIGHTNESS_MIN ) {
  if ( brightness <= floor ) { return 0; }
  while ( not brightnessQueue.isEmpty() ) { 
    brightnessQueue.dequeue();
  }
  for ( uint8_t i=1; i<=16; i++ ) {
    uint16_t step = i * rate / 16; 
    if ( step < brightness - floor ) {
      brightnessQueue.enqueue(brightness - step);
    }
    else {
      brightnessQueue.enqueue(floor);
      break;
    }
  }
  return 1;
}

// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.

bool Lantern::flickerBrightness() {
  while ( not brightnessQueue.isEmpty() ) { 
    brightnessQueue.dequeue();
  }
  uint16_t temp = brightness;
  for ( uint8_t i=0; i<15; i++ ) {
    temp ^= temp << 2;
    temp ^= temp >> 7;
    brightnessQueue.enqueue( temp );
  }
  brightnessQueue.enqueue( brightness );
  return 1;
}


bool Lantern::pulseBrightness() {
  while ( not brightnessQueue.isEmpty() ) { 
    brightnessQueue.dequeue();
  }
  brightnessQueue.enqueue( BRIGHTNESS_MAX );
  brightnessQueue.enqueue( 0 );
  brightnessQueue.enqueue( brightness );
  return 1;
}


bool Lantern::nextBrightness() {
  if ( brightnessQueue.isEmpty() ) { 
    return 0;
  }
  brightness = brightnessQueue.dequeue();
  if ( brightnessQueue.isEmpty() ) {
    return 1;
  }
  if ( brightness != brightnessQueue.peek() ) {
    return 1;
  }
  return 0;
}


void Lantern::pushInput(uint8_t value) {
  input <<= 1;
  input |= value;
}


void Lantern::buildTree() {
// Build a binary tree starting from this lantern. 
  #if MONITOR_BUILD_TREE
    Serial.print("Build tree from root "); Serial.println(this->getIndex());
  #endif
  Lantern* root = this;
  root->setParent(nullptr);
  QLantern queue;
  Lantern* lantern = root;
  queue.enqueue(lantern);
  delay = 1;

  while ( not queue.isEmpty() ) {
    lantern = queue.dequeue();
    #if MONITOR_BUILD_TREE
      lantern->printNeighbours(); 
    #endif
    Lantern* neighbour;
    while ( neighbour = lantern->nextNeighbour() ) {
      #if MONITOR_BUILD_TREE
        Serial.print("Lantern "); Serial.print( lantern->getIndex() ); 
        Serial.print(": check neighbour "); Serial.print( neighbour->getIndex() ); Serial.print("\n");
      #endif

      if ( neighbour->getParent() ) { continue; }
      if ( neighbour == root ) { continue; }
      
      #if MONITOR_BUILD_TREE
        Serial.print("Add lantern "); Serial.print( neighbour->getIndex() ); Serial.print(" to tree under parent ");
        Serial.print( lantern->getIndex() ); Serial.print(".\n"); 
      #endif

      neighbour->setParent(lantern);
      neighbour->setState(WAIT);
      delay += 3 + Random::pull(2);
      neighbour->setDelay(delay);
      nTreeNodes++;
      queue.enqueue(neighbour);
    }

    #if MONITOR_BUILD_TREE
    Serial.print("New beacon: "); Serial.println(lantern->getIndex());
    queue.print();
    #endif
  }
}


void Lantern::burnDown() { // TODO change so that lanterns stay near full brightness for longer.
// Expected value = -0.5
  if ( not getBrightness() ) {
    return 0;
  }
  int16_t newTarget = brightnessTarget - Random::pull(3) + 3;
  if ( newTarget < 0 ) {
    newTarget = 0;
  }
  if ( newTarget > 255 ) { 
    newTarget = 255;
  }
  brightnessTarget = newTarget;
}


void Lantern::addNeighbour(Lantern* neighbour) {
  neighbourList.put(neighbour);
}


Lantern* Lantern::nextNeighbour() {
  return neighbourList.next();
}


#if MONITOR_BUILD_TREE
void Lantern::printNeighbours() {
  neighbourList.print();
}
#endif


uint8_t Lantern::getIndex() {
  return index;
}


uint8_t Lantern::getState() {
  return state;
}


uint8_t Lantern::getInput() {
  return input;
}


uint8_t Lantern::getBrightness() {
  return brightness; 
}


uint8_t Lantern::getBrightnessTarget() {
  return brightnessTarget;
}


uint8_t Lantern::getDelay() {
  return delay;
}


Lantern* Lantern::getParent() {
  return parent;
}


void Lantern::setIndex(uint8_t value) {
  index = value;
}


void Lantern::setState(uint8_t value) {
  state = value;
}


bool Lantern::setBrightness(uint8_t value) {
  while ( not brightnessQueue.isEmpty() ) {
    brightnessQueue.dequeue();
  }
  brightnessQueue.enqueue(value);
  return 1;
}


void Lantern::setParent(Lantern* pLantern) {
  parent = pLantern;
}


void Lantern::setDelay(uint8_t value) {
  delay = value;
}


void Lantern::setNeighbours(Lantern* neighbour[4], uint8_t nNeighbours) {
  for ( uint8_t i=0; i<nNeighbours; i++ ) {
    neighbourList.put(neighbour[i]);
  }
  neighbourList.shuffle();
  neighbourList.resetIndex();
}


bool Lantern::isInput(uint8_t value, uint8_t mask=0x01) {
  return (mask & input) == (mask & value);
}


bool Lantern::isTouched(uint8_t duration=0x01) {
  return (input & duration) == duration;
}


bool Lantern::isReleased(uint8_t duration=0x01) {
  return (input & duration) == 0;
}


bool Lantern::isDoubleTouched() {
  return  (input & 0x3) == 0x1 and (input & 0xFC);
}


LinkedList::LinkedList() {
// Linked list for Lantern objects.
  p0 = nullptr;
  pIndex = nullptr;
  nNodes = 0;
}


Lantern* LinkedList::next() {
// Return next lantern from list.
  if ( not pIndex ) {
    return nullptr;
  }
  Node* temp = pIndex;
  pIndex = pIndex->pNext;
  return temp->pLantern;
}


void LinkedList::resetIndex() {
// Move index to top of list.
  pIndex = p0;
}


void LinkedList::put(Lantern* lantern) {
// Put a lantern at the start of the list.
  Node* newNode = new Node;
  newNode->pLantern = lantern;
  newNode->pNext = p0;
  p0 = newNode;
  nNodes++;
}


void LinkedList::shuffle() {  
// Shuffle the list in place using Fisher-Yates algorithm.
  uint8_t i = nNodes;
  Node* iNode = p0;
  uint8_t j;
  Node* jNode;
  Lantern* temp;
  while ( iNode->pNext ) {
    j = Random::urandom( i-- );
    jNode = iNode;
    while ( j-- ) { 
      jNode = jNode->pNext; 
    }
    temp = iNode->pLantern;
    iNode->pLantern = jNode->pLantern;
    jNode->pLantern = temp;
    iNode = iNode->pNext;
  }
}


#if MONITOR_BUILD_TREE
void LinkedList::print() {
// Print the list.
  Serial.print(nNodes); Serial.print(" neighbours: ");
  Node* pNode = p0;
  while ( pNode ) { Serial.print(pNode->pLantern->getIndex(), HEX); Serial.print("-> "); pNode = pNode->pNext; }
  Serial.print("\n");
}
#endif


QLantern::QLantern() {
// Queue for Lantern objects.
  pFront = nullptr;
}


bool QLantern::isEmpty() {
// True if queue has no nodes.
  if ( pFront ) {
    return false;
  }
  else {
    return true;
  }
}


void QLantern::enqueue(Lantern* lantern) {
// Put lantern at back of queue.
  Node* newNode = new Node;
  newNode->pLantern = lantern;
  newNode->pNext = nullptr;

  if ( isEmpty() ) { 
    pFront = newNode;
  }
  else {
    pBack->pNext = newNode;
  }
  pBack = newNode;
}


Lantern* QLantern::dequeue() {
// Take lantern from front of queue.
  Lantern* lantern = pFront->pLantern;
  Node* oldFront = pFront;
  pFront = oldFront->pNext;
  delete oldFront;

  return lantern;
}


#if MONITOR_BUILD_TREE
void QLantern::print() {
// Print the queue.
  Serial.print("Queue: ");
  Node* q = pFront;
  while ( q ) { Serial.print(q->pLantern->getIndex()); Serial.print("->>"); q = q->pNext; }
  Serial.print("\n");
}
#endif


Qint::Qint() {
// Queue for integer objects.
  pFront = nullptr;
}


bool Qint::isEmpty() {
// True if queue has no nodes.
  if ( pFront ) {
    return false;
  }
  else {
    return true;
  }
}


void Qint::enqueue(uint8_t value) {
// Put value at back of queue.
  Node* newNode = new Node;
  newNode->value = value;
  newNode->pNext = nullptr;

  if ( isEmpty() ) { 
    pFront = newNode;
  }
  else {
    pBack->pNext = newNode;
  }
  pBack = newNode;
}


uint8_t Qint::dequeue() {
// Take value from front of queue.
  if ( not pFront ) { return 0; }
  uint8_t value = pFront->value;
  Node* oldFront = pFront;
  pFront = oldFront->pNext;
  delete oldFront;

  return value;
}


uint8_t Qint::peek() {
// Return value from front of queue without removing it from queue.
  return pFront->value;
}


#if MONITOR_BUILD_TREE
void Qint::print() {
// Print the queue.
  Serial.print("Brightness queue: ");
  Node* q = pFront;
  while ( q ) { Serial.print( q->value ); Serial.print( "->>" ); q = q->pNext; }
  Serial.print("\n");
}
#endif
