#include "DeathPlane.h"

#include "../ParagonData/Collider.h"
#include "../ParagonData/LayerMask.h"

namespace Pg::DataScript
{
	DeathPlane::DeathPlane(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void DeathPlane::BeforePhysicsAwake()
	{
		//이때는 Property를 바꾸지는 않고,
		//assert로 체크 정도만 할 수 있게 설정.
	}

	void DeathPlane::Awake()
	{
		
	}

	void DeathPlane::Update()
	{

	}

	void DeathPlane::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		// OnTriggerEnter가 실행되었을 때 ->
		// Player인지 확인해서 검사해야 한다.
		for (int i = 0; i < count; i++)
		{
			//Player일 때, Area Handler한테 전달.
			Pg::Data::Collider* tCol = _colArr[i];
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
			{
				//Just Hit Death Plane.
				_areaHandler->OnPlayerHitDeathPlane();
			}

			// 그 다음에, Death Plane 한정으로 다른 적들이 떨어질 가능성이 있을 경우, 
			// 이 역시 대비해야 할 것이다.
		}
	}

	void DeathPlane::StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler)
	{
		//후에 함수 호출을 위해 마련.
		_areaHandler = areaHandler;
	}

}