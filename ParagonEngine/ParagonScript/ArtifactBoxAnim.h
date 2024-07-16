#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::DataScript
{
	class ArtifactBox;
	class ArtifactBoxAnim : public ScriptInterface<ArtifactBoxAnim>
	{
		DEFINE_PARAGON_SCRIPT(ArtifactBoxAnim);

	public:
		ArtifactBoxAnim(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

	private:
		Pg::Data::SkinnedMeshRenderer* _renderer{ nullptr };
		ArtifactBox* _artiBox;

		bool _animEnd{ false };
	};
}