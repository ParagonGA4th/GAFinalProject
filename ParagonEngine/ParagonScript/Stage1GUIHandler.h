#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "IObserver.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonData/ImageRenderer.h"

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class PauseOptionBox;
	//class CombatSystem;
	class GolemBossBehaviour;

	class Stage1GUIHandler : public ScriptInterface<Stage1GUIHandler>, public BaseGUIHandler, public IObserver
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

		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

	private:
		void SetupStaminaBillboardRenderObject();
		void SetupGolemBossHealthBar();
		void MatchUpdateStaminaToRO();
		void MatchUpdateBossHealthBar();

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };

		PauseOptionBox* _pauseOptionBox{ nullptr };
		Pg::Data::VisualEffectRenderObject* _staminaBillboardObject{ nullptr };
		Pg::Data::Transform* _playerTransform{ nullptr };
		unsigned int* _staminaTextureIndexPointer{ nullptr };
		//CombatSystem* _combatSystem{ nullptr };

		Pg::Data::ImageRenderer* _golemBossBar_Frame{ nullptr };
		Pg::Data::ImageRenderer* _golemBossBar_Fill{ nullptr };
		Pg::Data::ImageRenderer* _golemBossBar_Back{ nullptr };

		GolemBossBehaviour* _golemBossBehaviour{ nullptr };

		bool _isGolemBossMet = false;
	};
}

