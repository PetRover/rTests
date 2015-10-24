//
// Created by Bryce Cater on 9/5/15.
//

#include "rTests.h"
#include "rMotors.h"
#include "rWifi.h"
#include <stdexcept>
#include <unistd.h>
#include "rCamera.h"
#include <stdexcept>
#include <unistd.h>

#include "../rCore/easylogging++.h"
INITIALIZE_EASYLOGGINGPP;
el::Logger *logger = el::Loggers::getLogger("default");

// ===========================================
// TEST ENABLING FLAG SECTION
// ===========================================
//#define RUNTEST_TEST_GPIO
//#define RUNTEST_TEST_DC_MOTOR
//#define RUNTEST_TEST_WIFI_SEND
//#define RUNTEST_TEST_WIFI_RECEIVE_COMMAND
//#define RUNTEST_TEST_WIFI_RECEIVE_STATUS
#define RUNTEST_TEST_WIFI_RECEIVE_TEXT
//#define RUNTEST_TEST_CAMERA_SAVE_FILE
// ===========================================

int DELAY_SECONDS = 3;

namespace RVR
{
    int testGpio(int pinNumber)
    {


        VLOG(1) << "Begining GPIO test on pin (" << pinNumber << ")";
        try
        {
            RVR::GpioPin pin = RVR::GpioPin(pinNumber);
            switch (pin.getDirection())
            {
                case RVR::GpioDirection::OUT:
                {
                    logger->verbose(2,"Setting GPIO pin %v high... You have %v seconds to read with a DMM\n", pinNumber, DELAY_SECONDS);
                    pin.setValue(RVR::GpioValue::HIGH);
                    printCountdown(DELAY_SECONDS);
                    logger->verbose(2,"Setting GPIO pin %v low... You have %v seconds to read with a DMM\n", pinNumber, DELAY_SECONDS);
                    pin.setValue(RVR::GpioValue::LOW);
                    printCountdown(DELAY_SECONDS);
                    break;
                }
                case RVR::GpioDirection::IN:
                {
                    // TODO Implement this
                    break;
                }
                case RVR::GpioDirection::ERROR:
                {
                    LOG(ERROR) << "The pin object has a bad dirrection property";
                    throw std::runtime_error("The pin object has a bad dirrection property");
                }
            }
        }
        catch (...)
        {
            LOG(ERROR) << "[ FAILURE ] GPIO test of pin (" << pinNumber << ") failed";
            return -1;
        }
        VLOG(1) << "[ SUCCESS ] GPIO test on pin (" << pinNumber << ") passed.";
        return 0;
    }

    int testDcMotor(RVR::MotorName motorName)
    {
        if (motorName == RVR::MotorName::CAMERA_MOTOR)
        {
            printf("FAILURE: The camera motor is not a DC motor");
            return 1;
        }
        RVR::DcMotor motor = RVR::DcMotor(motorName);
        motor.setRampTime(2000);

        printf("Running motor forward for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, RVR::MotorDirection::FORWARD);
        printCountdown(DELAY_SECONDS);


        printf("Stopping motor for %d seconds\n", DELAY_SECONDS);
        motor.stopMotor();
        printCountdown(DELAY_SECONDS);

        printf("Running motor backwards for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, RVR::MotorDirection::REVERSE);
        printCountdown(DELAY_SECONDS);

        printf("Stopping motor\n");
        motor.stopMotor();

        return 0;

    }

    void testWifiSend(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        //make and fill command with data
        Command *ourCommand = new Command;
        ourCommand->setCommandType(CommandType::DRIVE_FORWARD);
        char message[100] = "Look...it works!";
        ourCommand->setCommandData(message);

        //turn command into NetworkChunk
        NetworkChunk* ourNetworkChunk = new NetworkChunk;
        *ourNetworkChunk = ourCommand->toNetworkChunk();

        //send NetworkChunk
        ourNetworkManager->sendData("USBSocket", ourNetworkChunk);
    }

    void testWifiReceiveCommand(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        NetworkChunk* chunk = new NetworkChunk;
        *chunk = ourNetworkManager->getData("USBSocket");

        Command ourCommand(*chunk);

        //print out type/data of ourCommand
        if (ourCommand.getCommandType() == CommandType::DRIVE_BACKWARD)
        {
            VLOG(2) << "Command type was DRIVE_BACKWARD";
        }

        for (int i=0; i < COMMAND_LENGTH; i++)
        {
            VLOG(2) << (ourCommand.getCommandData())[i];
        }
    }

    void testWifiReceiveStatus(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        NetworkChunk* chunk = new NetworkChunk;
        *chunk = ourNetworkManager->getData("USBSocket");

        Status ourStatus(*chunk);

        //print out type/data of ourStatus
        if (ourStatus.getStatusType() == StatusType::CHARGING)
        {
            VLOG(2) << "Status type was CHARGING";
        }

        for (int i=0; i < STATUS_LENGTH; i++)
        {
            VLOG(2) << (ourStatus.getStatusData())[i];
        }
    }

    void testWifiReceiveText(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        NetworkChunk* chunk = new NetworkChunk;
        *chunk = ourNetworkManager->getData("USBSocket");

        Text ourTextMessage(*chunk);

        //print out length/data of ourTextMessage
        int length = ourTextMessage.getLength();
        VLOG(2) << "Length of our Message "<< length;

        for (int i=0; i < length; i++)
        {
            VLOG(2) << (ourTextMessage.getTextMessage())[i];
        }
    }

////needs to be commented out for Alyssa...
//    int testCameraSaveFile(int secondsToRun)
//    {
//        VLOG(1) << "Begining Camera save file test.";
//        try
//        {
//            Camera camera = Camera();
//            VLOG(2) << "Setting stream at YUYV, 640px by 480px @ 30fps";
//            camera.setupStream(UVC_FRAME_FORMAT_YUYV, 640, 480, 30);
//            VLOG(2) << "Setting callback function to saveFrame()";
//            camera.setFrameCallback(sendFrame);
//
//            camera.setAutoExposure(true);
//
//            camera.startStream();
//            VLOG(1) << "Streaming for " << secondsToRun << " seconds.";
//            printCountdown(secondsToRun);
//            camera.stopStream();
//        }
//        catch (...)
//        {
//            LOG(ERROR) << "[ FAILURE ] Camera write file test failed";
//            return -1;
//        }
//        VLOG(1) << "[ SUCCESS ] Camera write file test passed";
//        return 0;
//
//    }
//    //commented out until here for Alyssa

    void printCountdown(int seconds)
    {
        logger->verbose(2,"| %v |...\n", seconds);
        usleep(1000000);
        if (seconds > 0)
        {
            printCountdown(seconds - 1);
        }

    }
}

int main(int argc, char *argv[])
{
    START_EASYLOGGINGPP(argc, argv);
    LOG(INFO) << "Starting the rover tests";
#ifdef RUNTEST_TEST_GPIO
    RVR::testGpio(10);
#endif

#ifdef RUNTEST_TEST_DC_MOTOR
    RVR::testDcMotor(RVR::MotorName::DRIVE_MOTOR_B);
#endif

#ifdef RUNTEST_TEST_WIFI_SEND
    RVR::testWifiSend("192.168.7.1");
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_COMMAND
    RVR::testWifiReceiveCommand("192.168.1.12");
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_STATUS
    RVR::testWifiReceiveStatus("192.168.1.12");
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_TEXT
    RVR::testWifiReceiveText("192.168.1.12");
#endif

#ifdef RUNTEST_TEST_CAMERA_SAVE_FILE
    RVR::testCameraSaveFile(3);
#endif

    return 0;
}