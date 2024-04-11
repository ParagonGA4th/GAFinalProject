#include "FactoryHelper.h"
#include "script_factory.h"

#include "Script.h"
#include "PlayerMove.h"

#include "../ParagonData/GameObject.h"

void Pg::DataScript::FactoryHelper::AddScript(Pg::Data::GameObject* obj, std::string scriptType)
{
	Pg::DataScript::Script* script;
	script = Pg::Factory::Script::ScriptFactory<Pg::DataScript::Script, Pg::Data::GameObject*>::Create(scriptType.c_str(), obj);
	
	//if (script == nullptr)
	//{
	//	if (scriptType.find("PlayerMove") != std::string::npos)
	//	{
	//		script = new PlayerMove(obj);
	//	}
	//}
	if(script != nullptr)
		obj->AddComponent(scriptType, script);
}
