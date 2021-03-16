#include "linear_allocator.h"

#include <iostream>
#include <cassert>

void TestLinearAllocator() {
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
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(10);
        assert(allocator.alloc(0) == nullptr);
        assert(allocator.alloc(6) != nullptr);
    }
    std::cerr << "TestLinearAllocator is OK" << std::endl;
}

int main() {
    TestLinearAllocator();
    return 0;
}
