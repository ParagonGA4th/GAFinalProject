#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "GUIDefinesAndStructs.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>

namespace Pg::DataScript
{
	class Stage2GUIHandler : public ScriptInterface<Stage2GUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2GUIHandler);

	public:
		Stage2GUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		//IGUI Handler.
		virtual void ResetToInitialState() override;

	private:
		//Obj Name / GUIAggregate
		std::unordered_map<std::string, GUIAggregate> _managedGuiObjectList;

	};
}

