#include "vector.h"

TVector::TVector() : Capacity(), Size(), Ptr(nullptr) {}

TVector::TVector(std::initializer_list<uint32_t> brace_enclosed_list)
    : Capacity(brace_enclosed_list.size())
    , Size(brace_enclosed_list.size())
    , Ptr(new uint32_t[Capacity])
{
    std::copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
}

TVector::TVector(TVectorView view)
    : Capacity(view.size())
    , Size(view.size())
    , Ptr(new uint32_t[Capacity])
{
    std::copy(view.begin(), view.end(), begin());
}

TVector::TVector(const TVector &obj)
    : Capacity(obj.Capacity)
    , Size(obj.Size)
    , Ptr(new uint32_t[Capacity])
{
    std::copy(obj.begin(), obj.end(), begin());
}

TVector::TVector(TVector &&obj)
    : Capacity(obj.Capacity)
    , Size(obj.Size)
    , Ptr(obj.Ptr)
{
    obj.Ptr = nullptr;
    obj.Capacity = obj.Size = 0;
}

TVector::~TVector() {
    delete[] Ptr;
}

bool TVector::empty() const {
    return !Size;
}

size_t TVector::capacity() const {
    return Capacity;
}

size_t TVector::size() const {
    return Size;
}

void TVector::push_back(uint32_t val) {
    if (Size == Capacity) {
        Capacity = Capacity ? Capacity << 1 : 1;
        uint32_t *tmp = new uint32_t[Capacity];         // not safe
        std::copy(begin(), end(), tmp);
        delete[] Ptr;
        Ptr = tmp;
    }
    Ptr[Size++] = val;
}

void TVector::pop_back() {
    --Size;                                             // pop_back() from empty stl vector is UB
}

void TVector::clear() {
    Size = 0;
}

void TVector::reserve(size_t cap) {
    if (cap > Capacity) {
        uint32_t *tmp = new uint32_t[cap];              // not safe
        std::copy(begin(), end(), tmp);
        delete[] Ptr;
        Ptr = tmp;
        Capacity = cap;
    }
}

void TVector::resize(size_t s) {
    if (Size < s) {
        reserve(s);
        std::fill(end(), begin() + s, 0);
    }
    Size = s;
}

TVector& TVector::operator=(std::initializer_list<uint32_t> brace_enclosed_list) {
    if (capacity() < brace_enclosed_list.size()) {
        delete[] Ptr;
        Capacity = brace_enclosed_list.size();
        Ptr = new uint32_t[Capacity];                   // not safe
    }
    Size = brace_enclosed_list.size();
    std::copy(brace_enclosed_list.begin(), brace_enclosed_list.end(), begin());
    return *this;
}

TVector& TVector::operator=(TVectorView view) {
    if (capacity() < view.size()) {
        delete[] Ptr;
        Capacity = view.size();
        Ptr = new uint32_t[Capacity];                   // not safe
    }
    Size = view.size();
    std::copy(view.begin(), view.end(), begin());
    return *this;
}

TVector& TVector::operator=(const TVector &obj) {
    if (capacity() < obj.capacity()) {
        delete[] Ptr;
        Capacity = obj.Capacity;
        Ptr = new uint32_t[Capacity];                   // not safe
    }
    Size = obj.size();
    std::copy(obj.begin(), obj.end(), begin());
    return *this;
}

TVector& TVector::operator=(TVector &&obj) {
    delete[] Ptr;
    Capacity = obj.capacity();
    Size = obj.size();
    Ptr = obj.begin();
    obj.Ptr = nullptr;
    obj.Capacity = obj.Size = 0;
    return *this;
}

uint32_t TVector::back() const {
    return Ptr[Size - 1];                                // back() from empty stl vector is UB
}

uint32_t& TVector::back() {
    return Ptr[Size - 1];                                // back() from empty stl vector is UB
}

uint32_t TVector::operator[](size_t idx) const {
    return Ptr[idx];
}

uint32_t& TVector::operator[](size_t idx) {
    return Ptr[idx];
}

const uint32_t* TVector::begin() const {
    return Ptr;
}

const uint32_t* TVector::end() const {
    return Ptr + Size;
}

uint32_t* TVector::begin() {
    return Ptr;
}

uint32_t* TVector::end() {
    return Ptr + Size;
}

std::reverse_iterator<const uint32_t*> TVector::rbegin() const {
    return std::make_reverse_iterator(Ptr + Size);
}

std::reverse_iterator<const uint32_t*> TVector::rend() const {
    return std::make_reverse_iterator(Ptr);
}

std::reverse_iterator<uint32_t*> TVector::rbegin() {
    return std::make_reverse_iterator(Ptr + Size);
}

std::reverse_iterator<uint32_t*> TVector::rend() {
    return std::make_reverse_iterator(Ptr);
}

TVector::TVectorView::TVectorView(size_t s, const uint32_t *p) : Size(s), Ptr(p) {}

TVector::TVectorView::TVectorView(const TVector &obj) : Size(obj.size()), Ptr(obj.begin()) {}

TVector::TVectorView::TVectorView(const TVectorView &obj) : Size(obj.size()), Ptr(obj.begin()) {}

TVector::TVectorView::TVectorView(TVectorView &&obj) : Size(obj.size()), Ptr(obj.begin()) {
    obj.Size = 0;
    obj.Ptr = nullptr;
}

bool TVector::TVectorView::empty() const {
    return !Size;
}

size_t TVector::TVectorView::size() const {
    return Size;
}

TVector::TVectorView& TVector::TVectorView::operator=(TVectorView &&obj) {
    Size = obj.size();
    Ptr = obj.begin();
    obj.Size = 0;
    obj.Ptr = nullptr;
    return *this;
}

uint32_t TVector::TVectorView::operator[](size_t idx) const {
    if (idx < size()) {
        return Ptr[idx];
    }
    return 0;
}

const uint32_t* TVector::TVectorView::begin() const {
    return Ptr;
}

const uint32_t* TVector::TVectorView::end() const {
    return Ptr + size();
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
