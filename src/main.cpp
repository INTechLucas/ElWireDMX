//
// Created by elektron on 19/09/18.
//

#include "ElWireMega.h"
#include "DMXSerial.h"

#define CHANNEL_MAX 64
#define MAX_VALUE 255
#define CIRCLE_CHAN 31
#define STROB_CHAN 32
#define WHITE_CHAN 33
#define BLUE_CHAN 34
#define PURPLE_CHAN 35
#define INSIDE_CHAN 36

#define DEMO false
#define DMX true

int dmxOrders[100];
int lastDmxOrders[100];

void checkSerial();
void demo();

void setup() {
    if (DMX) DMXSerial.init(DMXReceiver);
    elWire.begin();
    pinMode(13,OUTPUT);
    elWire.write(0,HIGH);
    delay(1000);
    elWire.write(0,LOW);
}

void loop() {
    if (DEMO) {
        demo();
    } else {
        elWire.update();

        if (!DMX) {
            checkSerial();
        } else {
            for (int i = 1; i <= CHANNEL_MAX; i++) { //read all channels
                dmxOrders[i - 1] = DMXSerial.read(i);
            }

            if (dmxOrders[CIRCLE_CHAN] != lastDmxOrders[CIRCLE_CHAN]) { //if an order to circle arrives
                elWire.circle(dmxOrders[CIRCLE_CHAN] * 3);
                lastDmxOrders[CIRCLE_CHAN] = dmxOrders[CIRCLE_CHAN];

            } else if (dmxOrders[STROB_CHAN] != lastDmxOrders[STROB_CHAN]) { //if an order to stroboscope arrives
                elWire.strob(dmxOrders[STROB_CHAN]);
                lastDmxOrders[STROB_CHAN] = dmxOrders[STROB_CHAN];

            } else if (dmxOrders[WHITE_CHAN] != lastDmxOrders[WHITE_CHAN]) { //if an order for a group arrives (idle, blink or on)
                elWire.groupOrder(elWire.WHITE, dmxOrders[WHITE_CHAN]);
                lastDmxOrders[WHITE_CHAN] = dmxOrders[WHITE_CHAN];

            } else if (dmxOrders[BLUE_CHAN] != lastDmxOrders[BLUE_CHAN]) {
                elWire.groupOrder(elWire.BLUE, dmxOrders[BLUE_CHAN]);
                lastDmxOrders[BLUE_CHAN] = dmxOrders[BLUE_CHAN];

            } else if (dmxOrders[PURPLE_CHAN] != lastDmxOrders[PURPLE_CHAN]) {
                elWire.groupOrder(elWire.PURPLE, dmxOrders[PURPLE_CHAN]);
                lastDmxOrders[PURPLE_CHAN] = dmxOrders[PURPLE_CHAN];

            } else if (dmxOrders[INSIDE_CHAN] != lastDmxOrders[INSIDE_CHAN]) {
                elWire.groupOrder(elWire.INSIDE, dmxOrders[INSIDE_CHAN]);
                lastDmxOrders[INSIDE_CHAN] = dmxOrders[INSIDE_CHAN];

            } else { //individuals orders
                for (int i = 0; i < NB_WIRES; i++) { //check the whole array
                    if (dmxOrders[i] != lastDmxOrders[i]) { //new order for channel i
                        if (dmxOrders[i] <= 5) {
                            elWire.idle(i);
                        } else if (dmxOrders[i] >= MAX_VALUE - 5) {
                            elWire.setOn(i);
                        } else {
                            elWire.blink(i, 4000 / dmxOrders[i]); //value to adjust
                        }
                        lastDmxOrders[i] = dmxOrders[i];
                    }
                }
            }
        }
    }
}

void demo(){

    long lastUpdate = millis();
    elWire.circle(1000);
    while (millis()-lastUpdate<=5000) {
        elWire.update();
    }
    lastUpdate = millis();
    elWire.strob(200);
    while (millis()-lastUpdate<=5000) {
        elWire.update();
    }
    lastUpdate = millis();
    elWire.blink(0,random(30, 300));
    elWire.blink(1,random(30, 300));
    elWire.blink(2,random(30, 300));
    elWire.blink(3,random(30, 300));
    elWire.blink(4,random(30, 300));
    elWire.blink(5,random(30, 300));
    elWire.blink(6,random(30, 300));
    elWire.blink(7,random(30, 300));
    elWire.blink(8,random(30, 300));
    elWire.blink(9,random(30, 300));
    while (millis()-lastUpdate<=5000) {
        elWire.update();
    }

    elWire.groupOrder(elWire.WHITE, 255);
    delay(1000);
    elWire.groupOrder(elWire.WHITE, 0);
    elWire.groupOrder(elWire.BLUE, 255);
    delay(1000);
    elWire.groupOrder(elWire.BLUE, 0);
    elWire.groupOrder(elWire.PURPLE, 255);
    delay(1000);
    elWire.groupOrder(elWire.PURPLE, 0);
    elWire.groupOrder(elWire.INSIDE, 255);
    delay(1000);
    elWire.groupOrder(elWire.INSIDE, 0);
    delay(1000);

    /*elWire.blink(0,random(30, 300));
    elWire.blink(1,random(30, 300));
    elWire.blink(2,random(30, 300));
    elWire.blink(3,random(30, 300));
    elWire.blink(4,random(30, 300));
    elWire.blink(5,random(30, 300));
    elWire.blink(6,random(30, 300));
    elWire.blink(7,random(30, 300));
    elWire.blink(8,random(30, 300));
    elWire.blink(9,random(30, 300));
    while(1) {
        elWire.update();
    }*/
}

/*void checkSerial() {  //reads orders on serial port. Orders must be like channel|value with channel<10
    if (Serial.available()) {
        char chan = Serial.read();
        delay(1);
        Serial.read();
        delay(1);
        String value;

        while (Serial.available()) {
            value += Serial.read() - 48;
        }

        Serial.print(chan);
        Serial.print("|");
        Serial.println(value);

        switch(chan) {
            case '0':
                Serial.print("Pulse 0 with delay of ");
                Serial.println(2*value.toInt());
                elWire.pulse(0, 2*value.toInt());
                break;
            case '1':
                Serial.print("Pulse 1 with delay of ");
                Serial.println(2*value.toInt());
                elWire.pulse(1, 2*value.toInt());
                break;
            case '4':
                Serial.print("Blink 0 with delay of ");
                Serial.println(value.toInt());
                elWire.blink(0, value.toInt());
                break;
            case '5':
                Serial.print("Blink 1 with delay of ");
                Serial.println(value.toInt());
                elWire.blink(1, value.toInt());
                break;
            default:
                Serial.println("Unknown command");
        }
    }
}*/