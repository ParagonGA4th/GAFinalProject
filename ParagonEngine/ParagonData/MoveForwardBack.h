#pragma once
#include "Script.h"
#include "../ParagonMath/PgMath.h"

#include <cstdlib>

/// <summary>
/// GameObject 내부 스크립트가 Graphics랑 연동해서 작동하는지 알아보기 위해.
/// </summary>

namespace Pg::Data
{
	class GameObject;
}
namespace Pg::Data
{
	class MoveForwardBack : public Script
	{
	public:
		MoveForwardBack(GameObject* obj);
		virtual void Update() override;

	private:
		int random1 = rand();
		int random2 = rand();
		float tMoveVar = (float)random1 / random2 * 2.0f;
	};

}

