#include <gateController.h>
#include <stdio.h>
#include <wiringPi.h>

#include <chrono>

std::mutex gateController::mLock;
std::condition_variable gateController::mCv;
std::atomic<State> gateController::mState;

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
    wiringPiISR(irSensorPIN, INT_EDGE_FALLING, IrSensorIntrpt);
    wiringPiISR(closeSensorPIN, INT_EDGE_BOTH, CloseSensorIntrpt);
    wiringPiISR(openSensorPIN, INT_EDGE_BOTH, OpenSensorIntrpt);
}

gateController& gateController::getController() {
    static gateController controller;

    return controller;
}

void gateController::openGate() {
    std::unique_lock<std::mutex> lk(mLock);

    MotorControl(MotorMode::FORWARD);
    mCv.wait(lk, [&] { return mState == State::OPENED; });
}

void gateController::closeGate() {
    std::unique_lock<std::mutex> lk(mLock);
    int irSensState = 0;

    while (mState != State::CLOSED) {
        irSensState = digitalRead(irSensorPIN);
        if (irSensState == HIGH) {
            MotorControl(MotorMode::REVERSE);
        }
        mCv.wait_for(lk, std::chrono::seconds(10), [&] { return mState == State::CLOSED; });
    }
}

void gateController::IrSensorIntrpt() {
    printf("%s: %d\n", __func__, digitalRead(irSensorPIN));
    if (mState == State::CLOSING) {
        MotorControl(MotorMode::FORWARD);
    }
}

void gateController::CloseSensorIntrpt() {
    int pinState = digitalRead(closeSensorPIN);

    printf("%s: %d\n", __func__, pinState);
    if (pinState == HIGH) {
        mState = State::OPENING;
    } else {
        MotorControl(MotorMode::OFF);
        mState = State::CLOSED;
        mCv.notify_all();
    }
}

void gateController::OpenSensorIntrpt() {
    int pinState = digitalRead(openSensorPIN);

    printf("%s: %d\n", __func__, pinState);
    if (pinState == HIGH) {
        mState = State::CLOSING;
    } else {
        MotorControl(MotorMode::OFF);
        mState = State::OPENED;
        mCv.notify_all();
    }
}

void gateController::MotorControl(const MotorMode mode) {
    const u_MotorMode _mode(mode);

    digitalWrite(forwardPIN, HIGH);
    digitalWrite(reversePIN, HIGH);
    digitalWrite(forwardPIN, _mode._forward);
    digitalWrite(reversePIN, _mode._reverse);
}
