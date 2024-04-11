#pragma once
#include "Script.h"

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