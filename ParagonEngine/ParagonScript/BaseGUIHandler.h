#pragma once
#include "GUIDefinesAndStructs.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"

#include <unordered_map>
#include <functional>
#include <array>

//Component의 자식은 아니다. 
//별개로 존재한다. 플로우 관리를 위해.
//인터페이스 활용 다중 상속.
//GUI Handler들의 인터페이스.

namespace Pg::Data
{
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class BaseGUIHandler
	{
	public:
		inline static const float STAMINA_GUI_Y_OFFSET = 4.f;

	public:
		//초기 상태로 되돌리는 함수. 어떤 이유가 되던, 리셋해야 할 때.
		void ResetToInitialState();

		//ResetToInitialState 말고, 개별 구현체별로 할 일이 있을 경우 받아야 함.
		virtual void AdditionalReset() {};
		//...

	protected:
		//Derived Class들이 GrabManagedObject 부분에서 호출하는 부분.
		void GrabOrganizeAllGUI(Pg::Data::GameObject* obj);

		//Derived Class들은 이를 필수적으로 구현해야 한다 - 추상.
		virtual void AssignPointersToGUI() abstract;

		void GetLifeUIObjects(Pg::Data::GameObject* obj);
		void UpdateLife();

		void SetTutorialExplainUI(Pg::Data::GameObject* obj);

	protected:
		//Obj Name / GUIAggregate
		std::unordered_map<std::string, GUIAggregate> _managedGuiObjectList;

		std::array<Pg::Data::ImageRenderer*, 3> _heartArray;

		int* _playerLifePointer{ nullptr };
		Pg::Data::ImageRenderer* _avoidUI;
		Pg::Data::ImageRenderer* _basicAttackUI;
		Pg::Data::ImageRenderer* _interactionKeyUI;
		Pg::Data::ImageRenderer* _interactionUI;
		Pg::Data::ImageRenderer* _jumpUI;
		Pg::Data::ImageRenderer* _MoveUI;
		Pg::Data::ImageRenderer* _strongAttackUI;
	};
}


