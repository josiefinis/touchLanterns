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

#define TRANSITION_MATRIX_ROWS      77

const StateTransition matrix[ TRANSITION_MATRIX_ROWS ] = { 

    {     OUT                     , RISING_EDGE                   , INIT_UP                   , RAISE_BRIGHTNESS | 5          } 
  , {     GO_OUT                  , DONT_CARE                     , OUT                       , NO_CHANGE                     }  
  , {     IDLE                    , RISING_EDGE                   , INIT_DOWN                 , LOWER_BRIGHTNESS | 4          } 
  , {     IDLE                    , AT_ZERO_BRIGHTNESS            , GO_OUT                    , SET_REFERENCE_TO_ZERO         } 
  , {     IDLE                    , AT_ZERO_DELAY                 , IDLE                      , TRACK_REFERENCE               } 
  , {     IDLE                    , DONT_CARE                     , IDLE                      , REDUCE_DELAY                  }
  , {     GO_IDLE                 , DONT_CARE                     , IDLE                      , SET_REF_TO_BRIGHTNESS         } 

  , {     INIT_DOWN               , FALLING_EDGE                  , FULL_DOWN                 , LOWER_BRIGHTNESS | 8          } 
  , {     INIT_DOWN               , MEDIUM_TOUCH                  , FLICKER_DOWN              , START_FLICKER                 } 
  , {     INIT_DOWN               , DONT_CARE                     , INIT_DOWN                 , LOWER_BRIGHTNESS | 3          } 
    
  , {     INIT_UP                 , FALLING_EDGE                  , FULL_UP                   , RAISE_BRIGHTNESS | 8          } 
  , {     INIT_UP                 , MEDIUM_TOUCH                  , FLICKER_UP                , START_FLICKER                 } 
  , {     INIT_UP                 , DONT_CARE                     , INIT_UP                   , RAISE_BRIGHTNESS | 5          } 

  , {     FULL_DOWN               , DONT_CARE                     , GO_OUT                    , SET_BRIGHTNESS_TO_ZERO        } 
  , {     FULL_UP                 , DONT_CARE                     , GO_IDLE                   , SET_BRIGHTNESS_TO_FULL        } 

  , {     FLICKER_DOWN            , FALLING_EDGE                  , AUTO_DOWN                 , SET_BRIGHTNESS_TO_REF         } 
  , {     FLICKER_DOWN            , LONG_TOUCH                    , ROOT_FULL_DOWN            , MAKE_TREE                     } 
  , {     FLICKER_DOWN            , DONT_CARE                     , FLICKER_DOWN              , FLICKER                       } 
    
  , {     FLICKER_UP              , FALLING_EDGE                  , AUTO_UP                   , SET_BRIGHTNESS_TO_REF         } 
  , {     FLICKER_UP              , LONG_TOUCH                    , ROOT_FULL_UP              , MAKE_TREE                     } 
  , {     FLICKER_UP              , DONT_CARE                     , FLICKER_UP                , FLICKER                       } 

  , {     AUTO_DOWN               , RISING_EDGE                   , PAUSE_DOWN                , PULSE                         } 
  , {     AUTO_DOWN               , AT_ONE_BRIGHTNESS             , PAUSE_DOWN                , SET_DELAY | 8                 } 
  , {     AUTO_DOWN               , DONT_CARE                     , AUTO_DOWN                 , LOWER_BRIGHTNESS | 1          } 

  , {     AUTO_UP                 , RISING_EDGE                   , PAUSE_UP                  , PULSE                         } 
  , {     AUTO_UP                 , AT_FULL_BRIGHTNESS            , PAUSE_UP                  , SET_DELAY | 8                 } 
  , {     AUTO_UP                 , DONT_CARE                     , AUTO_UP                   , RAISE_BRIGHTNESS | 1          } 

  , {     PAUSE_DOWN              , RISING_EDGE                   , AUTO_UP                   , PULSE                         } 
  , {     PAUSE_DOWN              , FALLING_EDGE                  , PAUSE_DOWN                , SET_DELAY | 8                 } 
  , {     PAUSE_DOWN              , MEDIUM_TOUCH                  , ROOT_FLICKER_DOWN         , START_FLICKER                 } 
  , {     PAUSE_DOWN              , AT_ZERO_DELAY                 , GO_IDLE                   , REDUCE_DELAY                  } 
  , {     PAUSE_DOWN              , DONT_CARE                     , PAUSE_DOWN                , REDUCE_DELAY                  } 

  , {     PAUSE_UP                , RISING_EDGE                   , AUTO_DOWN                 , PULSE                         } 
  , {     PAUSE_UP                , FALLING_EDGE                  , PAUSE_UP                  , SET_DELAY | 8                 } 
  , {     PAUSE_UP                , MEDIUM_TOUCH                  , ROOT_FLICKER_UP           , START_FLICKER                 } 
  , {     PAUSE_UP                , AT_ZERO_DELAY                 , GO_IDLE                   , REDUCE_DELAY                  } 
  , {     PAUSE_UP                , DONT_CARE                     , PAUSE_UP                  , REDUCE_DELAY                  } 

  , {     ROOT_FULL_DOWN          , LONG_TOUCH_FALLING_EDGE       , FULL_DOWN                 , LOWER_BRIGHTNESS | 8          } 
  , {     ROOT_FULL_DOWN          , DONT_CARE                     , ROOT_FULL_DOWN            , LOWER_BRIGHTNESS | 8          } 
    
  , {     ROOT_FULL_UP            , LONG_TOUCH_FALLING_EDGE       , FULL_UP                   , RAISE_BRIGHTNESS | 8          } 
  , {     ROOT_FULL_UP            , DONT_CARE                     , ROOT_FULL_UP              , RAISE_BRIGHTNESS | 8          } 

  , {     ROOT_FLICKER_DOWN       , FALLING_EDGE                  , AUTO_DOWN                 , SET_BRIGHTNESS_TO_REF         } 
  , {     ROOT_FLICKER_DOWN       , LONG_TOUCH_FALLING_EDGE       , ROOT_AUTO_DOWN            , MAKE_TREE                     } 
  , {     ROOT_FLICKER_DOWN       , DONT_CARE                     , ROOT_FLICKER_DOWN         , FLICKER                       } 
    
  , {     ROOT_FLICKER_UP         , FALLING_EDGE                  , AUTO_UP                   , SET_BRIGHTNESS_TO_REF         } 
  , {     ROOT_FLICKER_UP         , LONG_TOUCH_FALLING_EDGE       , ROOT_AUTO_UP              , MAKE_TREE                     } 
  , {     ROOT_FLICKER_UP         , DONT_CARE                     , ROOT_FLICKER_UP           , FLICKER                       } 

  , {     ROOT_AUTO_DOWN          , RISING_EDGE                   , ROOT_PAUSE_DOWN           , PULSE                         } 
  , {     ROOT_AUTO_DOWN          , AT_ONE_BRIGHTNESS             , ROOT_PAUSE_DOWN           , SET_DELAY | 8                 } 
  , {     ROOT_AUTO_DOWN          , DONT_CARE                     , ROOT_AUTO_DOWN            , LOWER_BRIGHTNESS | 1          } 

  , {     ROOT_AUTO_UP            , RISING_EDGE                   , ROOT_PAUSE_UP             , PULSE                         } 
  , {     ROOT_AUTO_UP            , AT_FULL_BRIGHTNESS            , ROOT_PAUSE_UP             , SET_DELAY | 8                 } 
  , {     ROOT_AUTO_UP            , DONT_CARE                     , ROOT_AUTO_UP              , RAISE_BRIGHTNESS | 1          } 

  , {     ROOT_PAUSE_DOWN         , RISING_EDGE                   , ROOT_AUTO_UP              , PULSE                         } 
  , {     ROOT_PAUSE_DOWN         , FALLING_EDGE                  , ROOT_PAUSE_DOWN           , SET_DELAY | 8                 } 
  , {     ROOT_PAUSE_DOWN         , AT_ZERO_DELAY                 , GO_IDLE                   , REDUCE_DELAY                  } 
  , {     ROOT_PAUSE_DOWN         , DONT_CARE                     , ROOT_PAUSE_DOWN           , REDUCE_DELAY                  } 

  , {     ROOT_PAUSE_UP           , RISING_EDGE                   , ROOT_AUTO_DOWN            , PULSE                         } 
  , {     ROOT_PAUSE_UP           , FALLING_EDGE                  , ROOT_PAUSE_UP             , SET_DELAY | 8                 } 
  , {     ROOT_PAUSE_UP           , AT_ZERO_DELAY                 , GO_IDLE                   , REDUCE_DELAY                  } 
  , {     ROOT_PAUSE_UP           , DONT_CARE                     , ROOT_PAUSE_UP             , REDUCE_DELAY                  } 

  , {     WAIT                    , AT_ZERO_DELAY                 , FOLLOW                    , REDUCE_DELAY                  } 
  , {     WAIT                    , PARENT_IS_FULL_DOWN           , WAIT_FULL_DOWN            , REDUCE_DELAY                  } 
  , {     WAIT                    , PARENT_IS_FULL_UP             , WAIT_FULL_UP              , REDUCE_DELAY                  } 
  , {     WAIT                    , DONT_CARE                     , WAIT                      , REDUCE_DELAY                  } 

  , {     WAIT_FULL_DOWN          , AT_ZERO_DELAY                 , FULL_DOWN                 , REDUCE_DELAY                  } 
  , {     WAIT_FULL_DOWN          , PARENT_IS_FULL_UP             , WAIT_FULL_UP              , REDUCE_DELAY                  } 
  , {     WAIT_FULL_DOWN          , PARENT_IS_WAIT_OR_FOLLOW      , WAIT                      , REDUCE_DELAY                  } 
  , {     WAIT_FULL_DOWN          , DONT_CARE                     , WAIT_FULL_DOWN            , REDUCE_DELAY                  } 

  , {     WAIT_FULL_UP            , AT_ZERO_DELAY                 , FULL_UP                   , REDUCE_DELAY                  } 
  , {     WAIT_FULL_UP            , PARENT_IS_FULL_DOWN           , WAIT_FULL_DOWN            , REDUCE_DELAY                  } 
  , {     WAIT_FULL_UP            , PARENT_IS_WAIT_OR_FOLLOW      , WAIT                      , REDUCE_DELAY                  } 
  , {     WAIT_FULL_UP            , DONT_CARE                     , WAIT_FULL_UP              , REDUCE_DELAY                  } 

  , {     FOLLOW                  , PARENT_IS_IDLE                , GO_IDLE                   , LEAVE_TREE                    }
  , {     FOLLOW                  , DONT_CARE                     , FOLLOW                    , TRACK_PARENT                  }
};
const TransitionMatrix Lantern::transitionMatrix = TransitionMatrix( TRANSITION_MATRIX_ROWS, matrix );


Lantern::Lantern() 
  : StateMachine( transitionMatrix )
  , sensorRegister( 0 )
  , brightness( 0 )
  , referenceBrightness( 0 )
  , delay( 0xFF )
  , parent( nullptr ) 
{}


void Lantern::pushSensor( bool value ) {
  sensorRegister <<= 1;
  sensorRegister |= value;
}


#define EDGE    0x3
uint8_t Lantern::classifySensorInput( void ) {
  switch ( sensorRegister ) {
    case 0xFF:
      return LONG_TOUCH;

    case 0xFE:
      return LONG_TOUCH_FALLING_EDGE;

    case 0x0F:
      return MEDIUM_TOUCH;
  }
  switch ( sensorRegister & EDGE ) {
    case 0x1:
      return RISING_EDGE;
    
    case 0x2:
      return FALLING_EDGE;
  }
  return DONT_CARE;
}


uint8_t Lantern::classifyBrightnessInput( void ) {
  switch ( getBrightness() ) {
    case 0:
      return AT_ZERO_BRIGHTNESS;

    case ONE_BRIGHTNESS:
      return AT_ONE_BRIGHTNESS;

    case FULL_BRIGHTNESS:
      return AT_FULL_BRIGHTNESS;
  }
  return DONT_CARE;
}


uint8_t Lantern::classifyParentInput( void ) {
  switch ( parent->getState() ) {
    case IDLE:
      return PARENT_IS_IDLE; 

    case FULL_DOWN:
      return PARENT_IS_FULL_DOWN; 

    case WAIT_FULL_DOWN:
      return PARENT_IS_FULL_DOWN; 

    case FULL_UP:
      return PARENT_IS_FULL_UP; 

    case WAIT_FULL_UP:
      return PARENT_IS_FULL_UP; 

    case WAIT:
      return PARENT_IS_WAIT_OR_FOLLOW; 

    case FOLLOW:
      return PARENT_IS_WAIT_OR_FOLLOW; 
  }
  return DONT_CARE;
}


uint8_t Lantern::prioritiseInput( void ) {
  if ( delay == 0 ) {                                    
    return AT_ZERO_DELAY;
  }
  if ( parent ) {
    return classifyParentInput();
  }
  if ( classifySensorInput() == DONT_CARE ) {
    return classifyBrightnessInput();
  }
  return classifySensorInput();
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
  switch ( getOutput() & 0xC0 ) {
    case LOWER_BRIGHTNESS:
      referenceBrightness = ONE_BRIGHTNESS;
      setStepSize();
      return 1;

    case RAISE_BRIGHTNESS:
      referenceBrightness = FULL_BRIGHTNESS;
      setStepSize();
      return 1;

    case SET_DELAY:
      setDelay();
      return 0;
  }

  switch ( getOutput() ) {
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
      setOutput( 0 );
      setStepSize();
      delay--;
      return 0;
    
    case MAKE_TREE:
      return 0;

    case REDUCE_DELAY:
      delay--;
      return 0;

    case TRACK_PARENT:
      setOutput( 1 );
      setStepSize();
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
      brightness ^= brightness << 2; 
      brightness ^= brightness >> 7;
      return 1;

    case FLICKER:
      brightness ^= brightness << 2; 
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
        brightness -= getStepSize();
        return 1;
      }
      if ( getBrightness() < referenceBrightness ) {
        brightness += getStepSize();
        return 1;
      }

    case TRACK_PARENT:
      if ( not parent ) { return 0; };
      if ( getBrightness() > parent->getBrightness() ) {
        brightness -= getStepSize();
        return 1;
      }
      if ( getBrightness() < parent->getBrightness() ) {
        brightness += getStepSize();
        return 1;
      }
  }

  switch ( getOutput() & 0xC0 ) {
    case LOWER_BRIGHTNESS:
      if ( isUndershoot() ) {
        setOutput( NO_CHANGE );
        setBrightness( referenceBrightness );
        return 1;
      }
      brightness -= getStepSize();
      return 1;

    case RAISE_BRIGHTNESS:
      if ( isOvershoot() ) {
        setOutput( NO_CHANGE );
        setBrightness( referenceBrightness );
        return 1;
      }
      brightness += getStepSize();
      return 1;
  }
  return 0;
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


uint8_t Lantern::getBrightness( void ) {
  return brightness >> 4;
}


void Lantern::setBrightness( uint8_t value ) {
  brightness &= ~0x0FFF;
  brightness |= value << 4;
}


uint8_t Lantern::getStepSize( void ) {
// Calculate and return step size.
  uint8_t exponent = ( brightness >> 12 );
  if ( exponent < 8 ) {
    return 1 << exponent;
  }
  return 0xFF;
}


void Lantern::setStepSize( void ) {
  brightness &= ~0xF000;
  brightness |= ( getOutput() & 0xF ) << 12;
}


void Lantern::setDelay( void ) {
  delay = getOutput() & 0x3F;
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
