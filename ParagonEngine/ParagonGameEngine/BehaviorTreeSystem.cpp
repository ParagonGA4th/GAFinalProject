#include "BehaviorTreeSystem.h"
#include "SceneSystem.h"

#include "../ParagonData/Animator.h"
#include "../ParagonUtil/Log.h"

#include <algorithm>

namespace Pg::Engine::BTree
{
	BehaviorTreeSystem::BehaviorTreeSystem()
	{
		_factory = std::make_unique<::BT::BehaviorTreeFactory>();
		_bBoardSharedData = std::make_unique<BTree::BTreeShareData>();
	}

	BehaviorTreeSystem::~BehaviorTreeSystem()
	{

	}

	void BehaviorTreeSystem::Initialize()
	{
		InitAllLeafNodes();
		LoadAllUniformXMLFiles();
		LoadAllInstancedXMLFiles();

		//SceneSystem에서 역시 SetCurrentScene에 연동해놓았으나, 별개로 Initialize.
		SyncSceneActiveBT();
	}

	void BehaviorTreeSystem::Update()
	{
		UpdateAnimators();
		ManageAnimatorList();
	}

	void BehaviorTreeSystem::InitAllLeafNodes()
	{
		//XML Node를 매칭해서 하는 것. 

	}

	void BehaviorTreeSystem::LoadAllUniformXMLFiles()
	{

	}

	void BehaviorTreeSystem::LoadAllInstancedXMLFiles()
	{

	}

	void BehaviorTreeSystem::SyncSceneActiveBT()
	{
		//Scene이 바뀌면 무조건 Active Uniform Animator List 클리어.
		if (!_activeUniformAnimatorList.empty())
		{
			_activeUniformAnimatorList.clear();
		}

		//전에 시행되었을 수도 있는 Active InstancedAnimatorList 삭제.
		if (!_activeInstancedAnimatorList.empty())
		{
			_activeInstancedAnimatorList.clear();
		}

		//SceneSystem 포인터 받기.
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;

		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::Animator* tAnimator = obj->GetComponent<Pg::Data::Animator>();

			if (tAnimator != nullptr)
			{
				if (tAnimator->_isUniform)
				{
					//Uniform에 대한 포인터를 ReadOnly로 받아 있는거다.
					auto tFound = _uniformTreeStorage.find(tAnimator->_behaviorTreePath);
					assert(tFound != _uniformTreeStorage.end() && "무조건 Uniform이 체크된 BehaviorTreePath는 미리 로드된 Uniform XML List 내부에 있어야!");

					//값 할당. 다만, READ-ONLY처럼 사용해야 하고 자동적으로 Animator가 동작하는 것이 아니다 (UNIFORM)
					tAnimator->_behavTree = tFound->second;
				}
				else
				{
					//Instantiated 되었다.

					//이미 저장된 InstancedTextStorage에서 Path가 있는지를 체크.
					auto tFound = _instancedTreePathContentStorage.find(tAnimator->_behaviorTreePath);
					assert(tFound != _instancedTreePathContentStorage.end() && "무조건 Instanced이 체크된 BehaviorTreePath는 미리 로드된 Instanced XML List 내부에 있어야!");

					//BehaviorTree 투입.
					*(tAnimator->_behavTree) = _factory->createTreeFromText(tFound->second.c_str(), BT::Blackboard::create());

					//자동으로 Blackboard사이 공유되는 자료 리스트 포인터 추가.
					auto blackboard = tAnimator->_behavTree->rootBlackboard();
					blackboard->set(BTree::BTreeShareData::KEY, _bBoardSharedData.get());

					//업데이트되는 리스트에서 추가.
					_activeInstancedAnimatorList.push_back(tAnimator);
				}
			}	
		}
	}

	void BehaviorTreeSystem::UpdateAnimators()
	{
		//Uniform List 업데이트.
		for (auto& it : _activeUniformAnimatorList)
		{
			//활성화.
			it.second->tickRoot();
		}

		//Instanced List 업데이트.
		for (auto& it : _activeInstancedAnimatorList)
		{
			//활성화되어 있을 때만 실행.
			if (it->GetActive())
			{	
				//활성화.
				it->_behavTree->tickRoot();
			}
		}

	}

	void BehaviorTreeSystem::ManageAnimatorList()
	{
		//Add
		{
			for (auto& path : _toAddPerFrameUniformPathList)
			{
				//Path가 이미 있는지를 체크. 
				auto tSearching = std::find_if(_activeUniformAnimatorList.begin(), _activeUniformAnimatorList.end(), [&path](std::pair<std::string, BT::Tree*>& val)
					{
						return val.first.compare(path) == 0;
					});
				if (tSearching != _activeUniformAnimatorList.end())
				{
					PG_WARN("이미 있는 Uniform Animator를 다시 추가할 수는 없다.");
					return;
				}

				auto tTree = _uniformTreeStorage.find(path);
				assert(tTree != _uniformTreeStorage.end() && "무조건 해당 Path를 찾았어야 한다. 유효하지 않은 Tree를 추가하려고 함.");
				_activeUniformAnimatorList.push_back(std::make_pair(path, tTree->second));
			}
		}

		//Remove
		{
			for (auto& path : _toRemovePerFrameUniformPathList)
			{
				//Path가 있으면 제거.
				_activeUniformAnimatorList.erase(std::remove_if(_activeUniformAnimatorList.begin(), _activeUniformAnimatorList.end(),
					[&path](std::pair<std::string, BT::Tree*>& val) { return val.first.compare(path) == 0; }), _activeUniformAnimatorList.end());
			}
		}

		//업데이트를 했으니 클리어.
		if (!_toAddPerFrameUniformPathList.empty())
		{
			_toAddPerFrameUniformPathList.clear();
		}

		if (!_toRemovePerFrameUniformPathList.empty())
		{
			_toRemovePerFrameUniformPathList.clear();
		}
	}

	void BehaviorTreeSystem::AddUniformRunningAnimator(const std::string& path)
	{
		_toAddPerFrameUniformPathList.push_back(path);
	}

	void BehaviorTreeSystem::RemoveUniformRunningAnimator(const std::string& path)
	{
		_toRemovePerFrameUniformPathList.push_back(path);
	}

	

}
