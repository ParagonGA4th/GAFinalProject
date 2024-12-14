#pragma once
#include "Script.h"
#include <ctti/type_id.hpp>

/// <summary>
/// ��� Script���� �θ� �� Ŭ���� (�������̽�)
/// </summary>

namespace Pg::DataScript
{
	template<class T>
	struct ScriptInterface : public Script 
	{
	public:
		ScriptInterface(Pg::Data::GameObject* obj) : Script(obj)
		{
			//
			//register_type();
		} 
		
		virtual ~ScriptInterface()
		{
			if (!registered_)
			{
				assert(false && "BADRETURN : �ڵ� �������͵��� �ʾҴ�.");
			}
		} // registered_�� ��𿡼��� ȣ��Ǿ�� �Ѵ�.

		static inline bool register_type()
		{
			//������ T::class_identifier�� �־�� ���� �� �� �ֵ��� ���Ƴ���.
			static_assert(requires {T::class_identifier; });

			auto& registry = get_registry();
			registry[T::class_identifier] = T::create_instance;
			return true;
		}

		static const bool registered_;

	protected:
		//�����ִ� Function Final.
		virtual void DoNotInheritFromShapeDirectlyButFromShapeInterfaceInstead() final {}
	};

	template<class T>
	const bool ScriptInterface<T>::registered_ = ScriptInterface<T>::register_type();
}

//��������� CTTI ���� Ȱ���� Script ��� Ŭ�������� ��� �̸� �����ؾ� �Ѵ� ���ο�!
#define DEFINE_PARAGON_SCRIPT(T) \
public: \
    static inline const std::string class_identifier = ctti::type_id<T>().name().begin(); \
    static Script* create_instance(Pg::Data::GameObject* obj) { return new T(obj); }

//�� ����ü�� ����, Script �ܰ���� Singleton�� ������ �� �ְ� ����! DontDestroyOnLoad�� ������Ʈ�� ó�� ���� �Ѵ�.
//�� ���, �����ڸ� ��������� �����ع����� �ȵȴ�.
//�긦 ���� ��ũ��Ʈ�� ���, Ÿ ��ũ��Ʈ���� GetInstance(); �� ���� �޾ƿ� �� �ִ�.
#define DEFINE_PARAGON_SCRIPT_SINGLETON(T) \
public: \
static inline const std::string class_identifier = ctti::type_id<T>().name().begin(); \
static Script* create_instance(Pg::Data::GameObject* obj) { return T::GetInstance(obj); } \
static T* GetInstance(Pg::Data::GameObject* obj) \
{ \
	static T inst(obj); \
	return &inst; \
} \
private: \
T(Pg::Data::GameObject* obj) : ScriptInterface(obj) {}

	
