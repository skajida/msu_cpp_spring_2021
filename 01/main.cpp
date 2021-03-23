#include "linear_allocator.h"

#include <iostream>
#include <cassert>

void TestBorderConditions() {
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
}

void TestReallocation() {
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(10);
        assert(allocator.alloc(11) == nullptr);
        allocator.makeAllocator(11);
        assert(allocator.alloc(11) != nullptr);
    }
}

void TestDanglingPointer() {
    {
        TLinearAllocator allocator;
        allocator.makeAllocator(10);
        assert(allocator.alloc(0) == nullptr);
        assert(allocator.alloc(6) != nullptr);
    }
}

void TestLinearAllocator() {
    TestBorderConditions();
    TestReallocation();
    TestDanglingPointer();
    std::cerr << "TestLinearAllocator is OK" << std::endl;
}

int main() {
    TestLinearAllocator();
    return 0;
}
