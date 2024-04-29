#pragma once
#include <cassert>

///기존 C++의 Assert와는 달리, 
///Release에도 함수를 호출하되, 결과 Assert만 하지 않게 설정한 것. 

#ifdef NDEBUG
#define CustomAssert(expression) (static_cast<void>(expression))
#else
#define CustomAssert(expression) assert(expression)
#endif

