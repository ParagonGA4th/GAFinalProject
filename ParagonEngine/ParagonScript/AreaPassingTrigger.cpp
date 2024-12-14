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
		//�̶��� Property�� �ٲ����� �ʰ�,
		//assert�� üũ ������ �� �� �ְ� ����.
	}

	void AreaPassingTrigger::Awake()
	{

	}

	void AreaPassingTrigger::Update()
	{

	}

	void AreaPassingTrigger::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		// OnTriggerEnter�� ����Ǿ��� �� ->
		// Player���� Ȯ���ؼ� �˻��ؾ� �Ѵ�.
		for (int i = 0; i < count; i++)
		{
			//Player�� ��, Area Handler���� ����.
			Pg::Data::Collider* tCol = _colArr[i];
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_PLAYER)
			{
				//�ڽ��� Area Index�� �����ߴٴ� ���� �˷��ش�.
				_areaHandler->SetCurrentAreaIndex(_areaIndex);
			}
		}
	}

	void AreaPassingTrigger::StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler)
	{
		//�Ŀ� �Լ� ȣ���� ���� ����.
		_areaHandler = areaHandler;
	}

}