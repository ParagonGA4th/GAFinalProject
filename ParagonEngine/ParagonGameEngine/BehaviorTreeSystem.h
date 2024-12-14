#pragma once

#include "../ParagonData/BtNodes/BTreeShareData.h"

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include <behaviortree_cpp_v3/bt_factory.h>

/// <summary>
/// Behavior Tree System. ���� ������ Behavior Tree�� �� ������Ʈ�� �����Ѵ�.
/// </summary>

namespace Pg::Data
{
	class Animator;
}

namespace Pg::Engine
{
	class SceneSystem;
}

namespace Pg::Engine::BTree
{
	class BehaviorTreeSystem
	{
	public:
		BehaviorTreeSystem();
		~BehaviorTreeSystem();

		void Initialize(const std::string& resourceListPath);

		//Scene�� �ٲ���� �� (+ Initialize) �� ȣ��Ǵ� �Լ�. BT ����Ʈ ������Ʈ.
		void SyncSceneActiveBT();

		//���� ������Ʈ.
		void Update();

		//���� Uniform�� Animator�� ���, ��ü���� �������� �ʰ�, Component�� Object ������ �ִٰ� �ؼ� �������� �ʴ´�.
		//AddUniformRunningAnimator()�� ȣ���ؼ� (API) -> Update ������ ���Խ�Ű��,
		//RemoveUniformRunningAnimator()�� ȣ���ؼ� Update �������� �����Ѵ�.
		//Instanced�� Component�� �����ϸ� Update ������ ���Եȴ�.
		void AddUniformRunningAnimator(const std::string& path);
		void RemoveUniformRunningAnimator(const std::string& path);

	public:
		//EngineMain���� ȣ��Ǿ�� ��. 
		//Scene�� �ٲ�� Animator �� �� �׿���� ��.
		//void OnSceneChange(Pg::Data::Scene* changedScene);

	private:
		//�ش� Location�鿡

		//��� Leaf Node���� ���������� �� �Լ����� Register�Ǿ�� �Ѵ�.
		void InitAllLeafNodes();
		//��� XML ���ϵ� BehaviorTreeSystem�� �°� �ε�.
		void LoadAllXMLFiles(const std::string& resourceListPath);

		//XML (Uniform) ���� �ϳ� BT::Tree�� �ε�.
		void LoadSingleUniformXMLFile(const std::string& path);
		//XML (Instanced) ���� �ϳ� �̸� TEXT�� �޾ƿ���. 
		void LoadSingleInstancedXMLFile(const std::string& path);

		//Update ���ο� ȣ��Ǵ� �Լ���.
		void UpdateAnimators();
		void ManageAnimatorList();


	private:
		//SceneSystem �̱��� ����.
		Pg::Engine::SceneSystem* _sceneSystem = nullptr;

		//��ü���� ComponentList ����.
		//Blackboard�� ���� Share�� �ʿ䰡 ����. 
		//������ ����� �������� Instance���� �����Ǵ� Variable�� ����� �� �ְ� �ϱ� ����.
		std::vector<Pg::Data::Animator*> _activeInstancedAnimatorList;
		std::vector<std::pair<std::string, BT::Tree*>> _activeUniformAnimatorList;
		
		//Storage
		//��ü���� (Uniform) BT::Tree ����. List. ����μ��� ResourceManager�� ���ұ��� �Ѳ����� �����ϰ� �ִ� ��. 
		//Tree ��ü ��ü�� �����Ѵ�. (�ּҸ� �Ҵ����ִ� ����� �Ǿ�� �Ѵ� ���� Animator���״�)
		std::unordered_map<std::string, BT::Tree> _uniformTreeStorage;

		//Instanced Animator Component�� ���, 
		//�׷����� List ���� ���� -> �ش� File Location�� Tree�� �ݵ�� ���� �� �ְ� �����ϴ� ��.
		// -> �̸� Text�� �о�ͼ� ��Ÿ�ӿ� Filesystem Ȱ���ϴ� ��찡 ���� �ϱ� ���� Change.
		std::unordered_map<std::string, std::string> _instancedTreePathContentStorage;

		//Scope Per Frame
		//�� ������ ���ο� �߰� / ������ ���� ������ �����ϴ� String Path List.
		std::vector<std::string> _toAddPerFrameUniformPathList;
		std::vector<std::string> _toRemovePerFrameUniformPathList;

	private:
		//BTree�� ����� ���� Factory.
		std::unique_ptr<BT::BehaviorTreeFactory> _factory;
		std::unique_ptr<Pg::Data::BTree::BTreeShareData> _bBoardSharedData;
	};
}


