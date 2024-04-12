#include "FactoryHelper.h"

#include "Script.h"
#include "ParagonScriptList.h"

#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	void FactoryHelper::AddScript(Pg::Data::GameObject* obj, const std::string& scriptType)
	{
		//명시적으로 include가 되어야 Registry가 반응한다.
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