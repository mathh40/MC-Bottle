#pragma once
#include <vector>
template <typename T>
class multivec
{
public:
	multivec() = default;
	multivec(uint64_t rowIn, uint64_t colomIn);
	T& operator()(uint64_t rowIn, uint64_t colomIn);
private:
	std::vector<T> map;
	uint64_t row;
	uint64_t colom;
};

template <typename T>
multivec<T>::multivec(uint64_t rowIn, uint64_t colomIn)
	:row(rowIn), colom(colomIn), map(rowIn * colomIn)
{

}

template <typename T>
T& multivec<T>::operator()(uint64_t rowIn, uint64_t colomIn)
{
	return map[rowIn * colom + colomIn];
}
