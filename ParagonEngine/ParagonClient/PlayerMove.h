#pragma once
#include "../ParagonData/Script.h"

/// <summary>
/// 플레이어 관련 스크립트
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}

class PlayerMove : public Pg::Data::Script
{
public:
	PlayerMove(Pg::Data::GameObject* obj);

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::Data::DynamicCollider* dynamicCol;
	Pg::API::Input::PgInput* _pgInput = nullptr;
};

