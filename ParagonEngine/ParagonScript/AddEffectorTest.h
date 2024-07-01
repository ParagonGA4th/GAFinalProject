#pragma once
#include "ScriptInterface.h"
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class Camera;
}

namespace Pg::API
{
	namespace Time
	{
		class PgTime;
	}
}

namespace Pg::DataScript
{
	class AddEffectorTest : public ScriptInterface<AddEffectorTest>
	{
		DEFINE_PARAGON_SCRIPT(AddEffectorTest);

	public:
		AddEffectorTest(Pg::Data::GameObject* obj);
		~AddEffectorTest();

	private:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
	};
}


