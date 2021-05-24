#pragma once

#include <memory>
#include <cstddef>
#include <cstdlib>

template <typename T>
class TAllocator {
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = value_type*;
    using const_pointer = const pointer;

    pointer allocate(size_type) const;
    pointer reallocate(pointer, size_type old_size, size_type new_size) const;
    void deallocate(pointer, size_type) const;
};

template <typename T>
TAllocator<T>::pointer TAllocator<T>::allocate(size_type size) const {
    return static_cast<pointer>(malloc(size * sizeof(value_type)));
}

template <typename T>
TAllocator<T>::pointer TAllocator<T>::reallocate(pointer ptr, size_type old_size, size_type new_size) const {
    pointer tmp = allocate(new_size);
    std::uninitialized_move_n(tmp, std::min(old_size, new_size), ptr);
    deallocate(ptr, old_size);
    return tmp;
}

template <typename T>
void TAllocator<T>::deallocate(pointer ptr, size_type size) const {
    std::destroy_n(ptr, size);
    free(ptr);
}
