#pragma once
#include "ScriptInterface.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::API
{
	namespace Input { class PgInput; }
}

namespace Pg::Data
{
	class StaticBoxCollider;
	class AudioSource;
	class SkinnedMeshRenderer;
}

namespace Pg::DataScript
{
	class MimicBehaviour;

	class MimicActive : public ScriptInterface<MimicActive>
	{
		DEFINE_PARAGON_SCRIPT(MimicActive);
	public:
		MimicActive(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;
	public:
		BEGIN_VISITABLES(Pg::DataScript::MimicActive);
		VISITABLE(std::string, _mimicName);
		END_VISITABLES;

	private:
		Pg::Data::GameObject* _mimic;
		Pg::Data::AudioSource* _boxOpenSound;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::SkinnedMeshRenderer* _renderer;
		MimicBehaviour* _mimicBehaviour;

		Pg::Math::PGFLOAT3 _rendererOffset = { 0.f, -0.5f, 0.f };

		Pg::API::Input::PgInput* _pgInput{ nullptr };
		bool _onTriggerStay{ false };
	};
}

