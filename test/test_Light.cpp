#include "Light.h"
#include "Light.cpp"
#include <iostream>

void test_Light( void ) {
  Light l;
}


void test_cast( void ) {
  Light l;
  l.set(50);
  std::cout << ( int ) l << std::endl;
  int x = l + 3;
  std::cout << x << std::endl;

}


void test_comparisons( void )
{
  Light l; l.set(50);
  Light m; m.set(50);
  Light n; n.set(30);

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
    Light l; l.set(50);
    if ( l + 5 == 55 )  { std::cout << "Light + int     OK" << std::endl; }
    if ( l - 5 == 45 )  { std::cout << "Light - int     OK" << std::endl; }
    if ( l * 2 == 100 ) { std::cout << "Light * int     OK" << std::endl; }
    if ( l / 2 == 25 )  { std::cout << "Light / int     OK" << std::endl; }
    
}


void test_lower( void )
{
    Light l; l.set(50);
    for ( int i=0; i<256; i++ ) {
        l.lower();
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
}


void test_raise( void )
{
    Light l; l.set(50);
    for ( int i=0; i<256; i++ ) {
        l.raise();
        std::cout << ( int ) l << " ";
    }
    std::cout << std::endl;
}


int main( void ) {
  test_Light();
  test_cast();
  test_comparisons();
  test_raise();
  test_lower();
  test_arithmetic();

  return 0;
}
