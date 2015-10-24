//
// Created by Bryce Cater on 9/5/15.
//

#include "rTests.h"
#include "rMotors.h"
#include "rWifi.h"
#include "rPower.h"
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
//#define RUNTEST_TEST_WIFI_SEND_COMMAND
//#define RUNTEST_TEST_WIFI_SEND_STATUS
//#define RUNTEST_TEST_WIFI_SEND_TEXT
//#define RUNTEST_TEST_DC_MOTOR
//#define RUNTEST_TEST_WIFI_RECEIVE_COMMAND
#define RUNTEST_TEST_WIFI_RECEIVE_STATUS
//#define RUNTEST_TEST_WIFI_RECEIVE_TEXT
//#define RUNTEST_TEST_CAMERA_SAVE_FILE
// ===========================================

int DELAY_SECONDS = 3;
const char* serverIP = "192.168.1.12";

namespace RVR
{
    int testGpio(int pinNumber)
    {


        VLOG(1) << "Begining GPIO test on pin (" << pinNumber << ")";
        try
        {
            GpioPin pin = GpioPin(pinNumber);
            switch (pin.getDirection())
            {
                case GpioDirection::OUT:
                {
                    logger->verbose(2,"Setting GPIO pin %v high... You have %v seconds to read with a DMM\n", pinNumber, DELAY_SECONDS);
                    pin.setValue(GpioValue::HIGH);
                    printCountdown(DELAY_SECONDS);
                    logger->verbose(2,"Setting GPIO pin %v low... You have %v seconds to read with a DMM\n", pinNumber, DELAY_SECONDS);
                    pin.setValue(GpioValue::LOW);
                    printCountdown(DELAY_SECONDS);
                    break;
                }
                case GpioDirection::IN:
                {
                    // TODO Implement this
                    break;
                }
                case GpioDirection::ERROR:
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

    int testDcMotor(DRV8842Motor motor)
    {
        motor.setRampTime(2000);

        printf("Running motor forward for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, MotorDirection::FORWARD);
        printCountdown(DELAY_SECONDS);


        printf("Stopping motor for %d seconds\n", DELAY_SECONDS);
        motor.stopMotor();
        printCountdown(DELAY_SECONDS);

        printf("Running motor backwards for %d seconds\n", DELAY_SECONDS);
        motor.startMotor(50, MotorDirection::REVERSE);
        printCountdown(DELAY_SECONDS);

        printf("Stopping motor\n");
        motor.stopMotor();

        return 0;
    }

    void testWifiSendCommand(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        //make and fill command with data
        Command *ourCommand = new Command;
        ourCommand->setCommandType(CommandType::DRIVE_BACKWARD);
        char message[100] = "YES!";
        ourCommand->setCommandData(message);

        //turn command into NetworkChunk
        NetworkChunk* ourNetworkChunk = new NetworkChunk;
        *ourNetworkChunk = ourCommand->toNetworkChunk();

        //send NetworkChunk
        ourNetworkManager->sendData("USBSocket", ourNetworkChunk);
    }

    void testWifiSendStatus(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        //make and fill status with data
        Status *ourStatus = new Status;
        ourStatus->setStatusType(StatusType::NOT_CHARGING);
        char message[100] = "YES!";
        ourStatus->setStatusData(message);

        //turn status into NetworkChunk
        NetworkChunk* ourNetworkChunk = new NetworkChunk;
        *ourNetworkChunk = ourStatus->toNetworkChunk();

        //send NetworkChunk
        ourNetworkManager->sendData("USBSocket", ourNetworkChunk);
    }

    void testWifiSendText(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        //make and fill text with data
        Text *ourTextMessage = new Text;
        ourTextMessage->setLength(10);
        char message[100] = "YESSSSS!!!";
        ourTextMessage->setTextMessage(message);

        //turn command into NetworkChunk
        NetworkChunk* ourNetworkChunk = new NetworkChunk;
        *ourNetworkChunk = ourTextMessage->toNetworkChunk();

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
    }

    void testWifiReceiveStatus(const char* ipAddress)
    {
        NetworkManager* ourNetworkManager = new NetworkManager;
        ourNetworkManager->initializeNewConnection("USBSocket", ipAddress);

        NetworkChunk* chunk = new NetworkChunk;
        *chunk = ourNetworkManager->getData("USBSocket");

        Status ourStatus(*chunk);
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
    RVR::PowerRail *motorRail = RVR::PowerManager::getRail(RVR::RAIL12V0);
    const RVR::DRV8842Motor driveAMotor = RVR::DRV8842Motor(3, 86, 88, 89, 87, 10, 81, 32, 45, 61, 77, motorRail, 2500, 125);
    RVR::testDcMotor(driveAMotor);
#endif

#ifdef RUNTEST_TEST_WIFI_SEND_COMMAND
    RVR::testWifiSendCommand(serverIP);
#endif

#ifdef RUNTEST_TEST_WIFI_SEND_STATUS
    RVR::testWifiSendStatus(serverIP);
#endif

#ifdef RUNTEST_TEST_WIFI_SEND_TEXT
    RVR::testWifiSendText(serverIP);
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_COMMAND
    RVR::testWifiReceiveCommand(serverIP);
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_STATUS
    RVR::testWifiReceiveStatus(serverIP);
#endif

#ifdef RUNTEST_TEST_WIFI_RECEIVE_TEXT
    RVR::testWifiReceiveText(serverIP);
#endif

#ifdef RUNTEST_TEST_CAMERA_SAVE_FILE
    RVR::testCameraSaveFile(3);
#endif

    return 0;
}