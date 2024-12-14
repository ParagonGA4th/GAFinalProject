#pragma once
#include <string>

/// <summary>
/// ��� �������� Event���� ���̽� Ŭ����. 
/// </summary>
/// 

namespace Pg::DataScript
{
	class IEvent
	{
	public:
		virtual ~IEvent() {}

		//���������� ��� �̺�Ʈ���� �����ؾ� �ϴ� �ĺ��� ���� Ŭ����.
		virtual const char* GetIdentifier() const abstract;
	};
}

//Event�� ����� ���̶��, �ش� ��ũ�θ� Ȱ���ؼ� �̸��� ����ؾ�!
#define DEFINE_EVENT_IDENTIFIER(_Identifier) \
public: \
static constexpr const char* _identifier{_Identifier}; \
virtual const char* GetIdentifier() const override { return _identifier; }