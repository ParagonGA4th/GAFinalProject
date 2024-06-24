#include "AreaPassingTrigger.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/LayerMask.h"

namespace Pg::DataScript
{
	AreaPassingTrigger::AreaPassingTrigger(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void AreaPassingTrigger::BeforePhysicsAwake()
	{
		//이때는 Property를 바꾸지는 않고,
		//assert로 체크 정도만 할 수 있게 설정.
	}

	void AreaPassingTrigger::Awake()
	{

	}

	void AreaPassingTrigger::Update()
	{

	}

	void AreaPassingTrigger::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		// OnTriggerEnter가 실행되었을 때 ->
		// Player인지 확인해서 검사해야 한다.
		for (int i = 0; i < count; i++)
		{
			//Player일 때, Area Handler한테 전달.
			Pg::Data::Collider* tCol = _colArr[i];
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
			{
				//자신의 Area Index에 도달했다는 것을 알려준다.
				_areaHandler->SetCurrentAreaIndex(_areaIndex);
			}
		}
	}

	void AreaPassingTrigger::StoreDesignatedAreaHandler(IAreaHandler* areaHandler)
	{
		//후에 함수 호출을 위해 마련.
		_areaHandler = areaHandler;
	}

}