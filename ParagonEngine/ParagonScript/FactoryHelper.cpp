#include "FactoryHelper.h"
#include "Script.h"
#include "ParagonScriptList.h"
#include "../ParagonData/GameObject.h"
#include <cassert>

namespace Pg::DataScript
{
	void FactoryHelper::Initialize()
	{
		//'>' ����, XML ���ο��� ������ �ʴ� character �����.
		for (auto& [typeName, createFunc] : Script::get_registry())
		{
			std::string tChangedString = typeName.substr(0, typeName.find(">"));
	
			if (typeName.empty())
			{
				assert(false && "CTTI�� �߸� �ԷµǾ���!");
			}

			//Ű ����, XML�� ���� �� ���� ���ڸ� �����ϰ� �ٲ۴�.
			auto nodeHandler = Script::get_registry().extract(typeName);	
			nodeHandler.key() = tChangedString;
			Script::get_registry().insert(std::move(nodeHandler));
		}

		//PG_TRACE(std::to_string(Script::get_registry().size()).append(" Scripts Initialized."));
	}

	void FactoryHelper::AddScript(Pg::Data::GameObject* obj, const std::string& scriptType)
	{
		//��������� include�� �Ǿ�� Registry�� �����Ѵ�.
		for (auto& iter : Script::get_registry()) 
		{
			if (scriptType.compare(iter.first) == 0)
			{
				obj->AddComponent(scriptType, iter.second(obj));
				return;
			}
		}

		assert(false && "�� ã�Ҵ�.");
	}
}