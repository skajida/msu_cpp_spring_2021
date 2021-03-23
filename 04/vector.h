#pragma once

#include <iterator>

class TVector {
private:
    size_t Capacity;
    size_t Size;
    uint32_t *Ptr;
public:
    class TVectorView {
    private:
        size_t Size;
        const uint32_t *Ptr;
    public:
        TVectorView(size_t, const uint32_t*);
        TVectorView(const TVector&);
        TVectorView(const TVectorView&);
        TVectorView(TVectorView&&);

        bool empty() const;
        size_t size() const;

        TVectorView& operator=(const TVectorView&) = delete;
        TVectorView& operator=(TVectorView&&);

        uint32_t operator[](size_t idx) const;

        const uint32_t* begin() const;
        const uint32_t* end() const;

        std::pair<TVectorView, TVectorView> split() const;
        std::pair<TVectorView, TVectorView> split(size_t) const;
    };

    TVector();
    TVector(std::initializer_list<uint32_t>);
    TVector(TVectorView);
    TVector(const TVector&);
    TVector(TVector&&);
    ~TVector();

    bool empty() const;
    size_t capacity() const;
    size_t size() const;

    void push_back(uint32_t val);
    void pop_back();

    void clear();
    void reserve(size_t);
    void resize(size_t);

    TVector& operator=(std::initializer_list<uint32_t>);
    TVector& operator=(TVectorView);
    TVector& operator=(const TVector&);
    TVector& operator=(TVector&&);

    uint32_t back() const;
    uint32_t& back();
    uint32_t operator[](size_t idx) const;
    uint32_t& operator[](size_t idx);

    const uint32_t* begin() const;
    const uint32_t* end() const;
    uint32_t* begin();
    uint32_t* end();

    std::reverse_iterator<const uint32_t*> rbegin() const;
    std::reverse_iterator<const uint32_t*> rend() const;
    std::reverse_iterator<uint32_t*> rbegin();
    std::reverse_iterator<uint32_t*> rend();
};
