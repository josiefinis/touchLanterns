/*
======================================================================================================================================================
                    STATE
======================================================================================================================================================
*/



#include <cstdint>
#include <iostream>
#include "Light.h"



#ifndef STATE_H
#define STATE_H


#define IDLE_ID     0
#define WAKE_ID     1
#define FULL_ID     2
#define FLKR_ID     3
#define AUTO_ID     4
#define PAUS_ID     5


class Lantern;


class State
{
    public:
        State ( const uint8_t id );

        operator int() const;
        virtual void enter( Lantern& );
        virtual void exit( Lantern& );
        virtual uint8_t act( Lantern& );
        virtual uint8_t getNext( Lantern& );

    private:
        const uint8_t id;
};


class Idle : public State
{
    public:
        Idle();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};


class Wake : public State
{
    public:
        Wake();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};


class Full : public State
{
    public:
        Full();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};


class Flicker : public State
{
    public:
        Flicker();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};


class Auto : public State
{
    public:
        Auto();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );;
};


class Pause : public State
{
    public:
        Pause();
        void enter( Lantern& );
        void exit( Lantern& );
        uint8_t act( Lantern& );
        uint8_t getNext( Lantern& );
};
#endif
