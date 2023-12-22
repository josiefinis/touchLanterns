/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#ifndef LANTERN_CPP
#define LANTERN_CPP

#include "Lantern.h"

// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.
// TODO handle INIT_DOWN when low brightness


#define TRANSITION_TABLE_ROWS      41

const uint32_t transitionTable[ TRANSITION_TABLE_ROWS ] PROGMEM = {  
    AUTO            |  ENTRY                                    |  STEP_SET_TO_ONE                      |  0
  , AUTO            |  LIGHT_EQ_FULL                            |  0                                    |  PAUSE
  , AUTO            |  LIGHT_EQ_ONE                             |  0                                    |  PAUSE
  , AUTO            |  NO_PARENT                                |  LIGHT_RAISE                          |  0
  , AUTO            |  PARENT_NOT_AUTO | LIGHT_EQ_PARENTS       |  0                                    |  PAUSE
  , AUTO            |  SENSOR_RISING_EDGE | NO_PARENT           |  0                                    |  PAUSE
  , AUTO            |  STEP_IS_DOWN | NO_PARENT                 |  LIGHT_LOWER                          |  0
  , AUTO            |  TIMER_OVER | LIGHT_GT_PARENTS            |  LIGHT_LOWER                          |  0
  , AUTO            |  TIMER_OVER | LIGHT_LT_PARENTS            |  LIGHT_RAISE                          |  0
  , FLICKER         |  ALWAYS                                   |  FLICKER                              |  0
  , FLICKER         |  ENTRY                                    |  FLICKER | REF_SET_TO_LIGHT           |  0
  , FLICKER         |  EXIT                                     |  LIGHT_SET_TO_REF                     |  0
  , FLICKER         |  SENSOR_FALLING_EDGE                      |                                       |  AUTO
  , FLICKER         |  SENSOR_LONG_TOUCH | NO_TREE              |  MAKE_TREE                            |  FULL
  , FULL            |  ENTRY                                    |  STEP_SET_TO_HUGE                     |  0
  , FULL            |  EXIT                                     |  STEP_SET_TO_TINY                     |  0
  , FULL            |  LIGHT_EQ_FULL                            |  LEAVE_TREE                           |  IDLE 
  , FULL            |  LIGHT_EQ_ZERO                            |  LEAVE_TREE                           |  IDLE 
  , FULL            |  NO_PARENT                                |  LIGHT_RAISE                          |  0
  , FULL            |  STEP_IS_DOWN | NO_PARENT                 |  LIGHT_LOWER                          |  0
  , FULL            |  TIMER_OVER | LIGHT_LT_PARENTS            |  LIGHT_RAISE                          |  0
  , FULL            |  TIMER_OVER | LIGHT_GT_PARENTS            |  LIGHT_LOWER                          |  0
  , IDLE            |  ENTRY                                    |  STEP_SET_TO_TINY                     |  0     
  , IDLE            |  EXIT                                     |  TIMER_CLEAR                          |  0  
  , IDLE            |  LIGHT_EQ_ZERO                            |  TIMER_CLEAR | REF_SET_TO_ZERO        |  0
  , IDLE            |  PARENT_IS_AUTO                           |  TIMER_SET_TO_RANDOM                  |  AUTO
  , IDLE            |  PARENT_IS_FULL                           |  TIMER_SET_TO_RANDOM                  |  FULL
  , IDLE            |  SENSOR_RISING_EDGE | NO_PARENT           |  0                                    |  INIT
  , IDLE            |  TIMER_OVER | LIGHT_GT_REF                |  TIMER_SET_LONG, LIGHT_LOWER          |  0
  , IDLE            |  TIMER_OVER | LIGHT_LT_REF                |  TIMER_SET_LONG, LIGHT_RAISE          |  0
  , INIT            |  ALWAYS                                   |  LIGHT_RAISE                          |  0
  , INIT            |  ENTRY                                    |  STEP_SET_TO_LARGE                    |  0
  , INIT            |  SENSOR_FALLING_EDGE                      |  0                                    |  FULL
  , INIT            |  SENSOR_MEDIUM_TOUCH                      |  0                                    |  FLICKER
  , INIT            |  STEP_IS_DOWN                             |  LIGHT_LOWER                          |  0
  , PAUSE           |  ENTRY                                    |  PULSE | TIMER_SET_TO_SHORT           |  0
  , PAUSE           |  EXIT                                     |  PULSE | CLEAR_TIMER | FLIP_UP_DOWN   |  0
  , PAUSE           |  PARENT_IS_AUTO                           |  0                                    |  AUTO
  , PAUSE           |  SENSOR_MEDIUM_EDGE | NO_TREE             |  MAKE_TREE                            |  FLICKER
  , PAUSE           |  SENSOR_RISING_EDGE | NO_PARENT           |  0                                    |  AUTO
  , PAUSE           |  TIMER_OVER                               |  LEAVE_TREE                           |  IDLE
}


Lantern::Lantern() 
  : StateMachine( transitionMatrix )
  , sensorRegister( 0 )
  , brightness( 0 )
  , referenceBrightness( 0 )
  , delay( 0xFF )
  , parent( nullptr ) 
{}


void Lantern::sense( bool value ) {
  uint16_t temp = sensorRegister << 1;
  sensorRegister = ( uint8_t ) temp | value;
}


uint8_t Lantern::sensorInput( void ) {
  switch ( sensorRegister ) {
    case 0xFF:
      return LONG_TOUCH;

    case 0x0F:
      return MEDIUM_TOUCH;
  }
  switch ( sensorRegister & 0x3 ) {
    case 0x1:
      return RISING_EDGE;
    
    case 0x2:
      return FALLING_EDGE;
  }
  return 0;
}


uint8_t Lantern::lightInput( void ) {
  uint16_t lightInput = 0;
  uint8_t light = getLight();
  switch ( light ) {
    case 0:
      lightInput = LIGHT_EQ_ZERO;
      break;

    case 1:
      lightInput = LIGHT_EQ_ONE;
      break;

    case FULL_BRIGHTNESS:
      lightInput = LIGHT_EQ_FULL;
      break;
  }
  
  if ( light == reference ) {
    lightInput |= LIGHT_EQ_REF;
  }
  else if ( light < reference ) {
    lightInput |= LIGHT_LT_REF;
  }
  else {
    lightInput |= LIGHT_GT_REF;
  }

  uint8_t parentLight = parent->getLight();
  if ( light == parentLight ) {
    lightInput |= LIGHT_EQ_PARENTS;
  }
  else if ( light < parentLight ) {
    lightInput |= LIGHT_LT_PARENTS;
  }
  else {
    lightInput |= LIGHT_GT_PARENTS;
  }
  return lightInput;
}


uint8_t Lantern::classifyParentInput( void ) {
  if ( not tree ) {
    return NO_TREE;
  }
  if ( not parent ) {
    return NO_PARENT;
  }
  uint16_t parentInput = 0;
  switch ( parent->getState() ) {
    case FULL:
      return PARENT_IS_FULL; 

    case AUTO:
      return PARENT_IS_AUTO;
  }
  return 0;
}


uint16_t Lantern::combineInput( void ) {
// Return the highest priority type of input, where priority order is delay, parent, sensor, brightness input.
  uint16_t input = 0;
  if ( delay == 0 ) {                                    
    delay--;
    input |= TIMER_OVER; 
  }
  input |= classifySensorInput();
  input |= classifyLightInput();
  input |= classifyParentInput();
  return input;
}


if ( isInput( input ) ) {
  output = getOutput();
  // Carry out output actions.
}
if ( isStateTransition() ) {
  output = getExitOutput();
  // Carry out output actions.
  changeState();
  output = getEntryOutput()
  // Carry out output actions.
}
bool Lantern::nextState( void ) {
// Change to new state and/or output based on current state and input.
  uint8_t input = prioritiseInput();
  if ( next( input ) ) {
    return 1;
  }
  return 0;
}


bool Lantern::updateOutput( void ) {
// 
  uint8_t output = getOutput();
  switch ( output & 0xC0 ) {
    case LOWER_BRIGHTNESS:
      if ( getState() == INIT_DOWN ) {
        referenceBrightness = HALF_BRIGHTNESS;
      }
      else {
        referenceBrightness = ONE_BRIGHTNESS;
      }
      setStepSize( output );
      return 1;

    case RAISE_BRIGHTNESS:
      referenceBrightness = FULL_BRIGHTNESS;
      setStepSize( output );
      return 1;

    case SET_DELAY:
      setDelay();
      return 0;
  }

  switch ( output ) {
    case NO_CHANGE:
      return 0;

    case START_FLICKER:
      referenceBrightness = getBrightness();

    case FLICKER:
      if ( brightness == 0 ) {
        brightness = Random::pull(4);
      }
      return 0;

    case PULSE:
      referenceBrightness = getBrightness();
      return 0;

    case SET_BRIGHTNESS_TO_ZERO:
      setBrightness( 0 );
      return 1;

    case SET_BRIGHTNESS_TO_FULL:
      setBrightness( FULL_BRIGHTNESS );
      return 1;

    case SET_BRIGHTNESS_TO_REF:
      setBrightness( referenceBrightness );
      return 1;

    case SET_REFERENCE_TO_ZERO:
      referenceBrightness = 0;
      return 0;

    case TRACK_REFERENCE:
      setStepSize( 0 );
      return 0;
    
    case MAKE_TREE:
      return 0;

    case REDUCE_DELAY:
      delay--;
      return 0;

    case TRACK_PARENT:
      referenceBrightness = parent->getReferenceBrightness();
      setStepSize( 1 );
      setOutput( TRACK_REFERENCE );
      return 0;

    case LEAVE_TREE:
      setParent( nullptr );
      return 0;
  }
  return 0;
}


bool Lantern::changeBrightness( void ) {
  switch ( getOutput() ) {
    case NO_CHANGE:
      return 0;

    case START_FLICKER:
      brightness ^= brightness << 5; 
      brightness ^= brightness >> 7;
      return 1;

    case FLICKER:
      brightness ^= brightness << 5; 
      brightness ^= brightness >> 7;
      return 1;

    case PULSE:
      if ( getBrightness() == 1 ) {
        setBrightness( 0 );
        return 1;
      }
      if ( getBrightness() == 0 ) {
        setBrightness( referenceBrightness );
        setOutput( NO_CHANGE );
        return 1;
      }
      setBrightness( 1 );
      return 1;

    case TRACK_REFERENCE:
      if ( getBrightness() > referenceBrightness ) {
        lowerBrightness();
        return 1;
      }
      if ( getBrightness() < referenceBrightness ) {
        raiseBrightness();
        return 1;
      }
  }

  switch ( getOutput() & 0xC0 ) {
    case LOWER_BRIGHTNESS:
      lowerBrightness();

    case RAISE_BRIGHTNESS:
      raiseBrightness();
  }
  return 0;
}


void Lantern::lowerBrightness() {
  if ( isUndershoot() ) {
    setOutput( NO_CHANGE );
    setBrightness( referenceBrightness );
    return 1;
  }
  brightness -= getStepSize();
  return 1;
}


void Lantern::raiseBrightness() {
  if ( isOvershoot() ) {
    setOutput( NO_CHANGE );
    setBrightness( referenceBrightness );
    return 1;
  }
  brightness += getStepSize();
  return 1;
}


void Lantern::burnDown() { // TODO change so that lanterns stay near full brightness for longer.
// Expected value = -0.5
  if ( not getBrightness() ) {
    return;
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


uint8_t Lantern::getReferenceBrightness( void ) {
  return referenceBrightness;
}


uint8_t Lantern::getBrightness( void ) {
  return brightness >> 4;
}


void Lantern::setBrightness( uint8_t value ) {
  brightness &= ~0x0FFF;
  brightness |= value << 4;
}


uint8_t Lantern::getStepSize( void ) {
// Calculate and return step size.
  uint16_t exponent = ( brightness >> 12 );
  if ( exponent < 8 ) {
    return 1 << exponent;
  }
  return 0xFF;
}


void Lantern::setStepSize( uint8_t value ) {
  brightness &= ~0xF000;
  brightness |= ( value & 0xF ) << 12;
}


void Lantern::setDelay( void ) {
  if ( getOutput() & RANDOM_DELAY ) {
    delay = Random::pull( getOutput() & 0x3 );
  }
  else {
    delay = 1 << ( getOutput() & 0x3 );
  }
}


int Lantern::getDelay( void ) {
  return delay;
}


Lantern* Lantern::getParent() {
  return parent;
}


void Lantern::setParent( Lantern* parent ) {
  this->parent = parent;
}


bool Lantern::isUndershoot ( void ) {
  return 16 * ( getBrightness() - referenceBrightness ) < getStepSize();
}


bool Lantern::isOvershoot ( void ) {
  return 16 * ( referenceBrightness - getBrightness() ) < getStepSize();
}
#endif
    AUTO            |  ENTRY                                    |  STEP_SET_TO_ONE                      |  0
  , AUTO            |  LIGHT_EQ_FULL                            |  0                                    |  PAUSE
  , AUTO            |  LIGHT_EQ_ONE                             |  0                                    |  PAUSE
  , AUTO            |  NO_PARENT                                |  LIGHT_RAISE                          |  0
  , AUTO            |  PARENT_NOT_AUTO | LIGHT_EQ_PARENTS       |  0                                    |  PAUSE
  , AUTO            |  SENSOR_RISING_EDGE | NO_PARENT           |  0                                    |  PAUSE
  , AUTO            |  STEP_IS_DOWN | NO_PARENT                 |  LIGHT_LOWER                          |  0
  , AUTO            |  TIMER_OVER | LIGHT_GT_PARENTS            |  LIGHT_LOWER                          |  0
  , AUTO            |  TIMER_OVER | LIGHT_LT_PARENTS            |  LIGHT_RAISE                          |  0

joid Lantern::autoState( void ) {
  if ( parent ) {
    if ( not isDelayOver() )           { return 0; }
    if ( light > parent->getLight() )  { lowerBrightness(); }
    if ( light < parent->getLight() )  { raiseBrightness(); }
  } 
    

  if ( getLight() == 1 )                                { changeState( PAUSE ); return 0; }
  if ( getLight() == LIGHT_FULL )                       { changeState( PAUSE ); return 0; }
  if ( not parent {
    if ( classifySensor() == SENSOR_RISING_EDGE )     { changeState( PAUSE ); return 0; }
  }
  if ( getLight() == parent->getLight() 
       and parent->getState() != AUTO )                 { changeState( PAUSE ); return 0; }
}


void Lantern::enterState( void ) {
  switch ( state ) {
    case AUTO:
      setStepsize( 1 );
      return 0;
  }
}

void Lantern::changeStateTo( uint8_t nextState ) {
  exitState( state );
  state = nextState;
  enterState( state );
}

}
