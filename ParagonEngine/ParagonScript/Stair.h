#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class Stair : public ScriptInterface<Stair>
	{
		DEFINE_PARAGON_SCRIPT(Stair);

	public:
		Stair(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
	};
}


