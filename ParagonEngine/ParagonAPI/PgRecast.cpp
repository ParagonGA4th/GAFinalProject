#include "PgRecast.h"
#include "../ParagonGameEngine/Navigation.h"
#include <singleton-cpp/singleton.h>

namespace Pg::API::Recast
{
	void PgRecast::Initialize()
	{
		auto& tNavSystem = singleton<Pg::Engine::Navigation>();
		_navSystem = &tNavSystem;
	}

	void PgRecast::SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition)
	{
		_navSystem->SetSEpos(index, startPosition, endPosition);
	}

	std::vector <std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> PgRecast::FindStraightPath(int index)
	{
		return _navSystem->FindStraightPath(index);
	}

	void PgRecast::SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb)
	{
		_navSystem->SetAgent(index, agentHeight, agentMaxSlope, agentRadius, agentMaxClimb);
	}
}