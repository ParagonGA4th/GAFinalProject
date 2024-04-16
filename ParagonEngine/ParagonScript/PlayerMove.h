#pragma once
#include "ScriptInterface.h"

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
namespace Pg::DataScript
{
	class PlayerMove : public ScriptInterface<PlayerMove>				//, Pg::Factory::Script::RegisteredInFactory<Pg::DataScript::Script, PlayerMove, Pg::Data::GameObject*>
	{
		DEFINE_PARAGON_SCRIPT(PlayerMove);

	public:
		PlayerMove(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::DynamicCollider* dynamicCol;
		Pg::API::Input::PgInput* _pgInput = nullptr;
	};

	//const bool PlayerMove::registered_ = ScriptInterface<PlayerMove>::register_type();
}


