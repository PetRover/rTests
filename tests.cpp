//
// Created by Bryce Cater on 9/5/15.
//

#include "rTests.h"
#include <thread>
#include <string>
#include <stdio.h>
#include <unistd.h>

#define RUNTEST_TEST_GPIO
const int DELAY_SECONDS = 3;
namespace RVR
{
    int testGpio(int pinNumber, RVR::GpioDirection direction)
    {
        switch (direction)
        {

            case RVR::GpioDirection::OUT:
            {
                printf("\n======================\nStarting GPIO ouput test\n======================\n\n");
                RVR::GpioPin pin = RVR::GpioPin(pinNumber, direction);
                printf("Setting GPIO pin %d high... You have %d seconds to read with a DMM\n", pinNumber,
                       DELAY_SECONDS);
                pin.setValue(RVR::GpioValue::HIGH);
                printCountdown(DELAY_SECONDS);
                printf("Setting GPIO pin %d low... You have %d seconds to read with a DMM\n", pinNumber, DELAY_SECONDS);
                pin.setValue(RVR::GpioValue::LOW);
                printCountdown(DELAY_SECONDS);
                printf("\n======================\nGPIO Output test complete\n======================\n\n");
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

    void printCountdown(int seconds)
    {
        printf("| %d |...\n", (seconds));
        usleep(1000000);
        if (seconds > 0)
        {
            printCountdown(seconds - 1);
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