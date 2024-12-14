#include "PrintRuntimeData.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/TextRenderer.h"

#include "../ParagonUtil/TimeSystem.h"

#include <singleton-cpp/singleton.h>
#include <string>

PrintRuntimeData::PrintRuntimeData(Pg::Data::GameObject* obj) : Pg::Data::Component(obj)
{
	auto& tPgTime = singleton<Pg::Util::Time::TimeSystem>();
	this->_timeSystem = &tPgTime;
}

void PrintRuntimeData::Start()
{
	_textRenderer = _object->GetComponent<Pg::Data::TextRenderer>();
	assert(_textRenderer != nullptr);
}

void PrintRuntimeData::Update()
{
	std::string tTextFPS = "FPS : ";
	std::string tActualFPS = std::to_string(_timeSystem->GetFrameRate());
	tTextFPS = tTextFPS + tActualFPS + '\n';

	_textRenderer->SetString(tTextFPS);
}
