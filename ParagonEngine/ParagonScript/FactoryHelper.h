#pragma once
#include <string>

namespace Pg::Data { class GameObject; }

namespace Pg::DataScript
{
	class FactoryHelper
	{
	public:
		static void AddScript(Pg::Data::GameObject* obj, const std::string& scriptType);
	};
}

