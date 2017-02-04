cmake_minimum_required(VERSION 3.6)
project(adbsync)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_FOLDER /mnt/data/Igrek/c++/adb-sync/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

#include_directories(/usr/include/qt/QtCore/)
#include_directories(/usr/include/qt/QtWidgets/)
#include_directories(/usr/include/qt/QtGui/)
#include_directories(${Qt5Widgets_INCLUDES})

#include_directories(/usr/include)

# only for colouring syntax in clion
set(SOURCE_FILES
        src/main.cpp
        src/logger/Logger.cpp src/logger/Logger.h
        src/dispatcher/EventDispatcher.cpp src/dispatcher/EventDispatcher.h
        src/dispatcher/IEventObserver.cpp src/dispatcher/IEventObserver.h
        src/dispatcher/Event.cpp src/dispatcher/Event.h
        )

# included qt libraries
add_executable(cmakedupa ${SOURCE_FILES})
target_link_libraries(cmakedupa Qt5::Widgets)


add_custom_target(adbsync COMMAND make -C ${PROJECT_FOLDER}/build CLION_EXE_DIR=${PROJECT_BINARY_DIR})
