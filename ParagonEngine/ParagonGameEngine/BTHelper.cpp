#include "BTHelper.h"

namespace Pg::Engine::BTree
{
	void BTHelper::Validate(const BT::Result& tOpValue)
	{
		if (!tOpValue)
		{
			throw BT::RuntimeError("Error: ", tOpValue.error());
		}
	}
}