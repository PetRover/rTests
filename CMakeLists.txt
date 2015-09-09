cmake_minimum_required(VERSION 3.3)

SET(EXECUTABLE_OUTPUT_PATH ../../bin)

add_executable(rTests tests.cpp rTests.h)

target_link_libraries(rTests rMotors)
target_link_libraries(rTests rProtocols)
target_link_libraries(rTests rSensors)
target_link_libraries(rTests rWifi)
target_link_libraries(rTests rPowerSystems)