#include "MonsterMove.h"
#include "../ParagonData/GameObject.h"
#include "Navigation.h"
#include <singleton-cpp/singleton.h>

MonsterMove::MonsterMove(Pg::Data::GameObject* obj) :
	Component(obj)
{

}

void MonsterMove::Start()
{
	auto& tNavSystem = singleton<Pg::Engine::Navigation>();
	_navSystem = &tNavSystem;
}

void MonsterMove::Update()
{

}

void MonsterMove::OnTriggerEnter(Pg::Data::Collider* c)
{

}

void MonsterMove::OnTriggerExit(Pg::Data::Collider* c)
{

}

bool MonsterMove::RotateToTarget(const DirectX::XMFLOAT3& targetPos)
{

}

bool MonsterMove::Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast)
{

}

float MonsterMove::CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition)
{
	auto angle = GetAngle(bossPosition, playerPosition);

	auto bossAngle = _object->_transform._position.y;

	// 현재보스의 각도와 계산한 플레이어와의 각도간의 차이가 180 이하일경우
	// angle만큼 그대로 움직이면 됨 (이렇게 안하면 90, 270도 중 270도로 돌아버려 비효율적)
	if (std::abs(bossAngle - angle) < 180.0f)
	{
		return angle;
	}
	// 각도간의 차이가 180도를 넘을경우
	else
	{
		// angle이 음수일 경우
		if (angle < 0)
		{
			return 360.0f + angle;
		}
		else
		{
			return angle - 360.0f;
		}
	}
}

float MonsterMove::GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst)
{
	DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&src);
	DirectX::XMVECTOR targetPosVec = DirectX::XMLoadFloat3(&dst);

	DirectX::XMVECTOR currentForward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR targetDirection = DirectX::XMVectorSubtract(targetPosVec, currentPosVec);
	targetDirection.m128_f32[1] = 0.0f;
	targetDirection = DirectX::XMVector3Normalize(targetDirection);

	// 두 벡터 간의 각도를 계산
	auto dotResult = DirectX::XMVector3Dot(currentForward, targetDirection);
	float dotProduct = DirectX::XMVectorGetX(dotResult);

	// 각도를 라디안에서 도로 변환
	float angle = acos(dotProduct);
	angle = DirectX::XMConvertToDegrees(angle);

	if (targetPosVec.m128_f32[0] > currentPosVec.m128_f32[0])
	{
		angle *= -1;
	}

	return angle;
}

float MonsterMove::GetDistance(DirectX::XMFLOAT3& src, DirectX::XMFLOAT3& dst)
{
	DirectX::XMVECTOR source = DirectX::XMLoadFloat3(&src);
	DirectX::XMVECTOR destination = DirectX::XMLoadFloat3(&dst);

	DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(destination, source);

	float result = DirectX::XMVectorGetX(DirectX::XMVector3Length(distance));

	return result;
}
