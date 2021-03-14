#include "linear_allocator.h"

#include <iostream>
#include <cassert>

void Test() {
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(50);
        assert(allocator.alloc(50) != nullptr);
        allocator.reset();
        assert(allocator.alloc(49) != nullptr);
        assert(allocator.alloc(1) != nullptr);
        allocator.reset();
        assert(allocator.alloc(10) != nullptr);
        assert(allocator.alloc(41) == nullptr);
    }
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(50);
        assert(allocator.alloc(50) != nullptr);
        allocator.reset();
        assert(allocator.alloc(49) != nullptr);
        assert(allocator.alloc(1) != nullptr);
        allocator.reset();
        assert(allocator.alloc(10) != nullptr);
        assert(allocator.alloc(40) != nullptr);
    }
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(10);
        assert(allocator.alloc(11) == nullptr);
        allocator.makeAllocator(11);
        assert(allocator.alloc(11) != nullptr);
    }
    std::cerr << "Test is OK" << std::endl;
}

int main() {
    Test();
    return 0;
}
