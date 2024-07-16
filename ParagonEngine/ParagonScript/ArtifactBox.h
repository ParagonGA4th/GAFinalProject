#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::API
{
	namespace Input { class PgInput; }
}

namespace Pg::DataScript
{
	class ArtifactBox : public ScriptInterface<ArtifactBox>
	{
		DEFINE_PARAGON_SCRIPT(ArtifactBox);

	public:
		ArtifactBox(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

	private:
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _renderer{ nullptr };

		bool _onTriggerStay{ false };
		bool _animEnd{ false };
	};
}