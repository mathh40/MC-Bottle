#pragma once

template<class T>
class LazyLoadBase
{
public:
	virtual ~LazyLoadBase() = default;
	T getValue();
private:
	T value;
	bool isLoaded = false;
	virtual T load() = 0;
};

template <class T>
T LazyLoadBase<T>::getValue()
{
	if (!isLoaded) {
		isLoaded = true;
		value = load();
	}

	return value;
}
