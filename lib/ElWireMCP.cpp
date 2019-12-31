//
// Created by elektron on 19/09/18.
//

#include "ElWireMCP.h"

ElWire elWire;

void ElWire::begin() {
    mcp[0].begin(0);
    mcp[1].begin(1);
    mcp[2].begin(2);

    for (uint8_t i=0; i<NB_WIRES; i++) {
        uint8_t mcpToChoose = i/10; //each mcp23017 controls 10 outputs
        mcp[mcpToChoose].pinMode(i%10, OUTPUT);
        write(i, LOW);
    }

    for (uint8_t i=0; i<NB_WIRES; i++) {
        mode[i] = IDLE;
    }
}

void ElWire::write(uint8_t pin, boolean state) {
    uint8_t mcpToChoose = pin/10; //each mcp23017 controls 10 outputs
    currentState[pin] = state;
    mcp[mcpToChoose].digitalWrite(pin%10, state);
}

void ElWire::pulse(uint8_t pin, int time) {
    mode[pin] = PULSE;
    write(pin, HIGH);
    timeStart[pin] = millis();
    timeToWait[pin] = time;
}

void ElWire::update() {
    for(int i=0; i<NB_WIRES; i++) {

        if(mode[i] == BLINK) {
            if(millis() - lastChange[i] >= periods[i]) {
                write(i, !currentState[i]);
                lastChange[i] = millis();
            }
        } else if (mode[i] == PULSE){
            if (millis() - timeStart[i] >= timeToWait[i]) {
                write(i, LOW);
                mode[i] = IDLE;
            }
        } else if (mode[i] == CIRCLE) {
            if((currentState[i] == HIGH) && (millis() - timeStart[i] >= timeToWait[i])) {
                write(i, LOW);
                write((i+1)%NB_WIRES, HIGH);
                timeStart[(i+1)%NB_WIRES] = millis();
            }
        }
    }
    digitalWrite(7,currentState[0]);
}

void ElWire::blink(uint8_t pin, int period) {
    mode[pin] = BLINK;
    write(pin, HIGH);
    lastChange[pin] = millis();
    periods[pin] = period;
}

void ElWire::idle(uint8_t pin) {
    write(pin, LOW);
    mode[pin] = IDLE;
}

void ElWire::setOn(uint8_t pin) {
    write(pin, HIGH),
    mode[pin] = ON;
}

void ElWire::circle(uint16_t period) {
    for(int i=0; i<NB_WIRES; i++){
        mode[i] = CIRCLE;
        write(i, LOW);
        timeToWait[i] = period/NB_WIRES;
    }
    write(0, HIGH);
    timeStart[0] = millis();
}

void ElWire::strob(uint16_t period) {
    for(int i=0; i<NB_WIRES; i++){
        blink(i, period);
    }
}
