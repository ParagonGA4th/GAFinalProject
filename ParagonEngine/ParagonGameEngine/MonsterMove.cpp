#include "MonsterMove.h"
#include "SceneSystem.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonMath/PgMath.h"
#include "Navigation.h"
#include "MonsterStrucr.h"
#include <singleton-cpp/singleton.h>

MonsterMove::MonsterMove(Pg::Data::GameObject* obj) :
	Component(obj), _isRotateFinish(false)
{

}

void MonsterMove::Start()
{
	auto& tNavSystem = singleton<Pg::Engine::Navigation>();
	_navSystem = &tNavSystem;

	auto& tTimeSystem = singleton<Pg::Util::Time::TimeSystem>();
	_timeSystem = &tTimeSystem;

	auto& tSceneSystem = singleton<Pg::Engine::SceneSystem>();
	_sceneSystem = &tSceneSystem;

	//플레이어 지정
	_player = _sceneSystem->GetCurrentScene()->FindObjectWithName("Player");
	_playerTransform = _player->GetComponent<Pg::Data::Transform>();
}

void MonsterMove::Update()
{
	//Chase();

	RotateToPlayer(_playerTransform->_position);
}

void MonsterMove::Chase()
{
	auto bossPosition = _object->_transform._position;

	auto playerPosition = _playerTransform->_position;

	// 패턴의 사거리를 받아옴
	if (!_isRotateFinish)
	{
		// 보스와 플레이어 사이의 각도를 측정
		auto angle = CalculateAngle({ bossPosition.x, bossPosition.y, bossPosition.z }, 
			{ playerPosition.x, playerPosition.y, playerPosition.z });

		// 보스가 플레이어를 바라보게 함
		_isRotateFinish = LookAtPlayer(angle, 0.8f);
	}
}

bool MonsterMove::MoveToTarget(DirectX::XMFLOAT3& startPos, DirectX::XMFLOAT3& targetPos, float speed)
{
	float moveSpeed = _timeSystem->GetDeltaTime();

	// 목적지까지 거리 계산
	auto dist = GetDistance(startPos, targetPos);

	if (dist > 0.5f)
	{
		// 플레이어 이동
		DirectX::XMVECTOR currentPosVec = DirectX::XMLoadFloat3(&startPos);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), currentPosVec);

		direction = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(currentPosVec, DirectX::XMVectorScale(direction, moveSpeed));
		_object->_transform._position = { newPosition.m128_f32[0], 0.0f, newPosition.m128_f32[2] };

		_prevPos.x = _object->_transform._position.x;
		_prevPos.y = _object->_transform._position.y;
		_prevPos.z = _object->_transform._position.z;

		return true;
	}

	else
	{
		return false;
	}
}

bool MonsterMove::RotateToTarget(const DirectX::XMFLOAT3& targetPos)
{
	return true;
}

bool MonsterMove::LookAtPlayer(float angle, float rotateSpeed)
{
	// 회전 속도
	float speed = _timeSystem->GetDeltaTime() * rotateSpeed;

	// 에러 범위
	float errorRange = speed * 2.0f;

	// 현재 각도가 목표로 하는 각도보다 작을 경우
	if (_object->_transform._rotation.y < angle)
	{
		// 회전 속도만큼 회전
		_object->_transform._rotation.x = _object->_transform._rotation.x;
		_object->_transform._rotation.y = _object->_transform._rotation.y + speed;
		_object->_transform._rotation.z = _object->_transform._rotation.z;
		//_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y + speed, _bossTransform->GetRotation().z);
	}
	// 현재 각도가 목표로 하는 각도보다 클 경우
	else
	{
		// 회전 속도만큼 회전
		_object->_transform._rotation.x = _object->_transform._rotation.x;
		_object->_transform._rotation.y = _object->_transform._rotation.y - speed;
		_object->_transform._rotation.z = _object->_transform._rotation.z;
		//_bossTransform->SetRotation(_bossTransform->GetRotation().x, _bossTransform->GetRotation().y - speed, _bossTransform->GetRotation().z);
	}

	// 현재의 각도가 목표로 하는 각도의 오차범위 내에 있을 경우
	if (angle - errorRange <= _object->_transform._rotation.y && _object->_transform._rotation.y <= angle + errorRange)
	{
		// 회전이 완료되었다고 반환
		return true;
	}
	else
	{
		return false;
	}
}

void MonsterMove::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
{
	Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
	tRotBasePos.y = _object->_transform._position.y;

	Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

	//정규화.
	Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

	Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

	//회전이 끝날 때 까지 돌기.
	if (!_isRotateFinish)
	{
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}
}

void MonsterMove::UpdateMove()
{
	//auto tempVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&_object->_transform._position), DirectX::XMLoadFloat3(&playerPos));
	//auto dirVec = DirectX::XMVector3Normalize(tempVec);
	//auto range = _nowPattern->_range + _nowPattern->_rangeOffset;

	//_navSystem->SetSEpos(1, _object->_transform._position.x, 0.0f, _object->_transform._position..z,
	//	playerPos.x + range * dirVec.m128_f32[0], 0.0f, playerPos.z + range * dirVec.m128_f32[2]);

	_straightPath = _navSystem->FindStraightPath(0);

	for (auto& path : _straightPath)
	{
		path.first.y = 0.0f;
		path.second.y = 0.0f;
	}
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
