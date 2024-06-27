#pragma once

#include "ScriptInterface.h"
#include "BaseAreaHandler.h"
#include "MovingObjectDefinesAndStructs.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Pg::DataScript
{
	class Stage2AreaHandler : public ScriptInterface<Stage2AreaHandler>, public BaseAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2AreaHandler);

	public:
		//자신이 속한 Stage의 Area 전체 개수.
		enum { ENTIRE_AREA_COUNT = 4 };

	public:
		Stage2AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;
	};
}


