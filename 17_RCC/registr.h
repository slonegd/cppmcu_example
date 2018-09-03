#pragma once

template<class Periph> struct Pointer {
   static auto get() { return reinterpret_cast<Periph*>(Periph::Base); }
};

