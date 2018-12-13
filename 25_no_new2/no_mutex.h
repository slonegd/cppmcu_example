#pragma once
#include <cxxabi.h>
// using __cxxabiv1::__guard;
int __cxa_guard_acquire(__cxxabiv1::__guard *g) {return 1;}
void __cxa_guard_release (__cxxabiv1::__guard *g) {}
void __cxa_guard_abort (__cxxabiv1::__guard *) {};