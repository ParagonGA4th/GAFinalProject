#pragma once
#include "ScriptInterface.h"
#include "BaseGUIHandler.h"
#include "IObserver.h"
#include "GUIDefinesAndStructs.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/ImageRenderer.h"

#include <unordered_map>

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
	namespace Time { class PgTime; }
}

namespace Pg::DataScript
{
	class PauseOptionBox;
	class BossBehaviour;

	class BossStageGUIHandler : public ScriptInterface<BossStageGUIHandler>, public BaseGUIHandler, public IObserver
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

		//핸들링 이벤트.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;
	private:
		void SetupStaminaBillboardRenderObject();
		void MatchUpdateStaminaToRO();
		void SetupBossHealthBar();
		void MatchUpdateBossHealthBar();
	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };

		PauseOptionBox* _pauseOptionBox{ nullptr };
		Pg::Data::VisualEffectRenderObject* _staminaBillboardObject{ nullptr };
		Pg::Data::Transform* _playerTransform{ nullptr };
		unsigned int* _staminaTextureIndexPointer{ nullptr };

		Pg::Data::ImageRenderer* _finalBossBar_Frame{ nullptr };
		Pg::Data::ImageRenderer* _finalBossBar_Fill{ nullptr };
		Pg::Data::ImageRenderer* _finalBossBar_Back{ nullptr };
		Pg::Data::ImageRenderer* _bossUI{ nullptr };
		
		BossBehaviour* _finalBossBehaviour{ nullptr };
		float dTime = 0.f;
	};
}

