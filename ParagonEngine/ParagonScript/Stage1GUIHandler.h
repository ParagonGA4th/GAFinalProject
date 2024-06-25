#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"

namespace Pg::DataScript
{
	class Stage1GUIHandler : public ScriptInterface<Stage1GUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1GUIHandler);

	public:
		Stage1GUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void AssignPointersToGUI() override;
	};
}

