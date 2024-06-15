#include "FactoryHelper.h"
#include "Script.h"
#include "ParagonScriptList.h"
#include "../ParagonData/GameObject.h"
#include <cassert>

namespace Pg::DataScript
{
	void FactoryHelper::Initialize()
	{
		//'>' 같은, XML 내부에서 허용되지 않는 character 지우기.
		for (auto& [typeName, createFunc] : Script::get_registry())
		{
			std::string tChangedString = typeName.substr(0, typeName.find(">"));
	
			if (typeName.empty())
			{
				assert(false && "CTTI가 잘못 입력되었다!");
			}

			//키 값을, XML이 담을 수 없는 문자를 누락하게 바꾼다.
			auto nodeHandler = Script::get_registry().extract(typeName);	
			nodeHandler.key() = tChangedString;
			Script::get_registry().insert(std::move(nodeHandler));
		}

		//PG_TRACE(std::to_string(Script::get_registry().size()).append(" Scripts Initialized."));
	}

	void FactoryHelper::AddScript(Pg::Data::GameObject* obj, const std::string& scriptType)
	{
		//명시적으로 include가 되어야 Registry가 반응한다.
		for (auto& iter : Script::get_registry()) 
		{
			if (scriptType.compare(iter.first) == 0)
			{
				obj->AddComponent(scriptType, iter.second(obj));
				return;
			}
		}

		assert(false && "못 찾았다.");
	}
}