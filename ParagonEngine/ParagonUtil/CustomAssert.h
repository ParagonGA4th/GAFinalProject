#pragma once
#include <cassert>

///���� C++�� Assert�ʹ� �޸�, 
///Release���� �Լ��� ȣ���ϵ�, ��� Assert�� ���� �ʰ� ������ ��. 

#ifdef NDEBUG
#define CustomAssert(expression) (static_cast<void>(expression))
#else
#define CustomAssert(expression) assert(expression)
#endif

