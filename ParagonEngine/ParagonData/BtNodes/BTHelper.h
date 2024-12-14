#pragma once
#include <behaviortree_cpp_v3/bt_factory.h>

/// <summary>
/// BT �۵� �� ������ ��Ƴ� �� �ְ� �����ִ� ����.
/// </summary>

namespace Pg::Data::BTree
{
	class BTHelper
	{
	public:
		static void Validate(const BT::Result& tOpValue);
	};
}

#define BT_VALIDATE(tExpected) 						  				    \
{																	    \
	if (!tExpected)													    \
	{																    \
		assert(false && tExpected.error().c_str());						\
	}																    \
}													  
