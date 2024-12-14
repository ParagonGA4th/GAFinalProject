#pragma once
#include "EnemyDefinesAndStructs.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>

//Component�� �ڽ��� �ƴϴ�. 
//������ �����Ѵ�. �÷ο� ������ ����.
//�������̽� Ȱ�� ���� ���.
//Enemy Handler���� �������̽�.

namespace Pg::DataScript
{
	class HandlerBundle3D;

	class BaseEnemyHandler
	{
	public:
		BaseEnemyHandler(unsigned int entireAreaCount);

		//�� �̻� Virtual�� �ƴϴ�.
		//�ʱ� ���·� �ǵ����� �Լ�. � ������ �Ǵ�, �����ؾ� �� ��.
		void ResetToInitialState();

		//���� �� ���� ���¶�� �����ϰ�, �� �׾����� Reset�� ��Ų��. 
		void ResetIfNotDead();

		//std::transformó��, �� �Լ����� �ش� �Լ��� ȣ���Ų��.
		//IEnemyBehaviour�� ������� ȣ��Ǵ� ��.
		void TransformEachEnemy(std::function<void(IEnemyBehaviour*)> func);

		//���� Enemy�� ������� ��, Handler���� �˷���� �Ѵ�.
		//��ü�� ������� ��, BattleArea ���� ��ҵ鿡�� �˷��ֱ�� �ؾ� �Ѵ�.
		//������ ȣ���� �Ǿ��.
		void FromEnemyNotifyDead(const std::string& tagName, IEnemyBehaviour* behav);

	protected:
		//Derived Class���� GrabManagedObject �κп��� ȣ���ϴ� �κ�.
		void GrabOrganizeAllEnemies(Pg::Data::GameObject* obj);

	public:
		//�ڽ��� ���� HandlerBundle3D�� �˾ƾ� �Ѵ�.
		HandlerBundle3D* _belongHandlerBundle3D{ nullptr };
	
	protected:
		unsigned int _entireAreaCount;

		//Area Index // <Tag Name / EnemyAgg>
		std::map<unsigned int,
			std::unordered_map<std::string, std::vector<EnemyAggregate>>> _managedMonstersList;
	};
}

