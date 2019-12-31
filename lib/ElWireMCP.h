//
// Created by elektron on 19/09/18.
//

#ifndef ELWIRE_ELWIRE_H
#define ELWIRE_ELWIRE_H

#include "Arduino.h"
#include "Adafruit_MCP23017.h"

#define NB_WIRES 3

class ElWire {

public:
    void begin();
    void write(uint8_t pin, boolean state);
    void pulse(uint8_t pin, int time);
    void blink(uint8_t pin, int period);
    void setOn(uint8_t pin);
    void idle(uint8_t pin);
    void circle(uint16_t period);
    void strob(uint16_t period);
    void update();

private:
    Adafruit_MCP23017 mcp[3] = {Adafruit_MCP23017(), Adafruit_MCP23017(), Adafruit_MCP23017()};

    boolean currentState[30];

    enum Mode {
        BLINK,
        PULSE,
        ON,
        CIRCLE,
        IDLE
    };


    Mode mode[NB_WIRES];

    int timeToWait[NB_WIRES];
    unsigned long timeStart[NB_WIRES];
    unsigned long lastChange[NB_WIRES];
    unsigned int periods[NB_WIRES];
};

extern ElWire elWire;


#endif //ELWIRE_ELWIRE_H
