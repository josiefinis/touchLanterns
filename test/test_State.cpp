/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/

#include <cstdint>
#include <iostream>


#ifndef STATE_H
#define STATE_H


#define IDLE_ID     0
#define INIT_ID     1
#define FULL_ID     2
#define FLKR_ID     3
#define AUTO_ID     4
#define PAUS_ID     5


class State
{
    public:
        State( const uint8_t id )
        : id( id )
        { }
        bool operator == ( const State& other )   const { return id == other.id; }
        bool operator == ( const uint8_t& other ) const { return id == other; }
        bool operator != ( const State& other )   const { return !( *this == other.id ); }
        bool operator != ( const uint8_t& other ) const { return !( *this == other ); }

        virtual void enter( void ) {}
        virtual void exit( void ) {}
        virtual void act( void ) {}
        virtual uint8_t getNext( void ) { return 0; } 

    private:
        const uint8_t id;
};
#endif


class Idle : public State
{
    public:
        Idle() : State( IDLE_ID ) { }

        void enter( void )
        {
            std::cout << "enter IDLE" << std::endl;
        }
        void exit( void )
        {
            std::cout << "exit IDLE" << std::endl;
        }
        void act( void )
        {
            std::cout << "input to IDLE" << std::endl;
        }
        uint8_t getNext( void )
        {
            return INIT_ID;
        }
};


class Init : public State
{
    public:
        Init() : State( INIT_ID ) { }

        void enter( void )
        {
            std::cout << "enter INIT" << std::endl;
        }
        void exit( void )
        {
            std::cout << "exit INIT" << std::endl;
        }
        void act( void )
        {
            std::cout << "input to INIT" << std::endl;
        }
        uint8_t getNext( void )
        {
            return IDLE_ID;
        }
};



class Auto : public State
{
    public:
        Auto()
            : State( AUTO_ID )
        { }
        void enter( void )
        {
            std::cout << "enter AUTO" << std::endl;
        }
        void exit( void )
        {
            std::cout << "exit AUTO" << std::endl;
        }
        void act( void )
        {
            std::cout << "input to AUTO" << std::endl;
        }
        uint8_t getNext( void )
        {
            return PAUS_ID;
        }
};

Idle IDLE;
Init INIT;
Auto AUTO;

int main( void )
{
    State* state = &IDLE;
    for ( int i=0; i<16; i++ )
    {
        state->act();
        if ( *state == state->getNext() ) { continue; }
        state->exit();
        switch ( state->getNext() )
        {
            case IDLE_ID:
                state = &IDLE;
                state->enter();
                break;

            case INIT_ID:
                state = &INIT;
                state->enter();
                break;
        }
    }

    return 0;
}
