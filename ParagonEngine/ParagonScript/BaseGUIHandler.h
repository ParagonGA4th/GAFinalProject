#pragma once
#include "GUIDefinesAndStructs.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>

//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
//GUI Handler들의 인터페이스.

namespace Pg::DataScript
{
	class BaseGUIHandler
	{
	public:
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		void ResetToInitialState();
		//...

	protected:
		//Derived Class들이 GrabManagedObject 부분에서 호출하는 부분.
		void GrabOrganizeAllGUI(Pg::Data::GameObject* obj);

		//Derived Class들은 이를 필수적으로 구현해야 한다 - 추상.
		virtual void AssignPointersToGUI() abstract;

	protected:
		//Obj Name / GUIAggregate
		std::unordered_map<std::string, GUIAggregate> _managedGuiObjectList;
	};
}


