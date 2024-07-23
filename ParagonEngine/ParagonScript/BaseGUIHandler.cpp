#include "BaseGUIHandler.h"
#include "TransformSimpleStorage.h"

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/Scene.h"

#include "PlayerHandler.h"

namespace Pg::DataScript
{
	void BaseGUIHandler::ResetToInitialState()
	{
		//다시 원 상태로 돌려줘야 한다.
		//개별 ResetAll 함수 내부는 필요하면 추가될 것이다.
		//일단은 그냥 보여주는 용도로 존재하는 애들도 있으니. 
		//원래는 SetActive같은 것들은 하부 가상함수에서 개별 조정할 계획이었으나, 
		//지금은 이렇게 관리.

		for (auto& [bObjName, bAgg] : _managedGuiObjectList)
		{
			//원래대로 Transform 바꿔주기.
			bAgg._transStorage.ToTransform(&(bAgg._guiObj->_transform));

			//Obj: SetActive 해주기.
			bAgg._guiObj->SetActive(bAgg._isObjActive);

			//Component : SetActive 해주기.
			//bAgg._rendererBase2D->SetActive(bAgg._isComponentActive);
		}
	}

	void BaseGUIHandler::GrabOrganizeAllGUI(Pg::Data::GameObject* obj)
	{
		Pg::Data::GameObject* _object = obj;

		// GUI Handler가 들고 와야 하는 오브젝트:
		// ScreenSpace에서 사용되는 모든 오브젝트들을 들고 있어야 한다.
		// ImageRenderer / TextRenderer 들중 하나라도 있는지를 검사해야 한다.
		// SetActive 검사 + Position 검사.

		for (auto& bObj : _object->GetScene()->GetObjectList())
		{
			Pg::Data::RendererBase2D* tIsImage = bObj->GetComponent<Pg::Data::ImageRenderer>();
			Pg::Data::RendererBase2D* tIsText = bObj->GetComponent<Pg::Data::TextRenderer>();

			if ((tIsImage != nullptr) || (tIsText != nullptr))
			{
				//이러면 처음에 자신이 살아있어야 하는지, 아닌지 등등이 SetActive로 지정되어 있어야 한다.
				//XML에서.
				GUIAggregate tToInsert;
				tToInsert._guiObj = bObj;
				tToInsert._isObjActive = bObj->GetActive();
				tToInsert._transStorage = TransformSimpleStorage(&(bObj->_transform));

				//단순히 GUI가 보여주기 위한 용도일 수도 있기에, 
				//ISingleGUI가 없어도 된다. NULLPTR 들어갈 수 있다는 말!
				tToInsert._guiComponent = bObj->GetComponent<ISingleGUI>();

				if (tIsImage != nullptr)
				{
					//Image라는 의미.
					tToInsert._rendererBase2D = tIsImage;
					//Component Active State 기록.
					tToInsert._isComponentActive = tToInsert._rendererBase2D->GetActive();
				}
				else
				{
					//Text라는 의미. 2D 두 개가 한꺼번에 공존할 수는 없으니.
					tToInsert._rendererBase2D = tIsText;
					//Component Active State 기록.
					tToInsert._isComponentActive = tToInsert._rendererBase2D->GetActive();
				}

				_managedGuiObjectList.insert(std::make_pair(bObj->GetName(), tToInsert));
			}
		}
	}

}
