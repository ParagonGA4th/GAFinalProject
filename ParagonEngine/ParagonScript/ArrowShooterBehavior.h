#pragma once
#include "ScriptInterface.h"

#include <vector>

namespace Pg::DataScript
{
	class ArrowLogic;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	};
}

namespace Pg::DataScript
{
	class ArrowShooterBehavior : public ScriptInterface<ArrowShooterBehavior>
	{
		DEFINE_PARAGON_SCRIPT(ArrowShooterBehavior);

	public:
		ArrowShooterBehavior(Pg::Data::GameObject* obj);

		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::API::Input::PgInput* _pgInput;
		std::vector<ArrowLogic*> _arrowVec;
	};
}



