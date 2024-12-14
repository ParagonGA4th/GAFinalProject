#pragma once

///템플릿 싱글턴 클래스
///-> singleton-cpp 라이브러리 연결, DLL 바운더리 넘는 외부 싱글턴으로 사용 유도 중. 

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
