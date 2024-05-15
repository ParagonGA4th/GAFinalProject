#pragma once
#include "../ParagonMath/PgMath.h"

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

	private:
		Pg::Engine::Navigation* _navSystem;
	};
}

