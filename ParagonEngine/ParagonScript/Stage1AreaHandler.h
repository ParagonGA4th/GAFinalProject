#pragma once

#include "ScriptInterface.h"
#include "IAreaHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <vector>

namespace Pg::DataScript
{
	class Stage1AreaHandler : public ScriptInterface<Stage1AreaHandler>, public IAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1AreaHandler);

	public:
		Stage1AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//Handler
		virtual void ResetToInitialState() override;
	};
}


