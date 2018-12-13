#pragma once
#include <cxxabi.h>

int  __cxa_guard_acquire (__cxxabiv1::__guard *) {return 1;}
void __cxa_guard_release (__cxxabiv1::__guard *) {}
void __cxa_guard_abort   (__cxxabiv1::__guard *) {};