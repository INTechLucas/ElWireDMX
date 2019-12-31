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

#define MODE_PIN 2

#define AUTOMODE false
#define DMX true //choose between DMX and standard serial

int dmxOrders[100];
int lastDmxOrders[100];

void checkSerial();
void autoMode();

ElWire elWire;

void setup() {
    if (DMX) DMXSerial.init(DMXReceiver);
    elWire.begin();
    pinMode(13,OUTPUT);
    pinMode(MODE_PIN, INPUT);
    digitalWrite(MODE_PIN, HIGH); //enable pullup on switch
}

void loop() {
    if (digitalRead(MODE_PIN)) {
        autoMode();
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

void autoMode(){

    //Making a circle for some seconds
    int randomTime = random(1000, 10000);
    long lastUpdate = millis();
    elWire.circle(1000);
    while (millis()-lastUpdate<=randomTime) {
        elWire.update();
    }

    //Strob for some seconds
    randomTime = random(1000, 5000);
    lastUpdate = millis();
    elWire.strob(200);
    while (millis()-lastUpdate<=randomTime) {
        elWire.update();
    }

    //Blinking at random periods for some seconds
    randomTime = random(1000, 10000);
    lastUpdate = millis();
    elWire.randomBlink();
    while (millis()-lastUpdate<=randomTime) {
        elWire.update();
    }

    //Blinking at random periods by groups for some seconds
    randomTime = random(1000, 5000);
    lastUpdate = millis();
    elWire.groupOrder(elWire.WHITE, random(0, 255));
    elWire.groupOrder(elWire.BLUE, random(0, 255));
    elWire.groupOrder(elWire.PURPLE, random(0, 255));
    elWire.groupOrder(elWire.INSIDE, random(0, 255));
    while (millis()-lastUpdate<=randomTime) {
        elWire.update();
    }

    //Blinking at random periods by groups for some seconds
    randomTime = random(300, 1000);
    for (int i=0; i<10; i++) {
        elWire.groupOrder(elWire.WHITE, 255);
        elWire.update();
        delay(randomTime);
        elWire.groupOrder(elWire.WHITE, 0);
        elWire.groupOrder(elWire.BLUE, 255);
        elWire.update();
        delay(randomTime);
        elWire.groupOrder(elWire.BLUE, 0);
        elWire.groupOrder(elWire.PURPLE, 255);
        elWire.update();
        delay(randomTime);
        elWire.groupOrder(elWire.PURPLE, 0);
        elWire.groupOrder(elWire.INSIDE, 255);
        elWire.update();
        delay(randomTime);
        elWire.groupOrder(elWire.INSIDE, 0);
        elWire.update();

    }
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