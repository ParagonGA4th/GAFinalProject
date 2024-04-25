#pragma once
#include "Script.h"
#include <ctti/type_id.hpp>

/// <summary>
/// 모든 Script들의 부모가 될 클래스 (인터페이스)
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
				assert(false && "BADRETURN : 자동 레지스터되지 않았다.");
			}
		} // registered_는 어디에선가 호출되어야 한다.

		static inline bool register_type()
		{
			//무조건 T::class_identifier가 있어야 실행 할 수 있도록 막아놨다.
			static_assert(requires {T::class_identifier; });

			auto& registry = get_registry();
			registry[T::class_identifier] = T::create_instance;
			return true;
		}

		static const bool registered_;

	protected:
		//막아주는 Function Final.
		virtual void DoNotInheritFromShapeDirectlyButFromShapeInterfaceInstead() final {}
	};

	template<class T>
	const bool ScriptInterface<T>::registered_ = ScriptInterface<T>::register_type();
}

//명시적으로 CTTI 등을 활용해 Script 상속 클래스들이 모두 이를 선언해야 한다 내부에!
#define DEFINE_PARAGON_SCRIPT(T) \
public: \
    static inline const std::string class_identifier = ctti::type_id<T>().name().begin(); \
    static Script* create_instance(Pg::Data::GameObject* obj) { return new T(obj); }

//위 구현체를 응용, Script 단계부터 Singleton을 선언할 수 있게 하자! DontDestroyOnLoad된 오브젝트에 처음 들어가야 한다.
//이 경우, 생성자를 명시적으로 정의해버리면 안된다.
//얘를 쓰는 스크립트인 경우, 타 스크립트들이 GetInstance(); 를 통해 받아올 수 있다.
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

	
