#pragma once
#include "ScriptInterface.h"

namespace Input
{
	class PgInput;
}

//실제로 Perlin Noise를 클라이언트에서 (Shaking) 활용한다.
class FastNoiseLite;

namespace Pg::DataScript
{
	class CameraShake : public ScriptInterface<CameraShake>
	{
		DEFINE_PARAGON_SCRIPT(CameraShake);
	public:
		CameraShake()

	};
}
