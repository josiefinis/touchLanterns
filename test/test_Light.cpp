#include "Light.h"
#include "Light.cpp"
#include <iostream>

void test_Light( void ) {
  Light l;
}


void test_cast( void ) {
  Light l;
  l.setBrightness(50);
  std::cout << ( int ) l << std::endl;
  int x = l + 3;
  std::cout << x << std::endl;

}


void test_comparisons( void )
{
  Light l; l.setBrightness(50);
  Light m; m.setBrightness(50);
  Light n; n.setBrightness(30);

  if ( l == 50 ) { std::cout << "Light == int       OK" << std::endl; }
  if ( m == l )  { std::cout << "Light == Light     OK" << std::endl; }
  if ( l != 40 ) { std::cout << "Light != int       OK" << std::endl; }
  if ( m != n )  { std::cout << "Light != Light     OK" << std::endl; }
  if ( n <  50 ) { std::cout << "Light <  int       OK" << std::endl; }
  if ( n <  l )  { std::cout << "Light <  Light     OK" << std::endl; }
  if ( l >  30 ) { std::cout << "Light >  int       OK" << std::endl; }
  if ( m >  n )  { std::cout << "Light >  Light     OK" << std::endl; }
  if ( l <= 50 ) { std::cout << "Light <= int       OK" << std::endl; }
  if ( m <= l )  { std::cout << "Light <= Light     OK" << std::endl; }
  if ( l >= 50 ) { std::cout << "Light >= int       OK" << std::endl; }
  if ( m >= n )  { std::cout << "Light >= Light     OK" << std::endl; }
}


void test_arithmetic( void ) 
{
    Light l; l.setBrightness(50);
    if ( l + 5 == 55 )  { std::cout << "Light + int     OK" << std::endl; }
    if ( l - 5 == 45 )  { std::cout << "Light - int     OK" << std::endl; }
    if ( l * 2 == 100 ) { std::cout << "Light * int     OK" << std::endl; }
    if ( l / 2 == 25 )  { std::cout << "Light / int     OK" << std::endl; }
}


void test_assignment_from( void ) 
{
    Light l; l.setBrightness(50);
    uint8_t ref;
    ref = l;
    std::cout << ( int ) ref << std::endl;
}


void test_lowerBrightness( void )
{
    Light q; q.setBrightness(50);
    std::cout << "rate: " << ( int ) q.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        q.lowerBrightness( 45 );
        std::cout << ( int ) q << " ";
    }
    std::cout << std::endl;
    std::cout << "rate: " << ( int ) q.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        q.lowerBrightness( 80 );
        std::cout << ( int ) q << " ";
    }
    q.setRate(7);
    std::cout << std::endl;
    std::cout << "rate: " << ( int ) q.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        q.lowerBrightness();
        std::cout << ( int ) q << " ";
    }
    std::cout << std::endl;
}


void test_raiseBrightness( void )
{
    Light l; l.setBrightness(50);
    l.setRate(7);
    std::cout << "rate: " << ( int ) l.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        l.raiseBrightness( 55 );
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
    std::cout << "rate: " << ( int ) l.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        l.raiseBrightness( 30 );
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
    std::cout << "rate: " << ( int ) l.getRate() << std::endl;
    for ( int i=0; i<256; i++ ) {
        l.raiseBrightness();
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
}


void test_toggleSign( void )
{
    Light l; 
    std::cout << "isDimming: " << ( int ) l.isDimming() << std::endl;
    std::cout << "isBrightnening: " << ( int ) l.isBrightening() << std::endl;
    l.toggleSign();
    std::cout << "isDimming: " << ( int ) l.isDimming() << std::endl;
    std::cout << "isBrightnening: " << ( int ) l.isBrightening() << std::endl;

    l.setRate(3);
    std::cout << "rate: " << ( int ) l.getRate() << std::endl;
    l.toggleSign();
    std::cout << "rate: " << ( int ) l.getRate() << std::endl;
    l.toggleSign();
    for ( int i=0; i<400; i++ ) {
        l.changeBrightness();
        if ( i % 50 == 49 ) { 
            l.toggleSign(); 
            std::cout << "rate: " << ( int ) l.getRate() << std::endl;
        }
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
}


int main( void ) {
//  test_Light();
//  test_cast();
//  test_comparisons();
    test_raiseBrightness();
    test_lowerBrightness();
//  test_toggleSign();
//  test_arithmetic();
//  test_assignment_from();

  return 0;
}

