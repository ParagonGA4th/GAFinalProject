#include "BaseGUIHandler.h"
#include "TransformSimpleStorage.h"
#include "TotalGameManager.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/ImageRenderer.h"

#include "PlayerHandler.h"

namespace Pg::DataScript
{
	void BaseGUIHandler::ResetToInitialState()
	{
		//�ٽ� �� ���·� ������� �Ѵ�.
		//���� ResetAll �Լ� ���δ� �ʿ��ϸ� �߰��� ���̴�.
		//�ϴ��� �׳� �����ִ� �뵵�� �����ϴ� �ֵ鵵 ������. 
		//������ SetActive���� �͵��� �Ϻ� �����Լ����� ���� ������ ��ȹ�̾�����, 
		//������ �̷��� ����.

		for (auto& [bObjName, bAgg] : _managedGuiObjectList)
		{
			//������� Transform �ٲ��ֱ�.
			bAgg._transStorage.ToTransform(&(bAgg._guiObj->_transform));

			//Obj: SetActive ���ֱ�.
			bAgg._guiObj->SetActive(bAgg._isObjActive);

			//Component : SetActive ���ֱ�.
			//bAgg._rendererBase2D->SetActive(bAgg._isComponentActive);
		}
	}

	void BaseGUIHandler::GrabOrganizeAllGUI(Pg::Data::GameObject* obj)
	{
		Pg::Data::GameObject* _object = obj;

		// GUI Handler�� ��� �;� �ϴ� ������Ʈ:
		// ScreenSpace���� ���Ǵ� ��� ������Ʈ���� ��� �־�� �Ѵ�.
		// ImageRenderer / TextRenderer ���� �ϳ��� �ִ����� �˻��ؾ� �Ѵ�.
		// SetActive �˻� + Position �˻�.

		for (auto& bObj : _object->GetScene()->GetObjectList())
		{
			Pg::Data::RendererBase2D* tIsImage = bObj->GetComponent<Pg::Data::ImageRenderer>();
			Pg::Data::RendererBase2D* tIsText = bObj->GetComponent<Pg::Data::TextRenderer>();

			if ((tIsImage != nullptr) || (tIsText != nullptr))
			{
				//�̷��� ó���� �ڽ��� ����־�� �ϴ���, �ƴ��� ����� SetActive�� �����Ǿ� �־�� �Ѵ�.
				//XML����.
				GUIAggregate tToInsert;
				tToInsert._guiObj = bObj;
				tToInsert._isObjActive = bObj->GetActive();
				tToInsert._transStorage = TransformSimpleStorage(&(bObj->_transform));

				//�ܼ��� GUI�� �����ֱ� ���� �뵵�� ���� �ֱ⿡, 
				//ISingleGUI�� ��� �ȴ�. NULLPTR �� �� �ִٴ� ��!
				tToInsert._guiComponent = bObj->GetComponent<ISingleGUI>();

				if (tIsImage != nullptr)
				{
					//Image��� �ǹ�.
					tToInsert._rendererBase2D = tIsImage;
					//Component Active State ���.
					tToInsert._isComponentActive = tToInsert._rendererBase2D->GetActive();
				}
				else
				{
					//Text��� �ǹ�. 2D �� ���� �Ѳ����� ������ ���� ������.
					tToInsert._rendererBase2D = tIsText;
					//Component Active State ���.
					tToInsert._isComponentActive = tToInsert._rendererBase2D->GetActive();
				}

				_managedGuiObjectList.insert(std::make_pair(bObj->GetName(), tToInsert));
			}
		}
	}

	void BaseGUIHandler::SetTutorialExplainUI(Pg::Data::GameObject* obj)
	{
		Pg::Data::GameObject* _object = obj;

		Pg::Data::GameObject* _avoid = obj->GetScene()->FindObjectWithName("AvoidUI");
		Pg::Data::GameObject* _basicAttack = obj->GetScene()->FindObjectWithName("BasicAttackUI");
		Pg::Data::GameObject* _interaction = obj->GetScene()->FindObjectWithName("InteractionUI");
		Pg::Data::GameObject* _jump = obj->GetScene()->FindObjectWithName("JumpUI");
		Pg::Data::GameObject* _move = obj->GetScene()->FindObjectWithName("MoveUI");
		Pg::Data::GameObject* _strongAttack = obj->GetScene()->FindObjectWithName("StrongAttackUI");

		_avoidUI = _avoid->GetComponent<Pg::Data::ImageRenderer>();
		_basicAttackUI = _basicAttack->GetComponent<Pg::Data::ImageRenderer>();
		//_interactionUI = _interaction->GetComponent<Pg::Data::ImageRenderer>();
		_jumpUI = _jump->GetComponent<Pg::Data::ImageRenderer>();
		_MoveUI = _move->GetComponent<Pg::Data::ImageRenderer>();
		_strongAttackUI = _strongAttack->GetComponent<Pg::Data::ImageRenderer>();

		_avoidUI->SetActive(false);
		_avoidUI->SetActive(false);
		_basicAttackUI->SetActive(false);
		//_interactionUI->SetActive(false);
		_jumpUI->SetActive(false);
		_MoveUI->SetActive(false);
		_strongAttackUI->SetActive(false);

		if (obj->GetScene()->GetSceneName() != "BossStage")
		{
			Pg::Data::GameObject* _interactionKey = obj->GetScene()->FindObjectWithName("InteractionKeyUI");
			_interactionKeyUI = _interactionKey->GetComponent<Pg::Data::ImageRenderer>();
			_interactionKeyUI->SetActive(false);
		}
	}
	void BaseGUIHandler::GetLifeUIObjects(Pg::Data::GameObject* obj)
	{
		for (int i = 1; i <= 3; i++)
		{
			Pg::Data::GameObject* tObj = obj->GetScene()->FindObjectWithName(std::string("Artifact").append(std::to_string(i)));
			assert(tObj != nullptr);
			_heartArray.at(i - 1) = tObj->GetComponent<Pg::Data::ImageRenderer>();
			assert(_heartArray.at(i - 1) != nullptr);
		}

		auto tBundle = TotalGameManager::GetInstance(nullptr)->GetHandlerBundleByScene(obj->GetScene());
		assert(tBundle != nullptr);
		_playerLifePointer = &(tBundle->_playerBehavior->_playerlife);
	}

	void BaseGUIHandler::UpdateLife()
	{
		//0�� ������ / 1�� ������.
		int tHeartCount = *_playerLifePointer;

		_heartArray[0]->SetImageIndex(1);
		_heartArray[1]->SetImageIndex(1);
		_heartArray[2]->SetImageIndex(1);

		for (int i = 0; i < tHeartCount; i++)
		{
			//���Դٴ� ���� �����ٴ� ���� �ǹ�.
			_heartArray[i]->SetImageIndex(0);
		}
	}

}
