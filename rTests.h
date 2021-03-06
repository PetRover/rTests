//
// Created by Bryce Cater on 9/5/15.
//

#ifndef RCORE_TESTS_H
#define RCORE_TESTS_H

#include "pins.h"

namespace RVR
{

    int testGpio(int pinNumber, GpioDirection direction);
    void testWifiSend(const char* ipAddress);
    void testWifiReceive(const char* ipAddress);

    void printCountdown(int seconds);

}
#endif //RCORE_TESTS_H
