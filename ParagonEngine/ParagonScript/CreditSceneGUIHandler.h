#pragma once
#include "ScriptInterface.h"
#include "IGUIHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class CreditSceneGUIHandler : public ScriptInterface<CreditSceneGUIHandler>, public IGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(CreditSceneGUIHandler);

	public:
		CreditSceneGUIHandler(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//IGUI Handler.
		virtual void ResetToInitialState() override;

	};
}

