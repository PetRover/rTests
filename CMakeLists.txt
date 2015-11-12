cmake_minimum_required(VERSION 3.3)

SET(EXECUTABLE_OUTPUT_PATH ../../bin)

IF (NOT CROSS_COMPILE)
    FIND_PACKAGE(libuvc REQUIRED)
    INCLUDE_DIRECTORIES("${libuvc_INCLUDE_DIRS}")
endif()

add_executable(rTests tests.cpp rTests.h)

IF (NOT CROSS_COMPILE)
    TARGET_LINK_LIBRARIES(rTests ${libuvc_LIBRARIES})
else()
#    TARGET_LINK_LIBRARIES(rTests libpthread.so.0)
#    TARGET_LINK_LIBRARIES(rTests libuvc.so)
endif()
TARGET_LINK_LIBRARIES(rTests rCamera)
TARGET_LINK_LIBRARIES(rTests rMotors)
TARGET_LINK_LIBRARIES(rTests rProtocols)
TARGET_LINK_LIBRARIES(rTests rSensors)
TARGET_LINK_LIBRARIES(rTests rWifi)
TARGET_LINK_LIBRARIES(rTests rPowerSystems)