#pragma once

///���ø� �̱��� Ŭ����
///-> singleton-cpp ���̺귯�� ����, DLL �ٿ���� �Ѵ� �ܺ� �̱������� ��� ���� ��. 

namespace Pg::Core
{
	template<typename T>
	class Singleton
	{
	public:
		static T* Instance();

	protected:
		Singleton() = default;

	private:
		Singleton(const Singleton& rhs) = delete;
		Singleton(Singleton&& rhs) = delete;
		Singleton& operator=(const Singleton& rhs) = delete;
		Singleton& operator=(Singleton&& rhs) = delete;

	};

	template<typename T>
	inline T* Singleton<T>::Instance()
	{
		static T* instance = new T();
		return instance;
	}
}
