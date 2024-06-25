#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"



namespace Pg::DataScript
{
	class CreditsSceneGUIHandler : public ScriptInterface<CreditsSceneGUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(CreditsSceneGUIHandler);

	public:
		CreditsSceneGUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void AssignPointersToGUI() override;

	};
}

