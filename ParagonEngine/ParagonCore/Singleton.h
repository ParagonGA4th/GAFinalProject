#pragma once

///≈€«√∏¥ ΩÃ±€≈œ ≈¨∑°Ω∫

template<typename T>
class Singleton
{
public:
	static T* instance();
};

template<typename T>
inline T* Singleton<T>::instance()
{
	static T* instance = new T();
	return instance;
}