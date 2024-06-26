#pragma once
#include <string>
#include <vector>

namespace Pg::Data { class GameObject; }

namespace Pg::DataScript
{
	class FactoryHelper
	{
	public:
		static void Initialize();
		static void AddScript(Pg::Data::GameObject* obj, const std::string& scriptType);
	};
}

