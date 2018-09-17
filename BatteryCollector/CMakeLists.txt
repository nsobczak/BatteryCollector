cmake_minimum_required (VERSION 2.6)
project (UE4)
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1 CACHE BOOL "" FORCE)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1 CACHE BOOL "" FORCE)
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/clang++.exe")
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/clang.exe")

include("G:/Documents/Git/Battery-Collector/BatteryCollector/Intermediate/ProjectFiles/IncludeDirectories.cmake")
include("G:/Documents/Git/Battery-Collector/BatteryCollector/Intermediate/ProjectFiles/Definitions.cmake")
include("G:/Documents/Git/Battery-Collector/BatteryCollector/Intermediate/ProjectFiles/BatteryCollectorEditor.cmake")

set(MONO_ROOT_PATH "E:/Programmes/Epic Games/UE_4.18/Engine/Binaries/DotNET")
set(BUILD cd /d "${MONO_ROOT_PATH}")

# Custom target for BatteryCollectorEditor project, Development configuration
add_custom_target(BatteryCollectorEditor-Development ${BUILD} && UnrealBuildTool.exe BatteryCollectorEditor Win64 Development -project="G:/Documents/Git/Battery-Collector/BatteryCollector/BatteryCollector.uproject" -game -progress)
add_custom_target(BatteryCollectorEditor-Development-CLEAN ${BUILD} && UnrealBuildTool.exe BatteryCollectorEditor Win64 Development -project="G:/Documents/Git/Battery-Collector/BatteryCollector/BatteryCollector.uproject" -clean)

add_executable(PleaseIgnoreMe ${BatteryCollectorEditor_FILES})