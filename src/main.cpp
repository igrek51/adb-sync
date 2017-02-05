#include <iostream>
#include "App.h"

using namespace std;

//TODO smart pointers, shared_ptr ...
//TODO c++ 11 standards: nullptr, foreach, auto, enum class

int main(int argc, char** argv) {

    App* app = new App(argc, argv);
    int result = app->run();
    delete app;

    return result;
}


