#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>

namespace Pg::Engine
{
	class Navigation;
}

class MonsterMove : public Pg::Data::Component
{
public:

	MonsterMove(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(Pg::Data::Collider* c);
	virtual void OnTriggerExit(Pg::Data::Collider* c);

	// 플레이어 바라보도록 회전시키기
	bool RotateToTarget(const DirectX::XMFLOAT3& targetPos);

	// 특정 포지션으로 이동시키기 (raycast true시 무언가에 막히면 거기까지만 찾아감)
	bool Move(DirectX::XMFLOAT3& targetPos, float speed, bool roateToTarget, bool rayCast);

	//목표 지점과의 각도를 계산.
	float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

	// 두 XMFLOAT3 좌표 사잇각
	float GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

	// 두 XMFLOAT3 좌표 사이의 거리
	float GetDistance(DirectX::XMFLOAT3& src, DirectX::XMFLOAT3& dst);
private:
	Pg::Engine::Navigation* _navSystem = nullptr;

};

