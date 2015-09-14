//
// Created by Bryce Cater on 9/5/15.
//

#include "rTests.h"
#include "rMotors.h"
#include <unistd.h>

//#define RUNTEST_TEST_GPIO
#define RUNTEST_TEST_DC_MOTOR
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

    int testDcMotor(RVR::MotorName motorName)
    {
        if (motorName == RVR::MotorName::CAMERA_MOTOR)
        {
            printf("FAILURE: The camera motor is not a DC motor");
            return 1;
        }
        RVR::DcMotor motor = RVR::DcMotor(motorName);
        motor.setRampTime(100);

        printf("Running motor forward for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, RVR::MotorDirection::FORWARD);
        printCountdown(DELAY_SECONDS);

        printf("Stopping motor for %d seconds\n", DELAY_SECONDS);
        motor.stopMotor();
        printCountdown(DELAY_SECONDS);

        printf("Running motor backwards for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, RVR::MotorDirection::REVERSE);
        printCountdown(DELAY_SECONDS);

        return 0;

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
#endif

#ifdef RUNTEST_TEST_DC_MOTOR
    RVR::testDcMotor(RVR::MotorName::DRIVE_MOTOR_A);
#endif
    return 0;
}