#pragma once
#include "MovingObjectDefinesAndStructs.h"
#include "IConfinedArea.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>

namespace Pg::DataScript
{
	class DeathPlane;
	class AreaPassingTrigger;
}

//Component�� �ڽ��� �ƴϴ�. 
//������ �����Ѵ�. �÷ο� ������ ����.
//�������̽� Ȱ�� ���� ���.
// Area Handler���� �������̽�.

namespace Pg::DataScript
{
	class HandlerBundle3D;

	class BaseAreaHandler
	{
	public:
		BaseAreaHandler(unsigned int entireAreaCount);

		//�� �̻� Virtual�� �ƴϴ�.
		//�ʱ� ���·� �ǵ����� �Լ�. � ������ �Ǵ�, �����ؾ� �� ��.
		void ResetToInitialState();

		//������ ���¸��� �ʿ��� ��. 
		//virtual void ResetAreaUntil(int index) abstract;
		void ResetAreaWithIndex(unsigned int index);

		// AreaPassingTrigger�� ���� :
		// ���� Player�� ��� �ִ��� ���޵� �� �ְ�.
		void SetCurrentAreaIndex(unsigned int index);
		
		//Player�� Death Plane�� �������� ��.
		void OnPlayerHitDeathPlane();

		//�ܺο��� ȣ���. Ư�� Area�� �ִ� IConfinedArea���� Activate ����.
		void SetActivateConfinedAreaIndex(unsigned int index, bool val);

		//...

	protected:
		//Derived Class���� GrabManagedObject �κп��� ȣ���ϴ� �κ�.
		void GrabOrganizeAllAreas(Pg::Data::GameObject* obj);

	public:
		//�ڽ��� ���� HandlerBundle3D�� �˾ƾ� �Ѵ�.
		HandlerBundle3D* _belongHandlerBundle3D{ nullptr };

	protected:
		unsigned int _entireAreaCount;

		// MovingObject ������ ����.
		// ObjectName / MovingObject
		// Area���� ����� ����ϱ�.
		std::map<unsigned int,
			std::unordered_map<std::string,
			MovingObjectAggregate>>  _managedMovingObjectList;

		//�ϴ�, BattleArea�� ���� �����ؾ� �Ѵ�.
		//IConfinedArea��� �������̽��� �ΰ�, 
		//�ش� ���� ����. Event Trigger ������ �׷���.
		//���� �ε��� : Area Index.
		//���빰 : �ε����� ���ӵ� IConfinedArea��.
		std::vector<std::unique_ptr<std::vector<IConfinedArea*>>> _confinedAreaList;

		//�������� �״� Death Plane�� ����. 
		std::vector<DeathPlane*> _deathPlaneList;

		//Area ���� �����ϴ� Trigger�� ���� ����. Trigger / Area Index
		std::map<AreaPassingTrigger*, unsigned int> _areaTriggerMap;

		

		//���� Player�� ���� Area Index�� ����.
		unsigned int _currentAreaIndex{ 0 };


	};
}


