#pragma once

//�����̴� ��� ������Ʈ���� ��ӹ޴� �������̽�
//�������� ������Ʈ�� �̸� �θ� Ŭ������ �޾ƾ� �Ѵ�.

namespace Pg::DataScript
{
	class IMovableObject
	{
	public:
		//��ġ�� ������, �ٸ� ���´� �������� ������� �ϴ� �Լ�.
		virtual void ResetAll() abstract;

		//�ڽ��� ���� ������ �ε��� - ��ȯ�ޱ�.
		//���� ��ü ���� �ٸ� ���� �����ؾ� �Ѵ�.
		virtual unsigned int GetDesignatedAreaIndex() abstract;
	};
}
