#include <gateController.h>
#include <stdio.h>
#include <wiringPi.h>

gateController::gateController() {
    wiringPiSetupGpio();
    pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(21, INPUT);
    pullUpDnControl(21, PUD_UP);
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    wiringPiISR(21, INT_EDGE_BOTH, intrptHndlr);
}

gateController& gateController::getController() {
    static gateController controller;

    return controller;
}

void gateController::openGate(){
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(1, LOW);
}

void gateController::closeGate(){
    digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
}

void gateController::intrptHndlr(){
    printf("Interupt registered PIN21 state: %d\n", digitalRead(21));
}
