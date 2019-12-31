//
// Created by elektron on 19/09/18.
//

#include "ElWireMega.h"

void ElWire::begin() {
    for (uint8_t i=0; i<NB_WIRES; i++) {
        pinMode(i+FIRST_PIN, OUTPUT); //declare pin 22,23,... as outputs
        write(i, LOW);
    }

    for (uint8_t i=0; i<NB_WIRES; i++) {
        mode[i] = IDLE;
    }
}

void ElWire::write(uint8_t pin, boolean state) {
    currentState[pin] = state;
    digitalWrite(pin+FIRST_PIN, state);
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
    write(pin, HIGH);
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

void ElWire::groupOrder(ElWire::Groups group, int value) {
    if(value <= 5) { //just set off
        if (group == WHITE) {
            for(int i = 0; i <= nbWhite; i++) {
                idle(white[i]);
            }
        } else if (group == BLUE) {
            for(int i = 0; i <= nbBlue; i++) {
                idle(blue[i]);
            }
        } else if (group == PURPLE) {
            for(int i = 0; i <= nbPurple; i++) {
                idle(purple[i]);
            }
        } else if (group == INSIDE) {
            for(int i = 0; i <= nbInside; i++) {
                idle(inside[i]);
            }
        }
    } else if(value >= 250) { //just set on
        if (group == WHITE) {
            for(int i = 0; i <= nbWhite; i++) {
                setOn(white[i]);
            }
        } else if (group == BLUE) {
            for(int i = 0; i <= nbBlue; i++) {
                setOn(blue[i]);
            }
        } else if (group == PURPLE) {
            for(int i = 0; i <= nbPurple; i++) {
                setOn(purple[i]);
            }
        } else if (group == INSIDE) {
            for(int i = 0; i <= nbInside; i++) {
                setOn(inside[i]);
            }
        }
    } else { //blink
        if (group == WHITE) {
            for(int i = 0; i < nbWhite; i++) {
                blink(white[i], 4000/value);
            }
        } else if (group == BLUE) {
            for(int i = 0; i < nbBlue; i++) {
                blink(blue[i], 4000/value);
            }
        } else if (group == PURPLE) {
            for(int i = 0; i < nbPurple; i++) {
                blink(purple[i], 4000/value);
            }
        } else if (group == INSIDE) {
            for(int i = 0; i < nbInside; i++) {
                blink(inside[i], 4000/value);
            }
        }
    }
}

void ElWire::randomBlink() {
    for (int i=0; i<=NB_WIRES; i++) {
        blink(i, random(30, 1000));
    }
}
