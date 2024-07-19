#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
	class StaticBoxCollider;
	class Collider;
}

namespace Pg::DataScript
{
	class Grating : public ScriptInterface<Grating>
	{
		DEFINE_PARAGON_SCRIPT(Grating);

	public:
		Grating(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	public:
		BEGIN_VISITABLES(Pg::DataScript::Grating);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	private:
		Pg::Data::StaticMeshRenderer* _renderer;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::Data::Collider* _otherCol;

		inline static const float ALPHA_PERCENT = 1.0f;
	};
}


