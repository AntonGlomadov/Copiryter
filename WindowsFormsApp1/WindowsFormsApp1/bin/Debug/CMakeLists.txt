#cmake_minimum_required(VERSION 2.8.4)
#project(OpenCVTest)
#
#
#find_package( OpenCV REQUIRED )
#
#set(SOURCE_FILES PanoApp/main.cpp)
#set(CMAKE_CXX_STANDARD 17)
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#include_directories(${OpenCV_INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PUBLIC ./jsonlib/single_include/nlohmann)
#target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#cv cmake warning
#set_property(TARGET "${PROJECT_NAME}" PROPERTY BUILD_SHARED_LIBS "OFF")

project ( Panosticher )

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmakefunctions/Loader.cmake )
Loader(${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY ( ext/logger/Logger )

ADD_SUBDIRECTORY ( Snitcher )
ADD_SUBDIRECTORY ( PanoApp )