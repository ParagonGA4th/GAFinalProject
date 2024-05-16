#pragma once
#include "../ParagonMath/PgMath.h"
#include <vector>

namespace Pg::Engine
{
	class Navigation;
}

namespace Pg::API::Recast
{
	class PgRecast
	{
	public:
		void Initialize();

		void SetSEpos(int index, Pg::Math::PGFLOAT3 startPosition, Pg::Math::PGFLOAT3 endPosition);

		// 직선경로 탐색 함수
		std::vector <std::pair<Pg::Math::PGFLOAT3, Pg::Math::PGFLOAT3>> FindStraightPath(int index);

		void SetAgent(int index, float agentHeight, float agentMaxSlope, float agentRadius, float agentMaxClimb);

	private:
		Pg::Engine::Navigation* _navSystem;
	};
}

