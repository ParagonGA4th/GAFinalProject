#pragma once

#include "../ParagonData/BtNodes/BTreeShareData.h"

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include <behaviortree_cpp_v3/bt_factory.h>

/// <summary>
/// Behavior Tree System. 게임 내부의 Behavior Tree들 및 업데이트를 관리한다.
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

		//Scene이 바뀌었을 때 (+ Initialize) 때 호출되는 함수. BT 리스트 업데이트.
		void SyncSceneActiveBT();

		//내용 업데이트.
		void Update();

		//만약 Uniform한 Animator인 경우, 객체별로 존재하지 않고, Component가 Object 하위에 있다고 해서 동작하지 않는다.
		//AddUniformRunningAnimator()를 호출해서 (API) -> Update 루프에 포함시키고,
		//RemoveUniformRunningAnimator()를 호출해서 Update 루프에서 제거한다.
		//Instanced는 Component에 존재하면 Update 루프에 포함된다.
		void AddUniformRunningAnimator(const std::string& path);
		void RemoveUniformRunningAnimator(const std::string& path);

	public:
		//EngineMain에서 호출되어야 함. 
		//Scene이 바뀌면 Animator 싹 다 죽여줘야 함.
		//void OnSceneChange(Pg::Data::Scene* changedScene);

	private:
		//해당 Location들에

		//모든 Leaf Node들은 내부적으로 이 함수에서 Register되어야 한다.
		void InitAllLeafNodes();
		//모든 XML 파일들 BehaviorTreeSystem에 맞게 로드.
		void LoadAllXMLFiles(const std::string& resourceListPath);

		//XML (Uniform) 파일 하나 BT::Tree로 로드.
		void LoadSingleUniformXMLFile(const std::string& path);
		//XML (Instanced) 파일 하나 미리 TEXT를 받아오기. 
		void LoadSingleInstancedXMLFile(const std::string& path);

		//Update 내부에 호출되는 함수들.
		void UpdateAnimators();
		void ManageAnimatorList();


	private:
		//SceneSystem 싱글턴 보관.
		Pg::Engine::SceneSystem* _sceneSystem = nullptr;

		//자체적인 ComponentList 관리.
		//Blackboard를 굳이 Share할 필요가 없다. 
		//변수를 나누어서 개별적인 Instance에서 상응되는 Variable을 사용할 수 있게 하기 위해.
		std::vector<Pg::Data::Animator*> _activeInstancedAnimatorList;
		std::vector<std::pair<std::string, BT::Tree*>> _activeUniformAnimatorList;
		
		//Storage
		//자체적인 (Uniform) BT::Tree 관리. List. 현재로서는 ResourceManager의 역할까지 한꺼번에 수행하고 있는 것. 
		//Tree 객체 자체를 보관한다. (주소만 할당해주는 방식이 되어야 한다 개별 Animator한테는)
		std::unordered_map<std::string, BT::Tree> _uniformTreeStorage;

		//Instanced Animator Component의 경우, 
		//그럼에도 List 보관 이유 -> 해당 File Location에 Tree가 반드시 있을 수 있게 보장하는 것.
		// -> 미리 Text를 읽어와서 런타임에 Filesystem 활용하는 경우가 없게 하기 위해 Change.
		std::unordered_map<std::string, std::string> _instancedTreePathContentStorage;

		//Scope Per Frame
		//한 프레임 내부에 추가 / 삭제를 위해 별개로 보관하는 String Path List.
		std::vector<std::string> _toAddPerFrameUniformPathList;
		std::vector<std::string> _toRemovePerFrameUniformPathList;

	private:
		//BTree를 만들어 내는 Factory.
		std::unique_ptr<BT::BehaviorTreeFactory> _factory;
		std::unique_ptr<Pg::Data::BTree::BTreeShareData> _bBoardSharedData;
	};
}


