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
      if (pobj->prev and pobj->next) {    // если в серединке
         pobj->prev->next = pobj->next;
         pobj->next->prev = pobj->prev;
      } else if (pobj->next) {            // если первый
         first = pobj->next;
         first->prev = nullptr;
         if (not first) last = nullptr;
      } else if (pobj->prev) {            // если последний
         last = pobj->prev;
         last->next = nullptr;
         if (not last) first = nullptr;
      }
      pobj->prev = nullptr;
      pobj->next = nullptr;
   }
};
