cmake_minimum_required(VERSION 3.6)
project(adbsync)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_FOLDER /mnt/data/Igrek/c++/adb-sync/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_system -lboost_thread")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_FOLDER}build/")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_FOLDER}build/")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_FOLDER}build/")
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_FOLDER}build/" )

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
        src/config/ConfigLoader.cpp src/config/ConfigLoader.h
        src/config/Database.cpp src/config/Database.h
        src/errors/Error.h
        src/filesystem/FileSystem.cpp src/filesystem/FileSystem.h
        src/filesystem/LocalFS.cpp src/filesystem/LocalFS.h
        src/filesystem/ADB.cpp src/filesystem/ADB.h
        src/filesystem/File.cpp src/filesystem/File.h
        src/filesystem/Directory.cpp src/filesystem/Directory.h
        src/filesystem/RegularFile.cpp src/filesystem/RegularFile.h
        src/gui/GUI.cpp src/gui/GUI.h
        src/gui/MainWindow.cpp src/gui/MainWindow.h
        src/gui/DiffListBox.cpp src/gui/DiffListBox.h
        src/synchronizer/Synchronizer.cpp src/synchronizer/Synchronizer.h
        src/system/CmdExecutor.cpp src/system/CmdExecutor.h
        src/synchronizer/DiffScanner.cpp src/synchronizer/DiffScanner.h
        src/diffs/Diff.cpp src/diffs/Diff.h
        src/diffs/DiffType.h
        src/threads/SingleThread.cpp src/threads/SingleThread.h
        src/threads/Thread.cpp src/threads/Thread.h
        src/threads/LoopThread.cpp src/threads/LoopThread.h
        src/App.cpp src/App.h
        src/config/ConfigProperties.cpp src/config/ConfigProperties.h
        src/utils/string_utils.cpp src/utils/string_utils.h src/errors/SystemCmdError.cpp src/errors/SystemCmdError.h src/errors/Error.cpp src/errors/SystemCmdError.cpp)

# highlighted included qt libraries
add_executable(cmakebuilt ${SOURCE_FILES})
target_link_libraries(cmakebuilt Qt5::Widgets)


add_custom_target(adbsync COMMAND make -C ${PROJECT_FOLDER}/build CLION_EXE_DIR=${PROJECT_BINARY_DIR})
