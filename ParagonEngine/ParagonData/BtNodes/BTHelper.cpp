#include "BTHelper.h"

namespace Pg::Data::BTree
{
	void BTHelper::Validate(const BT::Result& tOpValue)
	{
		if (!tOpValue)
		{
			throw BT::RuntimeError("Error: ", tOpValue.error());
		}
	}
}