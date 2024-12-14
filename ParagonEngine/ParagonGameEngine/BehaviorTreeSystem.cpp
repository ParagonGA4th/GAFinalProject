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

		//SceneSystem���� ���� SetCurrentScene�� �����س�������, ������ Initialize.
		SyncSceneActiveBT();
	}

	void BehaviorTreeSystem::Update()
	{
		UpdateAnimators();
		ManageAnimatorList();
	}

	void BehaviorTreeSystem::InitAllLeafNodes()
	{
		//XML Node�� ��Ī�ؼ� �ϴ� ��. 
		//������ ���� ����� �밡�ٰ� �ʿ��ϴ�

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

		_factory->registerNodeType<Node::Anim_gbTwoArmAttack>("Anim_gbTwoArmAttack");
		_factory->registerNodeType<Node::Anim_SpinAttack>("Anim_SpinAttack");
		_factory->registerNodeType<Node::Anim_SpinDashAttack>("Anim_SpinDashAttack");
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
		//Scene�� �ٲ�� ������ Active Uniform Animator List Ŭ����.
		if (!_activeUniformAnimatorList.empty())
		{
			_activeUniformAnimatorList.clear();
		}

		//���� ����Ǿ��� ���� �ִ� Active InstancedAnimatorList ����.
		if (!_activeInstancedAnimatorList.empty())
		{
			_activeInstancedAnimatorList.clear();
		}

		//SceneSystem ������ �ޱ�.
		auto& tSceneSystem = singleton<SceneSystem>();
		_sceneSystem = &tSceneSystem;
		
		for (auto& obj : _sceneSystem->GetCurrentScene()->GetObjectList())
		{
			Pg::Data::Animator* tAnimator = obj->GetComponent<Pg::Data::Animator>();

			if (tAnimator != nullptr)
			{
				if (tAnimator->_isUniform)
				{
					//Uniform�� ���� �����͸� ReadOnly�� �޾� �ִ°Ŵ�.
					auto tFound = _uniformTreeStorage.find(tAnimator->_behaviorTreePath);
					assert(tFound != _uniformTreeStorage.end() && "������ Uniform�� üũ�� BehaviorTreePath�� �̸� �ε�� Uniform XML List ���ο� �־��!");

					//�� �Ҵ�. �ٸ�, READ-ONLYó�� ����ؾ� �ϰ� �ڵ������� Animator�� �����ϴ� ���� �ƴϴ� (UNIFORM)
					tAnimator->_behavTree = &(tFound->second);
				}
				else
				{
					//Instantiated �Ǿ���.

					//�̹� ����� InstancedTextStorage���� Path�� �ִ����� üũ.
					auto tFound = _instancedTreePathContentStorage.find(tAnimator->_behaviorTreePath);
					assert(tFound != _instancedTreePathContentStorage.end() && "������ Instanced�� üũ�� BehaviorTreePath�� �̸� �ε�� Instanced XML List ���ο� �־��!");

					//BehaviorTree ����. (�̰� �´�)
					*(tAnimator->_behavTree) = _factory->createTreeFromText(tFound->second.c_str(), BT::Blackboard::create());
					//*(tAnimator->_behavTree) = _factory->createTreeFromFile(tAnimator->_behaviorTreePath, BT::Blackboard::create());

					//�ڵ����� Blackboard���� �����Ǵ� �ڷ� ����Ʈ ������ �߰�.
					//Blackboard�� �ΰ� ��ü������ �����ϴ� ������ + ������ �Ҽ� ������Ʈ�� ��� Object ������ ��ü�� ���.
					//��ü ���� GameObject�� �����͸� ���������� ������ �� �ְ�. (Instanced�� ����) + Shared Data
					//V4.5+���� ��ü������ ApplyVisitor�� ������, ���� �� ��ü�δ� (��� ���� �� �ƴ�) ���̺귯�� ����ü ������� Iterate�ؾ�.
					for (auto& itt : tAnimator->_behavTree->nodes)
					{
						::BT::TreeNode* tPlainNode = itt.get();
						if (auto it = dynamic_cast<Pg::Data::BTree::Node::BasePgBtNode*>(tPlainNode))
						{
							it->InitializeTreeNode(obj, _bBoardSharedData.get());
							it->InitCustom();
						}
					}

					//������Ʈ�Ǵ� ����Ʈ���� �߰�.
					_activeInstancedAnimatorList.push_back(tAnimator);
				}
			}	
		}
	}

	void BehaviorTreeSystem::UpdateAnimators()
	{
		//Uniform List ������Ʈ.
		for (auto& it : _activeUniformAnimatorList)
		{
			//Ȱ��ȭ.
			it.second->tickRoot();
		}

		//Instanced List ������Ʈ.
		for (auto& it : _activeInstancedAnimatorList)
		{
			//Ȱ��ȭ�Ǿ� ���� ���� ����.
			if (it->GetActive())
			{	
				//Ȱ��ȭ.
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
				//Path�� �̹� �ִ����� üũ. 
				auto tSearching = std::find_if(_activeUniformAnimatorList.begin(), _activeUniformAnimatorList.end(), [&path](std::pair<std::string, BT::Tree*>& val)
					{
						return val.first.compare(path) == 0;
					});
				if (tSearching != _activeUniformAnimatorList.end())
				{
					PG_WARN("�̹� �ִ� Uniform Animator�� �ٽ� �߰��� ���� ����.");
					return;
				}

				auto tTree = _uniformTreeStorage.find(path);
				assert(tTree != _uniformTreeStorage.end() && "������ �ش� Path�� ã�Ҿ�� �Ѵ�. ��ȿ���� ���� Tree�� �߰��Ϸ��� ��.");
				_activeUniformAnimatorList.push_back(std::make_pair(path, &(tTree->second)));
			}
		}

		//Remove
		{
			for (auto& path : _toRemovePerFrameUniformPathList)
			{
				//Path�� ������ ����.
				_activeUniformAnimatorList.erase(std::remove_if(_activeUniformAnimatorList.begin(), _activeUniformAnimatorList.end(),
					[&path](std::pair<std::string, BT::Tree*>& val) { return val.first.compare(path) == 0; }), _activeUniformAnimatorList.end());
			}
		}

		//������Ʈ�� ������ Ŭ����.
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
			CustomAssert(Pg::Util::Helper::ResourceHelper::IsFileExist(it.first) && "������ �����ϴ� ���ϸ� �ε��ؾ�!");

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
		//�̹� �ش� XML ��� ��ü�� ������ �����ϰ�, Tree�� ������ �Ѵ�.
		_uniformTreeStorage.insert(std::make_pair(path, _factory->createTreeFromFile(path, BT::Blackboard::create())));

		//�ڵ����� Blackboard���� �����Ǵ� �ڷ� ����Ʈ ������ �߰�.
		//V4.5+���� ��ü������ ApplyVisitor�� ������, ���� �� ��ü�δ� (��� ���� �� �ƴ�) ���̺귯�� ����ü ������� Iterate�ؾ�.
		for (auto& itt : _uniformTreeStorage.at(path).nodes)
		{
			::BT::TreeNode* tPlainNode = itt.get();
			if (auto it = dynamic_cast<Pg::Data::BTree::Node::BasePgBtNode*>(tPlainNode))
			{
				//uniform�̴�, �ڽ��� "�Ҽӵ�" GameObject�� ����.
				it->InitializeTreeNode(nullptr, _bBoardSharedData.get());
				it->InitCustom();
			}
		}
	}

	void BehaviorTreeSystem::LoadSingleInstancedXMLFile(const std::string& path)
	{
		//���� ��ü �Ľ�.
		std::ifstream file(path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string xmlString = buffer.str();

		//Path / XML ���� ������� ����ִ´�.
		_instancedTreePathContentStorage.insert(std::make_pair(path, buffer.str()));
	}

	////Scene�� �ٲ���� ��, �� �̻� ������ ������Ʈ ���־� / �ٲ���.
	//void BehaviorTreeSystem::OnSceneChange(Pg::Data::Scene* changedScene)
	//{
	//
	//}

}
