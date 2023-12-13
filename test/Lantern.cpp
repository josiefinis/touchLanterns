#include "Lantern.h"
#include "StateMachine.h"



// TODO Make flicker look better.
// TODO Make different variations of flicker or other effects.

#define TRANSITION_MATRIX_ROWS      44

const StateTransition matrix[ TRANSITION_MATRIX_ROWS ] = { 
  { OUT,                    RISING_EDGE,                INIT_UP,                                },
  { GO_OUT,                 DONT_CARE,                  OUT,                                    },
  { IDLE,                   RISING_EDGE,                INIT_DOWN,                              },
  { IDLE,                   ZERO_BRIGHTNESS,            INIT_DOWN,                              },
  { GO_IDLE,                DONT_CARE,                  IDLE,                                   },
  { INIT_DOWN,              FALLING_EDGE,               FULL_DOWN,                              },
  { INIT_DOWN,              MEDIUM_TOUCH,               FLICKER_DOWN,                           },
  { INIT_UP,                FALLING_EDGE,               FULL_UP,                                },
  { INIT_UP,                MEDIUM_TOUCH,               FLICKER_UP,                             },
  { FULL_DOWN,              DONT_CARE,                  GO_OUT,                                 },
  { FULL_UP,                DONT_CARE,                  GO_IDLE,                                },
  { FLICKER_DOWN,           FALLING_EDGE,               AUTO_DOWN,                              },
  { FLICKER_DOWN,           TOUCHED_LONG,               ROOT_FULL_DOWN,                         },
  { FLICKER_UP,             FALLING_EDGE,               AUTO_UP,                                },
  { FLICKER_UP,             TOUCHED_LONG,               ROOT_FULL_UP,                           },
  { AUTO_DOWN,              RISING_EDGE,                PAUSE_DOWN,                             },
  { AUTO_DOWN,              AT_MIN_BRIGHTNESS,          PAUSE_DOWN,                             },
  { AUTO_DOWN,              AT_MIN_BRIGHTNESS,          PAUSE_DOWN,                             },
  { AUTO_UP,                RISING_EDGE,                PAUSE_UP,                               },
  { AUTO_UP,                AT_MAX_BRIGHTNESS,          PAUSE_UP,                               },
  { PAUSE_DOWN,             RISING_EDGE,                AUTO_UP,                                },
  { PAUSE_DOWN,             MEDIUM_TOUCH,               ROOT_FLICKER_DOWN,                      },
  { PAUSE_DOWN,             NO_INPUT,                   IDLE,                                   },
  { PAUSE_UP,               RISING_EDGE,                AUTO_DOWN,                              },
  { PAUSE_UP,               MEDIUM_TOUCH,               ROOT_FLICKER_UP,                        },
  { PAUSE_UP,               NO_INPUT,                   IDLE,                                   },
  { ROOT_FULL_DOWN,         DONT_CARE,                  FULL_DOWN,                              },
  { ROOT_FULL_UP,           DONT_CARE,                  FULL_UP,                                },
  { ROOT_FLICKER_DOWN,      LONG_TOUCH_FALLING_EDGE,    ROOT_AUTO_DOWN                          },
  { ROOT_FLICKER_DOWN,      FALLING_EDGE,               AUTO_DOWN,                              },
  { ROOT_FLICKER_UP,        LONG_TOUCH_FALLING_EDGE,    ROOT_AUTO_UP,                           },
  { ROOT_FLICKER_UP,        FALLING_EDGE,               AUTO_UP,                                },
  { ROOT_AUTO_DOWN,         RISING_EDGE,                ROOT_PAUSE_DOWN,                        },
  { ROOT_AUTO_DOWN,         AT_MIN_BRIGHTNESS           ROOT_PAUSE_DOWN,                        },
  { ROOT_AUTO_UP,           RISING_EDGE,                ROOT_PAUSE_UP,                          },
  { ROOT_AUTO_UP,           AT_MAX_BRIGHTNESS           ROOT_PAUSE_UP,                          },
  { ROOT_PAUSE_DOWN,        RISING_EDGE,                ROOT_AUTO_UP,                           },
  { ROOT_PAUSE_DOWN,        NO_INPUT,                   IDLE,                                   },
  { ROOT_PAUSE_UP,          RISING_EDGE,                ROOT_AUTO_DOWN,                         },
  { ROOT_PAUSE_UP,          NO_INPUT,                   IDLE,                                   },
  { FOLLOW,                                                                                     },
  { WAIT,                                                                                       },
  { WAIT_FULL_DOWN,                                                                             },
  { WAIT_FULL_UP,                                                                               }
};
static const TransitionMatrix Lantern::transitionMatrix = TransitionMatrix( TRANSITION_MATRIX_ROWS, matrix );


Lantern::Lantern() 
  : StateMachine( transitionMatrix ), parent( nullptr ) {}


uint8_t Lantern::classifyInput( uint8_t value ) {
  switch ( value ) {
    case 0x00:
      return NO_INPUT;
    
    case 0xFF:
      return LONG_TOUCH;

    case 0xFE:
      return LONG_TOUCH_FALLING_EDGE;
  }
  switch ( value & 0xF ) {
    case 0x0F:
      return MEDIUM_TOUCH;
  }
  switch ( value & 0x3 ) {
    case 0x1:
      return RISING_EDGE;
    
    case 0x2:
      return FALLING_EDGE;
  }
  switch ( value & 1 ) {
    case 1:
      return IS_TOUCHED;
    
    case 0:
      return IS_RELEASED;
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


void Lantern::setParent(Lantern* pLantern) {
  parent = pLantern;
}


Lantern* Lantern::getParent() {
  return parent;
}


// ========================================================================================================================
// PRIVATE
// ========================================================================================================================


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
