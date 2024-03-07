#pragma once
#include "ParagonScript.h"

/// <summary>
/// 플레이어 관련 스크립트
/// </summary>

class PlayerMove : public Pg::Data::Script, RegisteredInFactory<Pg::Data::Script, PlayerMove, Pg::Data::GameObject*>
{
public:
	PlayerMove(GameObject* obj);

	static Script* CreateInstance(GameObject* go) { return new PlayerMove(go); }
	static const char* GetFactoryKey() { return "class PlayerMove"; }

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::Data::DynamicCollider* dynamicCol;
	PgInput* _pgInput = nullptr;
};

