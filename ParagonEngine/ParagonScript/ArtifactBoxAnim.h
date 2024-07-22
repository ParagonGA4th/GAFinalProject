#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::DataScript
{
	class CombatSystem;
	class ArtifactBox;
	class ArtifactBoxAnim : public ScriptInterface<ArtifactBoxAnim>
	{
		DEFINE_PARAGON_SCRIPT(ArtifactBoxAnim);

	public:
		ArtifactBoxAnim(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Update() override;

		virtual void GrabManagedObjects() override;

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

	private:
		Pg::Data::SkinnedMeshRenderer* _renderer{ nullptr };
		ArtifactBox* _artiBox{ nullptr };
		CombatSystem* _combatSystem{ nullptr };

		bool _animEnd{ false };
	};
}