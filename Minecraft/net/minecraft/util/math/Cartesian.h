#pragma once
#include <any>
namespace Cartesian
{
	template<class C, typename T>
	class Product
	{
	public:
		Product(C clazz, T iterables);
	private:
		C clazz;
		T iterables;
	};

	template <class C, typename T>
	Product<C, T>::Product(C clazz, T iterables)
		:clazz(clazz), iterables(iterables)
	{
	}

	template<class C, typename T>
	T cartesianProduct(T sets)
	{
		return Product((Iterable[])((Iterable[])toArray(Iterable.class, sets)));
	}

	template<typename T>
	T cartesianProduct(T sets)
	{
		return cartesianProduct<std::any, T>(sets);
	}

}