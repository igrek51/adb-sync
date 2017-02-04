#include <iostream>
#include "logger/Logger.h"

using namespace std;

//TODO smart pointers, shared ...
//TODO c++ 11 standards: nullptr, foreach, auto, enum class

int main(int argc, char** argv) {
//    App* app = new App(argc, argv);
//    delete app;

    Logger::info("hello, logger");

    return 0;
}


