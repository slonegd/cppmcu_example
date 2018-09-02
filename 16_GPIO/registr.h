#pragma once

template<class Periph> struct Pointer {
   static auto get() { return reinterpret_cast<Periph*>(Periph::Base); }
};

template<class T>
__IO uint32_t& dword(__IO T* p) { return *reinterpret_cast<__IO uint32_t*>(p); }

