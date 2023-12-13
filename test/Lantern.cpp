/* 
======================================================================================================================================================
                    LANTERN
======================================================================================================================================================
*/ 

#include "Lantern.h"
#include "StateMachine.cpp"
#include "Random.cpp"

// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.

#define TRANSITION_MATRIX_ROWS      53

const StateTransition matrix[ TRANSITION_MATRIX_ROWS ] = { 
  { OUT                   , RISING_EDGE               , INIT_UP               , RAISE_BRIGHTNESS | 5    }, 
  { GO_OUT                , DONT_CARE                 , OUT                   , SET_REFERENCE_TO_0      },  
  { IDLE                  , RISING_EDGE               , INIT_DOWN             , LOWER_BRIGHTNESS | 5    }, 
  { IDLE                  , AT_ZERO_BRIGHTNESS        , GO_OUT                , SET_REFERENCE_TO_0      }, 
  { GO_IDLE               , DONT_CARE                 , IDLE                  , TRACK_REFERENCE         }, 
  { INIT_DOWN             , FALLING_EDGE              , FULL_DOWN             , LOWER_BRIGHTNESS | 8    }, 
  { INIT_DOWN             , MEDIUM_TOUCH              , FLICKER_DOWN          , FLICKER                 }, 
  { INIT_DOWN             , DONT_CARE                 , INIT_DOWN             , LOWER_BRIGHTNESS | 5    }, 
  { INIT_UP               , FALLING_EDGE              , FULL_UP               , RAISE_BRIGHTNESS | 8    }, 
  { INIT_UP               , MEDIUM_TOUCH              , FLICKER_UP            , FLICKER                 }, 
  { INIT_UP               , DONT_CARE                 , INIT_UP               , RAISE_BRIGHTNESS | 5    }, 
  { FULL_DOWN             , DONT_CARE                 , GO_OUT                , SET_BRIGHTNESS_TO_0     }, 
  { FULL_UP               , DONT_CARE                 , GO_IDLE               , SET_BRIGHTNESS_TO_MAX   }, 
  { FLICKER_DOWN          , FALLING_EDGE              , AUTO_DOWN             , SET_BRIGHTNESS_TO_REF   }, 
  { FLICKER_DOWN          , LONG_TOUCH                , ROOT_FULL_DOWN        , MAKE_TREE               }, 
  { FLICKER_DOWN          , DONT_CARE                 , FLICKER               , FLICKER                 }, 
  { FLICKER_UP            , FALLING_EDGE              , AUTO_UP               , SET_BRIGHTNESS_TO_REF   }, 
  { FLICKER_UP            , LONG_TOUCH                , ROOT_FULL_UP          , MAKE_TREE               }, 
  { FLICKER_UP            , DONT_CARE                 , FLICKER_UP            , FLICKER                 }, 
  { AUTO_DOWN             , RISING_EDGE               , PAUSE_DOWN            , PULSE                   }, 
  { AUTO_DOWN             , AT_MIN_BRIGHTNESS         , PAUSE_DOWN                                      }, 
  { AUTO_DOWN             , DONT_CARE                 , AUTO_DOWN             , LOWER_BRIGHTNESS | 1    }, 
  { AUTO_UP               , RISING_EDGE               , PAUSE_UP              , PULSE                   }, 
  { AUTO_UP               , AT_MAX_BRIGHTNESS         , PAUSE_UP                                        }, 
  { AUTO_UP               , DONT_CARE                 , AUTO_UP               , RAISE_BRIGHTNESS | 1    }, 
  { PAUSE_DOWN            , RISING_EDGE               , AUTO_UP               , PULSE                   }, 
  { PAUSE_DOWN            , MEDIUM_TOUCH              , ROOT_FLICKER_DOWN     , FLICKER                 }, 
  { PAUSE_DOWN            , LONG_RELEASE              , IDLE                                            }, 
  { PAUSE_UP              , RISING_EDGE               , AUTO_DOWN             , PULSE                   }, 
  { PAUSE_UP              , MEDIUM_TOUCH              , ROOT_FLICKER_UP       , FLICKER                 }, 
  { PAUSE_UP              , LONG_RELEASE              , IDLE                                            }, 
  { ROOT_FULL_DOWN        , DONT_CARE                 , FULL_DOWN                                       }, 
  { ROOT_FULL_UP          , DONT_CARE                 , FULL_UP                                         }, 
  { ROOT_FLICKER_DOWN     , FALLING_EDGE              , AUTO_DOWN             , SET_BRIGHTNESS_TO_REF   }, 
  { ROOT_FLICKER_DOWN     , LONG_TOUCH_FALLING_EDGE   , ROOT_AUTO_DOWN        , MAKE_TREE               }, 
  { ROOT_FLICKER_DOWN     , DONT_CARE                 , ROOT_FLICKER_DOWN     , FLICKER                 }, 
  { ROOT_FLICKER_UP       , FALLING_EDGE              , AUTO_UP               , SET_BRIGHTNESS_TO_REF   }, 
  { ROOT_FLICKER_UP       , LONG_TOUCH_FALLING_EDGE   , ROOT_AUTO_UP          , MAKE_TREE               }, 
  { ROOT_FLICKER_UP       , DONT_CARE                 , ROOT_FLICKER_UP       , FLICKER                 }, 
  { ROOT_AUTO_DOWN        , RISING_EDGE               , ROOT_PAUSE_DOWN       , PULSE                   }, 
  { ROOT_AUTO_DOWN        , AT_MIN_BRIGHTNESS         , ROOT_PAUSE_DOWN                                 }, 
  { ROOT_AUTO_DOWN        , DONT_CARE                 , ROOT_AUTO_DOWN        , LOWER_BRIGHTNESS | 1    }, 
  { ROOT_AUTO_UP          , RISING_EDGE               , ROOT_PAUSE_UP         , PULSE                   }, 
  { ROOT_AUTO_UP          , AT_MAX_BRIGHTNESS         , ROOT_PAUSE_UP                                   }, 
  { ROOT_AUTO_UP          , DONT_CARE                 , ROOT_AUTO_UP          , RAISE_BRIGHTNESS | 1    }, 
  { ROOT_PAUSE_DOWN       , RISING_EDGE               , ROOT_AUTO_UP          , PULSE                   }, 
  { ROOT_PAUSE_DOWN       , LONG_RELEASE              , IDLE                                            }, 
  { ROOT_PAUSE_UP         , RISING_EDGE               , ROOT_AUTO_DOWN        , PULSE                   }, 
  { ROOT_PAUSE_UP         , LONG_RELEASE              , IDLE                                            }, 
  { WAIT                                                                                                }, 
  { WAIT_FULL_DOWN                                                                                      }, 
  { WAIT_FULL_UP                                                                                        }, 
  { FOLLOW                                                                                              }
};
const TransitionMatrix Lantern::transitionMatrix = TransitionMatrix( TRANSITION_MATRIX_ROWS, matrix );


Lantern::Lantern() 
  : StateMachine( transitionMatrix )
  , inputRegister( 0 )
  , brightness( 0 )
  , referenceBrightness( 0 )
  , parent( nullptr ) 
{}


void Lantern::pushInput( bool value ) {
  inputRegister <<= 1;
  inputRegister |= value;
}


#define EDGE    0x3
#define NOW     0x1
uint8_t Lantern::classifyInput( void ) {
  switch ( inputRegister ) {
    case 0x00:
      return LONG_RELEASE;
    
    case 0xFF:
      return LONG_TOUCH;

    case 0xFE:
      return LONG_TOUCH_FALLING_EDGE;

    case 0x0F:
      return MEDIUM_TOUCH;
  }

  switch ( inputRegister & EDGE ) {
    case 0x1:
      return RISING_EDGE;
    
    case 0x2:
      return FALLING_EDGE;
  }
  return DONT_CARE;
}


bool Lantern::nextState( void ) {
// Change to new state and/or output based on current state and sensor input.
  if ( next( classifyInput() ) ) {
    return 1;
  }
  return 0;
}


bool Lantern::updateOutput( void ) {
// 
  switch ( getOutput() ) {
    case NO_CHANGE:
      return 0;

    case FLICKER:
      referenceBrightness = getBrightness();
      return 0;

    case PULSE:
      referenceBrightness = getBrightness();
      return 0;

    case SET_BRIGHTNESS_TO_0:
      setBrightness( 0 );
      return 1;

    case SET_BRIGHTNESS_TO_MAX:
      setBrightness( MAX_BRIGHTNESS );
      return 1;

    case SET_BRIGHTNESS_TO_REF:
      setBrightness( referenceBrightness );
      return 1;

    case SET_REFERENCE_TO_0:
      referenceBrightness = 0;
      return 0;

    case TRACK_REFERENCE:
      return 1;
    
    case MAKE_TREE:
      return 0;
  }
  switch ( getOutput() & 0xC0 ) {
    case LOWER_BRIGHTNESS:
      referenceBrightness = MIN_BRIGHTNESS;
      setStepSize();
      return 1;

    case RAISE_BRIGHTNESS:
      referenceBrightness = MAX_BRIGHTNESS;
      setStepSize();
      return 1;
  }
  return 0;
}


bool Lantern::changeBrightness( void ) {
  switch ( getOutput() ) {
    case NO_CHANGE:
      return 0;

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


Lantern* Lantern::getParent() {
  return parent;
}


void Lantern::setParent(Lantern* pLantern) {
  parent = pLantern;
}


bool Lantern::isUndershoot ( void ) {
  return 16 * ( getBrightness() - referenceBrightness ) < getStepSize();
}


bool Lantern::isOvershoot ( void ) {
  return 16 * ( referenceBrightness - getBrightness() ) < getStepSize();
}
