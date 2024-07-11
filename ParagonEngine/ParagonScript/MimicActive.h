#pragma once
#include "ScriptInterface.h"

#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class StaticBoxCollider;
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

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;
	public:
		BEGIN_VISITABLES(Pg::DataScript::MimicActive);
		VISITABLE(std::string, _mimicName);
		END_VISITABLES;

	private:
		Pg::Data::GameObject* _mimic;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::SkinnedMeshRenderer* _renderer;
		MimicBehaviour* _mimicBehaviour;
	};
}

