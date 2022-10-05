#pragma once
#include <vector>
template <typename T> class multivec
{
  public:
    multivec() = default;
    multivec(uint64_t rowIn, uint64_t colomIn);
    T &operator()(uint64_t rowIn, uint64_t colomIn);
    size_t size() const;
    size_t sizeRow() const;
    size_t sizeColom() const;
    bool empty();

  private:
    std::vector<T> map;
    uint64_t row;
    uint64_t colom;
};

template <typename T>
multivec<T>::multivec(uint64_t rowIn, uint64_t colomIn) : row(rowIn), colom(colomIn), map(rowIn * colomIn)
{
}

template <typename T> T &multivec<T>::operator()(uint64_t rowIn, uint64_t colomIn)
{
    return map[rowIn * colom + colomIn];
}

template <typename T> size_t multivec<T>::size() const
{
    return row * colom;
}
template <typename T> size_t multivec<T>::sizeRow() const
{
    return row;
}
template <typename T> size_t multivec<T>::sizeColom() const
{
    return colom;
}
template <typename T> bool multivec<T>::empty()
{
    return map.empty();
}

template <typename T> class multivec3
{
  public:
    multivec3() = default;
    multivec3(uint64_t rowIn, uint64_t colomIn, uint64_t dimIn);
    T &operator()(uint64_t rowIn, uint64_t colomIn, uint64_t dimIn);

    size_t size() const;
    size_t sizeRow() const;
    size_t sizeColom() const;
    size_t sizeDim() const;
    bool empty();

  private:
    std::vector<T> map;
    uint64_t row;
    uint64_t colom;
    uint64_t dim;
};

template <typename T>
multivec3<T>::multivec3(uint64_t rowIn, uint64_t colomIn, uint64_t dimIn)
    : row(rowIn), colom(colomIn), dim(dimIn), map(rowIn * colomIn * dimIn)
{
}

template <typename T> T &multivec3<T>::operator()(uint64_t rowIn, uint64_t colomIn, uint64_t dimIn)
{
    return map[dim * dimIn + rowIn * colom + colomIn];
}

template <typename T> size_t multivec3<T>::size() const
{
    return row * colom;
}
template <typename T> size_t multivec3<T>::sizeRow() const
{
    return row;
}
template <typename T> size_t multivec3<T>::sizeColom() const
{
    return colom;
}
template <typename T> size_t multivec3<T>::sizeDim() const
{
    return dim;
}
template <typename T> bool multivec3<T>::empty()
{
    return map.empty();
}
