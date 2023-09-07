#include "Multithread.h"

int MTCoo1::counter = 0;
int MTCoo2::counter = 0;
int MTCoo3::counter = 0;

#if MTTesting
void MTFoo1() { std::cout << "Multi Thread foo1\n"; }
void MTFoo2() { std::cout << "Multi Thread foo2\n"; }
void MTFoo3() { std::cout << "Multi Thread foo3\n"; }
#endif

void Multithread::Init() {}












