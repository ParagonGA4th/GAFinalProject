#include "BehaviorTreeSystem.h"
#include "SceneSystem.h"
#include "../ParagonData/BtNodes/BTDefines.h"
//#include "../ParagonData/BtNodes/BasePgBtNode.h"
#include "../ParagonData/BtNodes/PgCustomBTNodes.h"
#include "../ParagonData/BtNodes/BTTemplateSpecialization.h"

#include "../ParagonData/Animator.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonUtil/CustomAssert.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonHelper/CSVHelper.h"
#include <fstream>
#include <algorithm>
#include <sstream>

namespace Pg::Engine::BTree
{
	BehaviorTreeSystem::BehaviorTreeSystem()
	{
		_factory = std::make_unique<::BT::BehaviorTreeFactory>();
		_bBoardSharedData = std::make_unique<Pg::Data::BTree::BTreeShareData>();
	}

	BehaviorTreeSystem::~BehaviorTreeSystem()
	{

	}

	void BehaviorTreeSystem::Initialize(const std::string& resourceListPath)
	{
		InitAllLeafNodes();
		LoadAllXMLFiles(resourceListPath);

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
		//일일히 만든 컨디션 노가다가 필요하다

#pragma region ACTUAL_NODE_REGISTRATION

		using namespace Pg::Data::BTree;
		_factory->registerNodeType<Node::Anim_Delay>("Anim_Delay");

		_factory->registerNodeType<Node::isDead>("isDead");
		_factory->registerNodeType<Node::isChase>("isChase");
		_factory->registerNodeType<Node::isHitSpace>("isHitSpace");
		_factory->registerNodeType<Node::isPlayerInSight>("isPlayerInSight");
		_factory->registerNodeType<Node::isDistanceClose>("isDistanceClose");		
		
		_factory->registerNodeType<Node::Anim_bDead>("Anim_bDead");
		_factory->registerNodeType<Node::Anim_bIdle>("Anim_bIdle");
		_factory->registerNodeType<Node::Anim_bWalk>("Anim_bWalk");
		_factory->registerNodeType<Node::Anim_bDash>("Anim_bDash");
		_factory->registerNodeType<Node::Anim_Cast>("Anim_Cast");
		_factory->registerNodeType<Node::Anim_RightArmAttack>("Anim_RightArmAttack");
		_factory->registerNodeType<Node::Anim_LeftArmAttack>("Anim_LeftArmAttack");
		_factory->registerNodeType<Node::Anim_TwoArmAttack>("Anim_TwoArmAttack");
		_factory->registerNodeType<Node::Anim_FeatherAttackStart>("Anim_FeatherAttackStart");
		_factory->registerNodeType<Node::Anim_FeatherAttack>("Anim_FeatherAttack");
		_factory->registerNodeType<Node::Anim_FeatherAttackEnd>("Anim_FeatherAttackEnd");
		_factory->registerNodeType<Node::Anim_FlyAttack>("Anim_FlyAttack");
		_factory->registerNodeType<Node::Anim_FlyAttackPrePare>("Anim_FlyAttackPrePare");
		_factory->registerNodeType<Node::Anim_Evasion>("Anim_Evasion");
		_factory->registerNodeType<Node::Anim_DownStart>("Anim_DownStart");
		_factory->registerNodeType<Node::Anim_Down>("Anim_Down");
		_factory->registerNodeType<Node::Anim_DownEnd>("Anim_DownEnd");
		_factory->registerNodeType<Node::isPase_1>("isPase_1");
		_factory->registerNodeType<Node::isPase_2>("isPase_2");
		_factory->registerNodeType<Node::isPase_3>("isPase_3");
		_factory->registerNodeType<Node::isDownStart>("isDownStart");
		_factory->registerNodeType<Node::isDown>("isDown");
		_factory->registerNodeType<Node::isDownEnd>("isDownEnd");
		_factory->registerNodeType<Node::isbDash>("isbDash");

		_factory->registerNodeType<Node::Anim_gbLeftArmAttack>("Anim_gbLeftArmAttack");
		_factory->registerNodeType<Node::Anim_gbRightArmAttack>("Anim_gbRightArmAttack");
		_factory->registerNodeType<Node::Anim_gbTwoArmAttack>("Anim_gbTwoArmAttack");
		_factory->registerNodeType<Node::Anim_SpinAttack>("Anim_SpinAttack");
		_factory->registerNodeType<Node::Anim_gDown>("Anim_gDown");
		_factory->registerNodeType<Node::isgPase_1>("isgPase_1");
		_factory->registerNodeType<Node::isgPase_2>("isgPase_2");
		_factory->registerNodeType<Node::isgSkill>("isgSkill");
		_factory->registerNodeType<Node::isgDown>("isgDown");

		_factory->registerNodeType<Node::Anim_fDead>("Anim_fDead");
		_factory->registerNodeType<Node::Anim_fIdle>("Anim_fIdle");
		_factory->registerNodeType<Node::Anim_fWalk>("Anim_fWalk");

		_factory->registerNodeType<Node::Anim_gDead>("Anim_gDead");
		_factory->registerNodeType<Node::Anim_gIdle>("Anim_gIdle");

		_factory->registerNodeType<Node::Anim_Dash>("Anim_Dash");
		_factory->registerNodeType<Node::Anim_gAttack>("Anim_gAttack");
		_factory->registerNodeType<Node::isDash>("isDash");

		_factory->registerNodeType<Node::Anim_Shoot>("Anim_Shoot");
		_factory->registerNodeType<Node::Toggle>("Toggle");

		_factory->registerNodeType<Node::Anim_tAttack>("Anim_tAttack");
		_factory->registerNodeType<Node::Anim_PoisonGas>("Anim_PoisonGas");
		_factory->registerNodeType<Node::isCoolDown>("isCoolDown");

		_factory->registerNodeType<Node::Anim_mAttack>("Anim_mAttack");
		_factory->registerNodeType<Node::Anim_mChase>("Anim_mChase");
		_factory->registerNodeType<Node::Anim_ThrowCoin>("Anim_ThrowCoin");


#pragma endregion ACTUAL_NODE_REGISTRATION

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
					tAnimator->_behavTree = &(tFound->second);
				}
				else
				{
					//Instantiated 되었다.

					//이미 저장된 InstancedTextStorage에서 Path가 있는지를 체크.
					auto tFound = _instancedTreePathContentStorage.find(tAnimator->_behaviorTreePath);
					assert(tFound != _instancedTreePathContentStorage.end() && "무조건 Instanced이 체크된 BehaviorTreePath는 미리 로드된 Instanced XML List 내부에 있어야!");

					//BehaviorTree 투입. (이게 맞다)
					*(tAnimator->_behavTree) = _factory->createTreeFromText(tFound->second.c_str(), BT::Blackboard::create());
					//*(tAnimator->_behavTree) = _factory->createTreeFromFile(tAnimator->_behaviorTreePath, BT::Blackboard::create());

					//자동으로 Blackboard사이 공유되는 자료 리스트 포인터 추가.
					//Blackboard를 두고 자체적으로 공유하는 데이터 + 개별적 소속 오브젝트의 경우 Object 포인터 자체를 기록.
					//자체 속한 GameObject의 포인터를 내부적으로 저장할 수 있게. (Instanced만 가능) + Shared Data
					//V4.5+에는 자체적으로 ApplyVisitor가 있지만, 지금 이 자체로는 (재귀 없는 거 아님) 라이브러리 구현체 기반으로 Iterate해야.
					for (auto& itt : tAnimator->_behavTree->nodes)
					{
						::BT::TreeNode* tPlainNode = itt.get();
						if (auto it = dynamic_cast<Pg::Data::BTree::Node::BasePgBtNode*>(tPlainNode))
						{
							it->InitializeTreeNode(obj, _bBoardSharedData.get());
							it->InitCustom();
						}
					}

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
				_activeUniformAnimatorList.push_back(std::make_pair(path, &(tTree->second)));
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

	void BehaviorTreeSystem::LoadAllXMLFiles(const std::string& resourceListPath)
	{
		std::string tUniformPath = Pg::Util::Helper::ResourceHelper::ForcePathUniformFull(resourceListPath);
		std::string tPath = tUniformPath + "/10_BehaviorTree.csv";

		auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromBTreeCSV(tPath);
		for (auto& it : tPathVec)
		{
			CustomAssert(Pg::Util::Helper::ResourceHelper::IsFileExist(it.first) && "무조건 존재하는 파일만 로드해야!");

			if (it.second)
			{
				//isUniform.
				LoadSingleUniformXMLFile(it.first);
			}
			else
			{
				LoadSingleInstancedXMLFile(it.first);
			}
		}		

		assert("");
	}
	
	void BehaviorTreeSystem::LoadSingleUniformXMLFile(const std::string& path)
	{
		//이미 해당 XML 경로 자체는 무조건 존재하고, Tree를 만들어야 한다.
		_uniformTreeStorage.insert(std::make_pair(path, _factory->createTreeFromFile(path, BT::Blackboard::create())));

		//자동으로 Blackboard사이 공유되는 자료 리스트 포인터 추가.
		//V4.5+에는 자체적으로 ApplyVisitor가 있지만, 지금 이 자체로는 (재귀 없는 거 아님) 라이브러리 구현체 기반으로 Iterate해야.
		for (auto& itt : _uniformTreeStorage.at(path).nodes)
		{
			::BT::TreeNode* tPlainNode = itt.get();
			if (auto it = dynamic_cast<Pg::Data::BTree::Node::BasePgBtNode*>(tPlainNode))
			{
				//uniform이니, 자신이 "소속된" GameObject는 없다.
				it->InitializeTreeNode(nullptr, _bBoardSharedData.get());
				it->InitCustom();
			}
		}
	}

	void BehaviorTreeSystem::LoadSingleInstancedXMLFile(const std::string& path)
	{
		//파일 자체 파싱.
		std::ifstream file(path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string xmlString = buffer.str();

		//Path / XML 버퍼 순서대로 집어넣는다.
		_instancedTreePathContentStorage.insert(std::make_pair(path, buffer.str()));
	}

	////Scene이 바뀌었을 때, 더 이상 별개의 업데이트 없애야 / 바뀌어야.
	//void BehaviorTreeSystem::OnSceneChange(Pg::Data::Scene* changedScene)
	//{
	//
	//}

}
