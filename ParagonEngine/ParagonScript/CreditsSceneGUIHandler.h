#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::API
{
	class PgScene;
	namespace Time { class PgTime; }
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class CreditsSceneGUIHandler : public ScriptInterface<CreditsSceneGUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(CreditsSceneGUIHandler);

	public:
		inline static const float STAY_IN_SCENE_TIME = 9.f;

	public:
		CreditsSceneGUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void CleanOnSceneChange() override;

		virtual void AssignPointersToGUI() override;

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics;
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::PgScene* _pgScene;
		Pg::Data::VisualEffectRenderObject* _vo;

		float _inputTime{ 0.f };

	};
}

