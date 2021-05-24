#pragma once

#include "allocator.h"
#include "iterator.h"

#include <iterator>

template <typename T, class Allocator=TAllocator<T>>
class TVector {
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using reference = value_type&;
    using const_reference = const reference;
    using iterator = TIterator<T>;
    using const_iterator = TIterator<T, const T*, const T&>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    constexpr static Allocator allocator = Allocator();
    size_type Capacity;
    size_type Size;
    pointer Data;
public:
    TVector();
    explicit TVector(size_t);
    TVector(std::initializer_list<value_type>);
    TVector(const TVector&);
    TVector(TVector&&);
    ~TVector();

    bool empty() const noexcept;
    size_type capacity() const noexcept;
    size_type size() const noexcept;

    template <typename... ArgsT>
    void emplace_back(ArgsT&&... ctor_params);

    void push_back(value_type val);
    void pop_back() noexcept;

    void clear() noexcept;
    void reserve(size_type);
    void resize(size_type);

    TVector& operator=(std::initializer_list<value_type>);
    TVector& operator=(const TVector&);
    TVector& operator=(TVector&&) noexcept;

    const_reference back() const noexcept;
    reference back() noexcept;

    const_reference operator[](size_type idx) const noexcept;
    reference operator[](size_type idx) noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
};

template <typename T, class Allocator>
TVector<T, Allocator>::TVector() : Capacity() , Size() , Data(nullptr) {}

template <typename T, class Allocator>
TVector<T, Allocator>::TVector(size_t size)
    : Capacity(size)
    , Size(size)
    , Data(allocator.allocate(size))
{
    std::uninitialized_default_construct(begin(), end());
}

template <typename T, class Allocator>
TVector<T, Allocator>::TVector(std::initializer_list<value_type> brace_enclosed_list)
    : Capacity(brace_enclosed_list.size())
    , Size(brace_enclosed_list.size())
    , Data(allocator.allocate(brace_enclosed_list.size()))
{
    std::uninitialized_copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
}

template <typename T, class Allocator>
TVector<T, Allocator>::TVector(const TVector &obj)
    : Capacity(obj.size())
    , Size(obj.size())
    , Data(allocator.allocate(obj.size()))
{
    std::uninitialized_copy(obj.begin(), obj.end(), begin());
}

template <typename T, class Allocator>
TVector<T, Allocator>::TVector(TVector &&obj)
    : Capacity(obj.capacity())
    , Size(obj.size())
    , Data(obj.Data)
{
    obj.Size = obj.Capacity = 0;
    obj.Data = nullptr;
}

template <typename T, class Allocator>
TVector<T, Allocator>::~TVector() {
    allocator.deallocate(Data, Size);
}

template <typename T, class Allocator>
bool TVector<T, Allocator>::empty() const noexcept {
    return !Size;
}

template <typename T, class Allocator>
TVector<T, Allocator>::size_type TVector<T, Allocator>::capacity() const noexcept {
    return Capacity;
}

template <typename T, class Allocator>
TVector<T, Allocator>::size_type TVector<T, Allocator>::size() const noexcept {
    return Size;
}

template <typename T, class Allocator>
template <typename... ArgsT>
void TVector<T, Allocator>::emplace_back(ArgsT&&... ctor_params) {
    if (size() == capacity()) {
        Capacity = Capacity ? Capacity << 1 : 1;
        Data = allocator.reallocate(Data, Size, Capacity);
    }
    std::construct_at(&Data[Size++], std::forward<ArgsT&&>(ctor_params)...);
}

template <typename T, class Allocator>
void TVector<T, Allocator>::push_back(value_type val) {
    if (size() == capacity()) {
        Capacity = Capacity ? Capacity << 1 : 1;
        Data = allocator.reallocate(Data, Size, Capacity);
    }
    std::construct_at(&Data[Size++], val);
}

template <typename T, class Allocator>
void TVector<T, Allocator>::pop_back() noexcept {
    std::destroy_at(&Data[--Size]);
}

template <typename T, class Allocator>
void TVector<T, Allocator>::clear() noexcept {
    resize(0);
}

template <typename T, class Allocator>
void TVector<T, Allocator>::reserve(size_type capacity) {
    if (this->capacity() < capacity) {
        Data = allocator.reallocate(Data, Size, capacity);
        Capacity = capacity;
    }
}

template <typename T, class Allocator>
void TVector<T, Allocator>::resize(size_type size) {
    if (this->size() < size) {
        reserve(size);
        std::uninitialized_default_construct(end(), begin() + size);
    } else {
        std::destroy(begin() + size, end());
    }
    Size = size;
}

template <typename T, class Allocator>
TVector<T, Allocator>& TVector<T, Allocator>::operator=(std::initializer_list<value_type> brace_enclosed_list) {
    if (capacity() < brace_enclosed_list.size()) {
        allocator.deallocate(Data, Size);
        Capacity = brace_enclosed_list.size();
        Data = allocator.allocate(brace_enclosed_list.size());
    } else {
        clear();
    }
    Size = brace_enclosed_list.size();
    std::uninitialized_copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
    return *this;
}

template <typename T, class Allocator>
TVector<T, Allocator>& TVector<T, Allocator>::operator=(const TVector &obj) {
    if (capacity() < obj.size()) {
        allocator.deallocate(Data, Size);
        Capacity = obj.capacity();
        Data = allocator.allocate(obj.size());
    } else {
        clear();
    }
    Size = obj.size();
    std::uninitialized_copy(obj.begin(), obj.end(), begin());
    return *this;
}

template <typename T, class Allocator>
TVector<T, Allocator>& TVector<T, Allocator>::operator=(TVector &&obj) noexcept {
    Capacity = obj.capacity();
    Size = obj.size();
    allocator.deallocate(Data);
    Data = obj.Data;
    obj.Size = obj.Capacity = 0;
    obj.Data = nullptr;
    return *this;
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_reference TVector<T, Allocator>::back() const noexcept {
    return Data[Size - 1];
}

template <typename T, class Allocator>
TVector<T, Allocator>::reference TVector<T, Allocator>::back() noexcept {
    return Data[Size - 1];
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_reference TVector<T, Allocator>::operator[](size_type idx) const noexcept {
    return Data[idx];
}

template <typename T, class Allocator>
TVector<T, Allocator>::reference TVector<T, Allocator>::operator[](size_type idx) noexcept {
    return Data[idx];
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_iterator TVector<T, Allocator>::begin() const noexcept {
    return const_iterator(Data);
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_iterator TVector<T, Allocator>::end() const noexcept {
    return const_iterator(Data + Size);
}

template <typename T, class Allocator>
TVector<T, Allocator>::iterator TVector<T, Allocator>::begin() noexcept {
    return iterator(Data);
}

template <typename T, class Allocator>
TVector<T, Allocator>::iterator TVector<T, Allocator>::end() noexcept {
    return iterator(Data + Size);
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_reverse_iterator TVector<T, Allocator>::rbegin() const noexcept {
    return std::make_reverse_iterator(Data + Size);
}

template <typename T, class Allocator>
TVector<T, Allocator>::const_reverse_iterator TVector<T, Allocator>::rend() const noexcept {
    return std::make_reverse_iterator(Data);
}

template <typename T, class Allocator>
TVector<T, Allocator>::reverse_iterator TVector<T, Allocator>::rbegin() noexcept {
    return std::make_reverse_iterator(Data + Size);
}

template <typename T, class Allocator>
TVector<T, Allocator>::reverse_iterator TVector<T, Allocator>::rend() noexcept {
    return std::make_reverse_iterator(Data);
}
