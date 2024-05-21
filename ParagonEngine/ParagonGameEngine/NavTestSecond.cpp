#include "NavTestSecond.h"
#include "Navigation.h"
#include "DebugSystem.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <string>

namespace Pg::Engine
{
	NavTestSecond::NavTestSecond(Pg::Data::GameObject* owner) :
		Component(owner)
	{
		auto& tNavSystem = singleton<Pg::Engine::Navigation>();
		_navSystem = &tNavSystem;

		auto& tDebugSystem = singleton<Pg::Engine::DebugSystem>();
		_debugSystem = &tDebugSystem;
	}

	void NavTestSecond::Start()
	{

		//_navSystem->SetAgent(0, 2.f, 45.f, 2.f, 0.9f);
		_navSystem->SetAgent(2, 2.f, 45.f, 2.f, 0.9f);

		Move(1,{ _object->_transform._position.x, 0.f, _object->_transform._position.z }, { 0.0f, 0.0f, -10.0f });
		Move(2,{ _object->_transform._position.x +2.f, 0.f, _object->_transform._position.z }, { 3.0f, 0.0f, -10.0f });
		Move(3,{ _object->_transform._position.x +4.f, 0.f, _object->_transform._position.z }, { 5.0f, 0.0f, -10.0f });
		Move(4,{ _object->_transform._position.x -4.f, 0.f, _object->_transform._position.z }, { 10.0f, 0.0f, -10.0f });
		//Move2({ _object->_transform._position.x, 0.f, _object->_transform._position.z }, { 0.0f, 0.0f, -10.0f });

		//_navSystem->SyncAgent(0, _object->_transform._position);
	}

	void NavTestSecond::Update()
	{


		//_navSystem->FindStraightPath(0);
	}

	void NavTestSecond::Move(int index, Pg::Math::PGFLOAT3 startPos, Pg::Math::PGFLOAT3 targetPos)
	{
		DirectX::XMVECTOR currentPosVec = Pg::Math::PG2XM_FLOAT3_VECTOR(startPos);
		DirectX::XMVECTOR targetPosVec = Pg::Math::PG2XM_FLOAT3_VECTOR(targetPos);

		DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(currentPosVec, targetPosVec);

		dir = DirectX::XMVector3Normalize(dir);

		_navSystem->SetSEpos(index, { startPos.x, 0.f, startPos.z },
			{ targetPos.x + 0.5f * dir.m128_f32[0], 0.0f, targetPos.z + 0.5f * dir.m128_f32[2] });

		//_navSystem->SetSEpos(0, {_object->_transform._position.x, 0.f, _object->_transform._position.z},
		//	{ 0.0f, 0.0f, -10.0f });

		//메모리 새겠지만, 테스트를 위해!
		Pg::Data::NavCylinderInfo* tNavCyl = new Pg::Data::NavCylinderInfo(_navSystem->FindStraightPath(index));
		tNavCyl->_radius = 1.0f;
		tNavCyl->_height = 1.0f;
		_debugSystem->DrawNavCylinderDebug(tNavCyl);

		//for (auto& path : _straightpath)
		//{
		//	path.first.y = 0.0f;
		//	path.second.y = 0.0f;
		//	_debugSystem->DrawLineDebug(path.first, path.second, Pg::Math::PGFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		//}
		//PG_TRACE()
		//DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(dir, 1.f));


	}

	void NavTestSecond::Move2(Pg::Math::PGFLOAT3 startPos, Pg::Math::PGFLOAT3 targetPos)
	{
		DirectX::XMVECTOR currentPosVec = Pg::Math::PG2XM_FLOAT3_VECTOR(startPos);
		DirectX::XMVECTOR targetPosVec = Pg::Math::PG2XM_FLOAT3_VECTOR(targetPos);

		DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(currentPosVec, targetPosVec);

		dir = DirectX::XMVector3Normalize(dir);

		_navSystem->SetSEpos(2, { startPos.x, 0.f, startPos.z },
			{ targetPos.x + 0.5f * dir.m128_f32[0], 0.0f, targetPos.z + 0.5f * dir.m128_f32[2] });

		//_navSystem->SetSEpos(0, {_object->_transform._position.x, 0.f, _object->_transform._position.z},
		//	{ 0.0f, 0.0f, -10.0f });

		//메모리 새겠지만, 테스트를 위해!
		Pg::Data::NavCylinderInfo* tNavCyl2 = new Pg::Data::NavCylinderInfo(_navSystem->FindStraightPath(2));
		tNavCyl2->_radius = 1.0f;
		tNavCyl2->_height = 1.0f;
		_debugSystem->DrawNavCylinderDebug(tNavCyl2);

		//_straightpath = _navSystem->FindStraightPath(2);

		//for (auto& path : _straightpath)
		//{
		//	path.first.y = 0.0f;
		//	path.second.y = 0.0f;
		//	_debugSystem->DrawLineDebug(path.first, path.second, Pg::Math::PGFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
		//}
	}
}