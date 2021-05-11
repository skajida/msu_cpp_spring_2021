#pragma once

#include <cstddef>

template <typename T, typename PointerT=T*, typename ReferenceT=T&>
class TIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = PointerT;
    using reference = ReferenceT;
private:
    pointer Ptr;
public:
    explicit TIterator(pointer);

    std::weak_ordering operator<=>(const TIterator&) const = default;

    reference operator*() const;
    reference operator[](difference_type) const;

    difference_type operator-(TIterator) const noexcept;

    TIterator operator+(difference_type) const noexcept;
    TIterator operator-(difference_type) const noexcept;

    TIterator& operator+=(difference_type) noexcept;
    TIterator& operator-=(difference_type) noexcept;

    TIterator& operator++() noexcept;
    TIterator& operator--() noexcept;
    TIterator operator++(int) noexcept;
    TIterator operator--(int) noexcept;

    operator pointer() const noexcept;
};

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>::TIterator(pointer ptr) : Ptr(ptr) {}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>::reference TIterator<T, PointerT, ReferenceT>::operator*() const {
    return *Ptr;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>::reference TIterator<T, PointerT, ReferenceT>::operator[](ptrdiff_t idx) const {
    return Ptr[idx];
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>::difference_type TIterator<T, PointerT, ReferenceT>::operator-(TIterator obj) const noexcept {
    return Ptr - obj.Ptr;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT> TIterator<T, PointerT, ReferenceT>::operator+(difference_type shift) const noexcept {
    return TIterator(Ptr + shift);
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT> TIterator<T, PointerT, ReferenceT>::operator-(difference_type shift) const noexcept {
    return TIterator(Ptr - shift);
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>& TIterator<T, PointerT, ReferenceT>::operator+=(difference_type shift) noexcept {
    Ptr += shift;
    return *this;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>& TIterator<T, PointerT, ReferenceT>::operator-=(difference_type shift) noexcept {
    Ptr -= shift;
    return *this;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>& TIterator<T, PointerT, ReferenceT>::operator++() noexcept {
    ++Ptr;
    return *this;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>& TIterator<T, PointerT, ReferenceT>::operator--() noexcept {
    --Ptr;
    return *this;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT> TIterator<T, PointerT, ReferenceT>::operator++(int) noexcept {
    TIterator res(Ptr);
    ++Ptr;
    return res;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT> TIterator<T, PointerT, ReferenceT>::operator--(int) noexcept {
    TIterator res(Ptr);
    --Ptr;
    return res;
}

template <typename T, typename PointerT, typename ReferenceT>
TIterator<T, PointerT, ReferenceT>::operator TIterator<T, PointerT, ReferenceT>::pointer() const noexcept {
    return Ptr;
}
