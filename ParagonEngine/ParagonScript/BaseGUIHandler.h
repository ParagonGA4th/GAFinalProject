#pragma once
#include "GUIDefinesAndStructs.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"

#include <unordered_map>
#include <functional>
#include <array>

//Component�� �ڽ��� �ƴϴ�. 
//������ �����Ѵ�. �÷ο� ������ ����.
//�������̽� Ȱ�� ���� ���.
//GUI Handler���� �������̽�.

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
		//�ʱ� ���·� �ǵ����� �Լ�. � ������ �Ǵ�, �����ؾ� �� ��.
		void ResetToInitialState();

		//ResetToInitialState ����, ���� ����ü���� �� ���� ���� ��� �޾ƾ� ��.
		virtual void AdditionalReset() {};
		//...

	protected:
		//Derived Class���� GrabManagedObject �κп��� ȣ���ϴ� �κ�.
		void GrabOrganizeAllGUI(Pg::Data::GameObject* obj);

		//Derived Class���� �̸� �ʼ������� �����ؾ� �Ѵ� - �߻�.
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


