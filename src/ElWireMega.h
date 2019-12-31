//
// Created by elektron on 19/09/18.
//

#ifndef ELWIRE_ELWIRE_H
#define ELWIRE_ELWIRE_H

#include "Arduino.h"

#define NB_WIRES 30
#define FIRST_PIN 22

class ElWire {

public:

    enum Mode {
        BLINK,
        PULSE,
        ON,
        CIRCLE,
        IDLE
    };

    enum Groups {
        WHITE,
        BLUE,
        PURPLE,
        INSIDE
    };

    void begin();
    void write(uint8_t pin, boolean state);
    void pulse(uint8_t pin, int time);
    void blink(uint8_t pin, int period);
    void setOn(uint8_t pin);
    void idle(uint8_t pin);
    void circle(uint16_t period);
    void strob(uint16_t period);
    void groupOrder(Groups group, int value);
    void randomBlink();
    void update();

private:
    boolean currentState[30];

    Mode mode[NB_WIRES];

    int timeToWait[NB_WIRES];
    unsigned long timeStart[NB_WIRES];
    unsigned long lastChange[NB_WIRES];
    unsigned int periods[NB_WIRES];

    uint8_t white[10] = {1,5,6,8,9};
    const int nbWhite = 5;
    uint8_t blue[10] = {1,5,6,8,9};
    const int nbBlue = 5;
    uint8_t purple[10] = {1,5,6,8,9};
    const int nbPurple = 5;
    uint8_t inside[10] = {0,1,2,3,4,5,6,7,8,9};
    const int nbInside = 10;
};

extern ElWire elWire;


#endif //ELWIRE_ELWIRE_H
