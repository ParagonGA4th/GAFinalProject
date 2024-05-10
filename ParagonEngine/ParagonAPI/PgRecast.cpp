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

}