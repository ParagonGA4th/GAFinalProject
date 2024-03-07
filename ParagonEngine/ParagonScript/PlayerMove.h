#pragma once
#include "../ParagonData/Script.h"
#include <auto_register/factory.h>

namespace Pg::Data
{
	class GameObject;
	class DynamicCollider;
}

namespace Pg::API::Input
{
	class PgInput;
}

/// <summary>
/// 플레이어 관련 스크립트
/// </summary>

class PlayerMove : public Pg::Data::Script, RegisteredInFactory<Pg::Data::Script, PlayerMove, Pg::Data::GameObject*>
{
public:
	PlayerMove(Pg::Data::GameObject* obj);

	static Script* CreateInstance(Pg::Data::GameObject* go) { return new PlayerMove(go); }
	static const char* GetFactoryKey() { return "class PlayerMove"; }

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::Data::DynamicCollider* dynamicCol;
	Pg::API::Input::PgInput* _pgInput = nullptr;
};

