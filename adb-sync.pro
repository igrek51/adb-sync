######################################################################
# Automatically generated by qmake (3.1) Tue Jan 31 15:33:37 2017
######################################################################

TEMPLATE = app
TARGET = adb-sync
INCLUDEPATH += .

QT += core gui widgets

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -lboost_system -lboost_thread -g -rdynamic

CONFIG += console

# Input
HEADERS += \
           src/logger/Logger.h \
           src/dispatcher/EventDispatcher.h \
           src/dispatcher/IEventObserver.h \
           src/dispatcher/Event.h \
           src/dispatcher/EventClass.h \
           src/config/ConfigLoader.h \
           src/config/ConfigProperties.h \
           src/config/Database.h \
           src/errors/Error.h \
           src/filesystem/FileSystem.h \
           src/filesystem/LocalFS.h \
           src/filesystem/ADB.h \
           src/filesystem/File.h \
           src/filesystem/Directory.h \
           src/filesystem/RegularFile.h \
           src/gui/GUI.h \
           src/gui/MainWindow.h \
           src/gui/DiffListBox.h \
           src/synchronizer/Synchronizer.h \
           src/system/CommandExecutor.h \
           src/synchronizer/DiffScanner.h \
           src/diffs/Diff.h \
           src/diffs/DiffType.h \
           src/threads/SingleThread.h \
           src/threads/Thread.h \
           src/threads/LoopThread.h \
           src/App.h \
           src/utils/string_utils.h \
           src/errors/SystemCmdError.h \
           src/errors/ParseError.h \
           src/events/DiffInvertedButtonClicked.h \
           src/events/DiffListUpdateRequest.h \
           src/events/DiffRemovedButtonClicked.h \
           src/events/DiffScanButtonClicked.h \
           src/events/DiffPartialScanCompleted.h \
           src/events/DiffSyncCompleted.h \
           src/events/ExecuteAllDiffsButtonClicked.h \
           src/events/ExecuteDiffButtonClicked.h \
           src/events/ProgressUpdated.h \
           src/events/ShowUIMessageRequest.h \
           src/synchronizer/DiffSync.h \
           src/test/TestApp.h \
           src/test/LocalFSTest.h \
           src/test/QApplicationTest.h \
           src/test/ThreadsTest.h \
           src/test/BusyboxTest.h

FORMS += forms/mainwindow.ui

SOURCES += src/main.cpp \
           src/logger/Logger.cpp \
	       src/dispatcher/EventDispatcher.cpp \
	       src/dispatcher/IEventObserver.cpp \
	       src/dispatcher/Event.cpp \
	       src/dispatcher/EventClass.cpp \
	       src/config/ConfigLoader.cpp \
	       src/config/ConfigProperties.cpp \
	       src/config/Database.cpp \
	       src/errors/Error.cpp \
	       src/filesystem/FileSystem.cpp \
	       src/filesystem/LocalFS.cpp \
	       src/filesystem/ADB.cpp \
	       src/filesystem/File.cpp \
	       src/filesystem/Directory.cpp \
	       src/filesystem/RegularFile.cpp \
	       src/gui/GUI.cpp \
	       src/gui/MainWindow.cpp \
	       src/gui/DiffListBox.cpp \
	       src/synchronizer/Synchronizer.cpp \
	       src/system/CommandExecutor.cpp \
	       src/synchronizer/DiffScanner.cpp \
	       src/diffs/Diff.cpp \
	       src/threads/SingleThread.cpp \
	       src/threads/Thread.cpp \
	       src/threads/LoopThread.cpp \
	       src/App.cpp \
           src/utils/string_utils.cpp \
           src/errors/SystemCmdError.cpp \
           src/errors/ParseError.cpp \
           src/test/TestApp.cpp \
           src/synchronizer/DiffSync.cpp


DESTDIR = bin
OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui
