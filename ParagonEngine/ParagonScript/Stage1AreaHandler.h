#pragma once
#include "ScriptInterface.h"
#include "BaseAreaHandler.h"

namespace Pg::DataScript
{
	class Stage1AreaHandler : public ScriptInterface<Stage1AreaHandler>, public BaseAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1AreaHandler);

	public:
		//자신이 속한 Stage의 Area 전체 개수.
		enum {ENTIRE_AREA_COUNT = 3};

	public:
		Stage1AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

	};
}


