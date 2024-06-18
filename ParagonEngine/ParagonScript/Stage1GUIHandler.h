#pragma once
#include "ScriptInterface.h"
#include "IGUIHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class Stage1GUIHandler : public ScriptInterface<Stage1GUIHandler>, public IGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1GUIHandler);

	public:
		Stage1GUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//IGUI Handler.
		virtual void ResetToInitialState() override;

	};
}

