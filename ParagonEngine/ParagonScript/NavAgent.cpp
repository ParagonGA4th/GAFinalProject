#include "NavAgent.h"
#include "../ParagonAPI/PgRecast.h"

#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{

	NavAgent::NavAgent(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgRecast = &singleton<Pg::API::Recast::PgRecast>();
	}

	void NavAgent::Awake()
	{

	}

	void NavAgent::Start()
	{
		_pgRecast->SetAgent(0, 2.f, 45.f, 1.2f, 0.9f);
	}

	void NavAgent::Update()
	{
		_pgRecast->SetSEpos(0, { _object->_transform._position.x, 0.0f, _object->_transform._position.z }, { 0.0f, 0.0f, -10.0f });
		
		_pgRecast->FindStraightPath(0);
	}
}