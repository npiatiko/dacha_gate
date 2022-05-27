#include <gateController.h>
#include <stdio.h>
#include <wiringPi.h>

gateController::gateController() {
    wiringPiSetupGpio();
    pinMode(forwardPIN, OUTPUT);
    pinMode(reversePIN, OUTPUT);
    MotorControl(MotorMode::OFF);
    pinMode(irSensorPIN, INPUT);
    pinMode(closeSensorPIN, INPUT);
    pinMode(openSensorPIN, INPUT);
    pullUpDnControl(irSensorPIN, PUD_UP);
    pullUpDnControl(closeSensorPIN, PUD_UP);
    pullUpDnControl(openSensorPIN, PUD_UP);
    wiringPiISR(irSensorPIN, INT_EDGE_BOTH, IrSensorIntrpt);
    wiringPiISR(closeSensorPIN, INT_EDGE_BOTH, CloseSensorIntrpt);
    wiringPiISR(openSensorPIN, INT_EDGE_BOTH, OpenSensorIntrpt);
}

gateController& gateController::getController() {
    static gateController controller;

    return controller;
}

void gateController::openGate() {
    MotorControl(MotorMode::FORWARD);
}

void gateController::closeGate() {
    MotorControl(MotorMode::REVERSE);
}

void gateController::IrSensorIntrpt() {
    printf("%s: %d\n", __func__, digitalRead(irSensorPIN));
}

void gateController::CloseSensorIntrpt() {
    printf("%s: %d\n", __func__, digitalRead(closeSensorPIN));
}

void gateController::OpenSensorIntrpt() {
    printf("%s: %d\n", __func__, digitalRead(openSensorPIN));
}

void gateController::MotorControl(const MotorMode mode) {
    const u_MotorMode _mode(mode);

    digitalWrite(forwardPIN, HIGH);
    digitalWrite(reversePIN, HIGH);
    digitalWrite(forwardPIN, _mode._forward);
    digitalWrite(reversePIN, _mode._reverse);
}
