#pragma once

#include <cstddef>

class TLinearAllocator {
private:
    size_t size;
    size_t offset;
    char *ptr;
public:
    TLinearAllocator();
    TLinearAllocator(const TLinearAllocator&) = delete;
    ~TLinearAllocator();

    TLinearAllocator& operator=(const TLinearAllocator&) = delete;

    void makeAllocator(size_t maxSize);
    char* alloc(size_t s);
    void reset();
};
