#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class PauseBox;
	//class CombatSystem;

	class Stage1GUIHandler : public ScriptInterface<Stage1GUIHandler>, public BaseGUIHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1GUIHandler);

	public:
		Stage1GUIHandler(Pg::Data::GameObject* obj);
		~Stage1GUIHandler();

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

		PauseBox* _pauseBox{ nullptr };
		Pg::Data::VisualEffectRenderObject* _staminaBillboardObject{ nullptr };
		Pg::Data::Transform* _playerTransform{ nullptr };
		unsigned int* _staminaTextureIndexPointer{ nullptr };
		//CombatSystem* _combatSystem{ nullptr };
	};
}

