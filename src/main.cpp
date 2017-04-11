#include "App.h"
//#include "test/BusyboxTest.h"

//TODO smart pointers, shared_ptr ...

int main(int argc, char** argv) {
//	return BusyboxTest().run();
	return App(argc, argv).run();
}
