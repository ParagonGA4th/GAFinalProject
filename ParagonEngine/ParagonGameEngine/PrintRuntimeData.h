#pragma once
#include "../ParagonData/Script.h"
#include "../ParagonMath/PgMath.h"

#include <cstdlib>

/// <summary>
/// 런타임에 발생하는 하드웨어 정보를 출력하기 위한 스크립트.
/// </summary>

namespace Pg::Data
{
	class GameObject;
	class TextRenderer;
}

namespace Pg::Engine
{
	namespace Time
	{
		class TimeSystem;
	}
}

class PrintRuntimeData : public Pg::Data::Script
{
public:
	PrintRuntimeData(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::Engine::Time::TimeSystem* _timeSystem = nullptr;
	Pg::Data::TextRenderer* _textRenderer = nullptr;
};





