#pragma once

/// <summary>
/// �����ؼ� ���� �����ϰ� �ؾ� �ϴ� ��ũ��Ʈ ������ Ŭ�������� ���,
/// �̸� �ϰ������� ������ �� �ְ� ���ִ� Reset�� �������̽�.
/// </summary>

namespace Pg::DataScript
{
	class IScriptResettable
	{
	public:
		virtual void ResetAll() abstract;
	};
}
