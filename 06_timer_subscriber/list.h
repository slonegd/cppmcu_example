#pragma once

#include <iterator>

template<class T>
struct Listable
{
   T* prev {nullptr};
   T* next {nullptr};
};


template<class T>
class List
{

   T* first {nullptr};
   T* last  {nullptr};

public:
   void push_back (T* pobj)
   {
      pobj->prev = last;
      if (last)
         last->next = pobj;
      last = pobj;
      if (not first)
         first = pobj;
   }

   void push_front (T* pobj) {}

   void remove (T* pobj)
   {
      if (pobj->prev and pobj->next) {
         pobj->prev->next = pobj->next;
         pobj->next->prev = pobj->prev;
      } else if (pobj->next) {
         first = pobj->next->prev;
      } else if (pobj->prev) {
         last = pobj->prev->next;
      }
      pobj->prev = nullptr;
      pobj->next = nullptr;
   }
};
