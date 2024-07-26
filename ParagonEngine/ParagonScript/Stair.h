#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data 
{
	class StaticMeshRenderer;
	class SphereCollider;
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class StairHelper;
	class Stair : public ScriptInterface<Stair>
	{
		DEFINE_PARAGON_SCRIPT(Stair);

	public:
		Stair(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::DataScript::StairHelper* _helper{ nullptr };

		std::vector<Pg::Data::StaticMeshRenderer*> _renderers;
		Pg::Data::SphereCollider* _area{ nullptr };
		Pg::Data::ImageRenderer* _artiUI{ nullptr };

		inline static const float ALPHA_PERCENT = 1.f;
		inline static const float POSITION_PERCENT = 0.3f;

		int _index = 0;
		float _rollbackPos = 9.759f;
		bool _isGimickDone{ false };
	};
}


