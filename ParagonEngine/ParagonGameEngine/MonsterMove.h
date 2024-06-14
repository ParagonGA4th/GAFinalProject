#pragma once
#include "../ParagonData/Component.h"

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>

namespace Pg::Engine
{
	class Navigation;
	class SceneSystem;
}

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class MonsterHelper;
	class StaticBoxCollider;
}

class MonsterMove : public Pg::Data::Component
{
public:

	MonsterMove(Pg::Data::GameObject* obj);

	virtual void BeforePhysicsAwake() override;
	virtual void Start() override;
	virtual void Update() override;
	//virtual void OnCollisionEnter(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override;
	virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	//플레이어를 쫓는 함수
	void Chase();

	//대쉬
	void Dash();

	//타겟의 위치로 이동
	bool MoveToTarget(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed);
	// 플레이어 바라보도록 회전시키기
	bool RotateToTarget(const DirectX::XMFLOAT3& targetPos);

	bool LookAtPlayer(float angle, float rotateSpeed);

	void RotateToPlayer(Pg::Math::PGFLOAT3& targetPos);

	// 특정 포지션으로 이동시키기 (raycast true시 무언가에 막히면 거기까지만 찾아감)
	void UpdateMove();

	//목표 지점과의 각도를 계산.
	float CalculateAngle(const DirectX::XMFLOAT3& bossPosition, const DirectX::XMFLOAT3& playerPosition);

	// 두 XMFLOAT3 좌표 사잇각
	float GetAngle(DirectX::XMFLOAT3 src, DirectX::XMFLOAT3 dst);

	// 두 XMFLOAT3 좌표 사이의 거리
	float GetDistance(DirectX::XMFLOAT3& src, DirectX::XMFLOAT3& dst);

private:
	Pg::Engine::Navigation* _navSystem = nullptr;
	Pg::Util::Time::TimeSystem* _timeSystem = nullptr;
	Pg::Engine::SceneSystem* _sceneSystem = nullptr;

	std::vector<std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> _straightPath;

	float _moveSpeed;
	float _dashSpeed;
	float _distance;

	float _attackRange;
	float _sightRange;

	//몬스터의 상태
	bool _isStart;
	bool _isHit;
	bool _isRotateFinish;

	//대쉬 관련 변수.
	bool _isDash;			//돌진 여부
	bool _hasDashed;		//돌진했는지 여부
	float _dashRange;		//돌진 거리
	float _dashDuration;	//돌진 지속 시간
	float _currentDashTime; //현재 돌진 시간

	bool _isMoving;
	bool _isRotate;

	DirectX::XMFLOAT3 _prevPos;
	DirectX::XMFLOAT3 _backStepPos;

	Pg::Data::GameObject* _player;
	Pg::Data::Transform* _playerTransform;
	Pg::Data::MonsterHelper* _monsterHelper;

	std::vector<Pg::Data::StaticBoxCollider*> _attackCol;

protected:
	//MonsterStatus _status;

};

