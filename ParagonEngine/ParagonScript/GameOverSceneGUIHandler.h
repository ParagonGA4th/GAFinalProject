#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"

namespace Pg::API
{
	class PgScene;
	namespace Time { class PgTime; }
}

namespace Pg::DataScript
{
	class GameOverSceneGUIHandler : public ScriptInterface<GameOverSceneGUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(GameOverSceneGUIHandler);

		inline static const float STAY_IN_SCENE_TIME = 2.f;
	public:
		GameOverSceneGUIHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		virtual void AssignPointersToGUI() override;

	private:
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::PgScene* _pgScene{ nullptr };

		Pg::Data::ImageRenderer* _backgroundImage{ nullptr };
		Pg::Data::ImageRenderer* _overText{ nullptr };
		
		float _inputTime{ 0.f };
	};
}

