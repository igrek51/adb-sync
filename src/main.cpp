#include <iostream>
#include "logger/Logger.h"
#include "config/ConfigLoader.h"

#include "utils/string_utils.h"

#include <vector>

using namespace std;

//TODO smart pointers, shared ...
//TODO c++ 11 standards: nullptr, foreach, auto, enum class

int main(int argc, char** argv) {

    Logger::info("hello");

    return 0;
}


