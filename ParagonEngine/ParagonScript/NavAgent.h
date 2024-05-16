#pragma once
#include "ScriptInterface.h"

namespace Pg::API
{
	namespace Recast
	{
		class PgRecast;
	}
}

namespace Pg::DataScript
{
	class NavAgent : public ScriptInterface<NavAgent>
	{
		DEFINE_PARAGON_SCRIPT(NavAgent);

	public:
		NavAgent(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::API::Recast::PgRecast* _pgRecast = nullptr;
	};
}

