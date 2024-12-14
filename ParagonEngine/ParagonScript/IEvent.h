#pragma once
#include <string>

/// <summary>
/// 모든 개별적인 Event들의 베이스 클래스. 
/// </summary>
/// 

namespace Pg::DataScript
{
	class IEvent
	{
	public:
		virtual ~IEvent() {}

		//개별적으로 모든 이벤트들이 구현해야 하는 식별자 리턴 클래스.
		virtual const char* GetIdentifier() const abstract;
	};
}

//Event를 등록할 것이라면, 해당 매크로를 활용해서 이름을 등록해야!
#define DEFINE_EVENT_IDENTIFIER(_Identifier) \
public: \
static constexpr const char* _identifier{_Identifier}; \
virtual const char* GetIdentifier() const override { return _identifier; }