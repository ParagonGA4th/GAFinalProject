#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "GUIDefinesAndStructs.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class PauseBox;

	class BossStageGUIHandler : public ScriptInterface<BossStageGUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(BossStageGUIHandler);
	public:
		BossStageGUIHandler(Pg::Data::GameObject* obj);
		~BossStageGUIHandler();

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void CleanOnSceneChange() override;

		//필수 구현.
		virtual void AssignPointersToGUI() override;

		//얘만 독특하게 관리하게 할 때. 
		virtual void AdditionalReset() override;

	private:
		void SetupStaminaBillboardRenderObject();
		void MatchUpdateStaminaToRO();

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };

		PauseBox* _pauseBox{ nullptr };
		Pg::Data::VisualEffectRenderObject* _staminaBillboardObject{ nullptr };
		Pg::Data::Transform* _playerTransform{ nullptr };
		unsigned int* _staminaTextureIndexPointer{ nullptr };
	};
}

