#pragma once

template<size_t heap_size>
class Heap
{
   uint8_t memory[heap_size];
   size_t free_index {0};
public:
   void* allocate (size_t size)
   {
      void* p = reinterpret_cast<void*>(memory + free_index);
      free_index += size;
      free_index += free_index % 4; // выравнивание памяти по 4 байта
      // если нам необходимо памяти больше, чем выделено, то тут будет HardFault
      // что определиться при первой же отладке
      return (free_index < heap_size) ? p : nullptr;
      // return p; // в релизной версии проверка не нужна
   }
};

// выделяем память под динамические объекты
constexpr size_t heap_size {256};
Heap<heap_size> heap {};