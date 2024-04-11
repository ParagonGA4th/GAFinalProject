#include "FactoryHelper.h"
#include "script_factory.h"

#include "Script.h"
#include "PlayerMove.h"

#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	//FactoryHelper::FactoryHelper()
	//{
	//	
	//}

	void FactoryHelper::AddScript(Pg::Data::GameObject* obj, const std::string& scriptType)
	{
		//Pg::DataScript::Script* script;
		//script = Pg::Factory::Script::ScriptFactory<Pg::DataScript::Script, Pg::Data::GameObject*>::Create(scriptType.c_str(), obj);

		//if (script == nullptr)
		//{
		//	if (scriptType.find("PlayerMove") != std::string::npos)
		//	{
		//		script = new PlayerMove(obj);
		//	}
		//}

		for (auto& iter : Script::get_registry()) 
		{
			if (scriptType.compare(iter.first) == 0)
			{
				obj->AddComponent(scriptType, iter.second(obj).release());
				return;
			}
		}

		assert(false && "못 찾았다.");
	}

	

}