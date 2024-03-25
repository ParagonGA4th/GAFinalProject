#include "FactoryHelper.h"
#include "script_factory.h"

#include "Script.h"

#include "../ParagonData/GameObject.h"

void Pg::DataScript::FactoryHelper::AddScript(Pg::Data::GameObject* obj, std::string scriptType)
{
	Pg::DataScript::Script* script;
	script = Pg::Factory::Script::Factory<Pg::DataScript::Script, Pg::Data::GameObject*>::Create(scriptType.c_str(), obj);
	if (script != nullptr)
	{
		obj->AddComponent(scriptType, script);
	}
}
