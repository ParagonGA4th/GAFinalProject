#pragma once

#include "ScriptInterface.h"
#include "IAreaHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class BossSceneAreaHandler : public ScriptInterface<BossSceneAreaHandler>, public IAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(BossSceneAreaHandler);

	public:
		BossSceneAreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler
		virtual void ResetToInitialState() override;
	};
}


