#include "linear_allocator.h"

TLinearAllocator::TLinearAllocator()
    : size(0)
    , offset(0)
    , ptr(nullptr)
{
    //
}

TLinearAllocator::~TLinearAllocator() {
    delete[] ptr;
}

void TLinearAllocator::makeAllocator(size_t maxSize) {
    delete[] ptr;
    size = maxSize;
    offset = 0;
    ptr = new char[maxSize];
}

char* TLinearAllocator::alloc(size_t s) {
    if (s && offset + s <= size) {
        offset += s;
        return ptr + offset - s;
    }
    return nullptr;
}

void TLinearAllocator::reset() {
    offset = 0;
}
