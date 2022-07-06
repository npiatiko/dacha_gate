#include <gateController.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include <chrono>
#include <iostream>
#include <thread>

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
    mCv.wait(lk, [&] { return mState.load() == State::OPENED; });
}

void gateController::closeGate() {
    std::unique_lock<std::mutex> lk(mLock);
    int irSensState = 0;

    while (mState.load() != State::CLOSED) {
        irSensState = digitalRead(irSensorPIN);
        if (irSensState == HIGH) {
            MotorControl(MotorMode::REVERSE);
        }
        mCv.wait_for(lk, std::chrono::seconds(10), [&] { return mState.load() == State::CLOSED; });
    }
}

void gateController::IrSensorIntrpt() {
    int pinState = digitalRead(irSensorPIN);
    std::thread irThread([&]() {
        static std::mutex IrLockMtx;
        std::unique_lock<std::mutex> IrLocker(IrLockMtx, std::defer_lock);

        if (IrLocker.try_lock()) {
            std::unique_lock<std::mutex> lk(mLock);

            if (mState.load() == State::OPENED) {
                MotorControl(MotorMode::OFF);
            } else if (mState.load() == State::CLOSING) {
        MotorControl(MotorMode::FORWARD);
    }
        } else {
            printf("Ignoring Interrupt!!\n");
        }
    });

    irThread.detach();
    printf("%s: %d\n", __func__, pinState);
}

void gateController::CloseSensorIntrpt() {
    std::unique_lock<std::mutex> lk(mLock);

    int pinState = digitalRead(closeSensorPIN);

    printf("%s: %d\n", __func__, pinState);
    if (pinState == HIGH) {
        mState.store(State::OPENING);
    } else {
        MotorControl(MotorMode::OFF);
        mState.store(State::CLOSED);
        mCv.notify_all();
    }
}

void gateController::OpenSensorIntrpt() {
    std::unique_lock<std::mutex> lk(mLock);

    int pinState = digitalRead(openSensorPIN);

    printf("%s: %d\n", __func__, pinState);
    if (pinState == HIGH) {
        mState.store(State::CLOSING);
    } else {
        MotorControl(MotorMode::OFF);
        mState.store(State::OPENED);
        mCv.notify_all();
    }
}

void gateController::MotorControl(const MotorMode mode) {
    const u_MotorMode _mode(mode);

    digitalWrite(forwardPIN, HIGH);
    digitalWrite(reversePIN, HIGH);
    digitalWrite(motorBrakePIN, ~(_mode._forward ^ _mode._reverse));
    digitalWrite(forwardPIN, _mode._forward);
    digitalWrite(reversePIN, _mode._reverse);
}
