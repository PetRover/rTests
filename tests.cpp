//
// Created by Bryce Cater on 9/5/15.
//

#include "rTests.h"
#include <thread>
#include <string>
#include <stdio.h>

#define RUNTEST_TEST_GPIO

namespace RVR
{
    int testGpio(int pinNumber, RVR::GpioDirection direction)
    {
        switch (direction)
        {
            case RVR::GpioDirection::OUT:
            {
                RVR::GpioPin pin = RVR::GpioPin(pinNumber, direction);
                printf("Setting GPIO pin %d high... You have 10 seconds to read with a DMM", pinNumber);
                pin.setValue(RVR::GpioValue::HIGH);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                printf("Setting GPIO pin %d low... You have 10 seconds to read with a DMM", pinNumber);
                pin.setValue(RVR::GpioValue::LOW);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                printf("Gpio Test complete on pin %d", pinNumber);
                return 0;
            }
            case RVR::GpioDirection::IN:
            {
                // TODO Implement this
                return 0;
            }
            case RVR::GpioDirection::ERROR:
            {
                printf("Invalid input arguments");
                return -1;
            }
        }


    }
}

int main(void)
{
#ifdef RUNTEST_TEST_GPIO
    RVR::testGpio(10, RVR::GpioDirection::OUT);
//    RVR::testGpio(10, RVR::GpioDirection::OUT);
#endif
    return 0;
}