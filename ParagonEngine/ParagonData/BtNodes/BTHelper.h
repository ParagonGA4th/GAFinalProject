#pragma once
#include <behaviortree_cpp_v3/bt_factory.h>

/// <summary>
/// BT 작동 중 오류를 잡아낼 수 있게 도와주는 헬퍼.
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
