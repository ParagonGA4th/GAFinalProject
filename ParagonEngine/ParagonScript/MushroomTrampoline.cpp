#include "MushroomTrampoline.h"

#include "../ParagonData/scene.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonMath/PgMath.h"

#include "../ParagonUtil/Log.h"

namespace Pg::DataScript
{
	MushroomTrampoline::MushroomTrampoline(Pg::Data::GameObject* obj)
		: ScriptInterface(obj)
	{
	}

	void MushroomTrampoline::BeforePhysicsAwake()
	{
		auto playerObj = _object->GetScene()->FindObjectWithName("Player");
		_playerCap = playerObj->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_playerCap != nullptr);
	}

	void MushroomTrampoline::Awake()
	{
	}

	void MushroomTrampoline::Update()
	{

	}

	void MushroomTrampoline::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		for (int i = 0; i < count; i++)
		{
			Pg::Data::Collider* col = _colArr[i];

			if (col->_object->GetTag() == "TAG_Player")
			{
				_playerCap->AddForce(Pg::Math::PGFLOAT3::GlobalUp() * _jumpPower, Pg::Data::ForceMode::eIMPULSE);
				PG_TRACE("TRIGGER!!");
			}
		}
	}
}