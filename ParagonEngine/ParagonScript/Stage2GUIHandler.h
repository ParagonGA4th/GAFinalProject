#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "GUIDefinesAndStructs.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

#include <unordered_map>

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class PauseOptionBox;

	class Stage2GUIHandler : public ScriptInterface<Stage2GUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage2GUIHandler);

	public:
		Stage2GUIHandler(Pg::Data::GameObject* obj);
		~Stage2GUIHandler();

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void CleanOnSceneChange() override;

		virtual void AssignPointersToGUI() override;

		//얘만 독특하게 관리하게 할 때. 
		virtual void AdditionalReset() override;

	private:
		void SetupStaminaBillboardRenderObject();
		void MatchUpdateStaminaToRO();

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };

		PauseOptionBox* _pauseOptionBox{ nullptr };
		Pg::Data::VisualEffectRenderObject* _staminaBillboardObject{ nullptr };
		Pg::Data::Transform* _playerTransform{ nullptr };
		unsigned int* _staminaTextureIndexPointer{ nullptr };
	};
}

