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
		//�̶��� Property�� �ٲ����� �ʰ�,
		//assert�� üũ ������ �� �� �ְ� ����.
	}

	void DeathPlane::Awake()
	{
		
	}

	void DeathPlane::Update()
	{

	}

	void DeathPlane::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		// OnTriggerEnter�� ����Ǿ��� �� ->
		// Player���� Ȯ���ؼ� �˻��ؾ� �Ѵ�.
		for (int i = 0; i < count; i++)
		{
			//Player�� ��, Area Handler���� ����.
			Pg::Data::Collider* tCol = _colArr[i];
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
			{
				//Just Hit Death Plane.
				_areaHandler->OnPlayerHitDeathPlane();
			}

			// �� ������, Death Plane �������� �ٸ� ������ ������ ���ɼ��� ���� ���, 
			// �� ���� ����ؾ� �� ���̴�.
		}
	}

	void DeathPlane::StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler)
	{
		//�Ŀ� �Լ� ȣ���� ���� ����.
		_areaHandler = areaHandler;
	}

}