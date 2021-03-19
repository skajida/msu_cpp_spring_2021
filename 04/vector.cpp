#include "vector.h"

TVector::TVector() : _capacity(), _size(), ptr(nullptr) {}

TVector::TVector(std::initializer_list<uint32_t> brace_enclosed_list)
    : _capacity(brace_enclosed_list.size())
    , _size(brace_enclosed_list.size())
    , ptr(new uint32_t[_capacity])
{
    std::copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
}

TVector::TVector(TVectorView view)
    : _capacity(view.size())
    , _size(view.size())
    , ptr(new uint32_t[_capacity])
{
    std::copy(view.begin(), view.end(), begin());
}

TVector::TVector(const TVector &obj)
    : _capacity(obj._capacity)
    , _size(obj._size)
    , ptr(new uint32_t[_capacity])
{
    std::copy(obj.begin(), obj.end(), begin());
}

TVector::TVector(TVector &&obj)
    : _capacity(obj._capacity)
    , _size(obj._size)
    , ptr(obj.ptr)
{
    obj.ptr = nullptr;
    obj._capacity = obj._size = 0;
}

TVector::~TVector() {
    delete[] ptr;
}

bool TVector::empty() const {
    return !_size;
}

size_t TVector::capacity() const {
    return _capacity;
}

size_t TVector::size() const {
    return _size;
}

void TVector::push_back(uint32_t val) {
    if (_size == _capacity) {
        _capacity = _capacity ? _capacity << 1 : 1;
        uint32_t *tmp = new uint32_t[_capacity];        // not safe
        std::copy(begin(), end(), tmp);
        delete[] ptr;
        ptr = tmp;
    }
    ptr[_size++] = val;
}

void TVector::pop_back() {
    --_size;                                             // pop_back() from empty stl vector is UB
}

void TVector::clear() {
    _size = 0;
}

void TVector::reserve(size_t cap) {
    if (cap > _capacity) {
        uint32_t *tmp = new uint32_t[cap];              // not safe
        std::copy(begin(), end(), tmp);
        delete[] ptr;
        ptr = tmp;
        _capacity = cap;
    }
}

void TVector::resize(size_t s) {
    if (_size < s) {
        reserve(s);
        std::fill(end(), begin() + s, 0);
    }
    _size = s;
}

TVector& TVector::operator=(std::initializer_list<uint32_t> brace_enclosed_list) {
    if (capacity() < brace_enclosed_list.size()) {
        delete[] ptr;
        _capacity = brace_enclosed_list.size();
        ptr = new uint32_t[_capacity];                  // not safe
    }
    _size = brace_enclosed_list.size();
    std::copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
    return *this;
}

TVector& TVector::operator=(TVectorView view) {
    if (capacity() < view.size()) {
        delete[] ptr;
        _capacity = view.size();
        ptr = new uint32_t[_capacity];                  // not safe
    }
    _size = view.size();
    std::copy(view.begin(), view.end(), begin());
    return *this;
}

TVector& TVector::operator=(const TVector &obj) {
    if (capacity() < obj.capacity()) {
        delete[] ptr;
        _capacity = obj._capacity;
        ptr = new uint32_t[_capacity];                  // not safe
    }
    _size = obj.size();
    std::copy(obj.begin(), obj.end(), begin());
    return *this;
}

TVector& TVector::operator=(TVector &&obj) {
    delete[] ptr;
    _capacity = obj.capacity();
    _size = obj.size();
    ptr = obj.begin();
    obj.ptr = nullptr;
    obj._capacity = obj._size = 0;
    return *this;
}

uint32_t TVector::back() const {
    return ptr[_size - 1];                              // back() from empty stl vector is UB
}

uint32_t& TVector::back() {
    return ptr[_size - 1];                              // back() from empty stl vector is UB
}

uint32_t TVector::operator[](size_t idx) const {
    return ptr[idx];
}

uint32_t& TVector::operator[](size_t idx) {
    return ptr[idx];
}

const uint32_t* TVector::begin() const {
    return ptr;
}

const uint32_t* TVector::end() const {
    return ptr + _size;
}

uint32_t* TVector::begin() {
    return ptr;
}

uint32_t* TVector::end() {
    return ptr + _size;
}

std::reverse_iterator<const uint32_t*> TVector::rbegin() const {
    return std::make_reverse_iterator(ptr + _size);
}

std::reverse_iterator<const uint32_t*> TVector::rend() const {
    return std::make_reverse_iterator(ptr);
}

std::reverse_iterator<uint32_t*> TVector::rbegin() {
    return std::make_reverse_iterator(ptr + _size);
}

std::reverse_iterator<uint32_t*> TVector::rend() {
    return std::make_reverse_iterator(ptr);
}

TVector::TVectorView::TVectorView(size_t s, const uint32_t *p) : _size(s), ptr(p) {}

TVector::TVectorView::TVectorView(const TVector &obj) : _size(obj.size()), ptr(obj.begin()) {}

TVector::TVectorView::TVectorView(const TVectorView &obj) : _size(obj.size()), ptr(obj.begin()) {}

TVector::TVectorView::TVectorView(TVectorView &&obj) : _size(obj.size()), ptr(obj.begin()) {
    obj._size = 0;
    obj.ptr = nullptr;
}

bool TVector::TVectorView::empty() const {
    return !_size;
}

size_t TVector::TVectorView::size() const {
    return _size;
}

TVector::TVectorView& TVector::TVectorView::operator=(TVectorView &&obj) {
    _size = obj.size();
    ptr = obj.begin();
    obj._size = 0;
    obj.ptr = nullptr;
    return *this;
}

uint32_t TVector::TVectorView::operator[](size_t idx) const {
    if (idx < size()) {
        return ptr[idx];
    }
    return 0;
}

const uint32_t* TVector::TVectorView::begin() const {
    return ptr;
}

const uint32_t* TVector::TVectorView::end() const {
    return ptr + size();
}

std::pair<TVector::TVectorView, TVector::TVectorView> TVector::TVectorView::split() const {
    return { { (size() + 1) / 2, begin() }, { size() / 2, begin() + (size() + 1) / 2 } };
}

std::pair<TVector::TVectorView, TVector::TVectorView> TVector::TVectorView::split(size_t len) const {
    return {
        { std::min(size(), len), begin() },
        { size() - std::min(size(), len), begin() + std::min(size(), len) }
    };
}
