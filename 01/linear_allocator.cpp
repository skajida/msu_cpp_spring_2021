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

char* TLinearAllocator::alloc(size_t size_) {
    if (size_ && offset + size_ <= size) {
        offset += size_;
        return ptr + offset - size_;
    }
    return nullptr;
}

void TLinearAllocator::reset() {
    offset = 0;
}
