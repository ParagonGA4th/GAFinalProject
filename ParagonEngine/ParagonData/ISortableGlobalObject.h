#pragma once

//Don't Destroy On Load�� �ǵ��� ������ �°� ������Ʈ�ǰ� �ϱ� ����,
//Sorting�ǰ� �� ��.
//��ũ�� �������, ������ ������ ��������.
//�ٸ� ������� �ǰ� �ϱ� ����.
//�����ϱ� ���ؼ�, Manager���� 1 ������Ʈ �� 1 ������Ʈ�� ���·� ���� �Ѵ�.

namespace Pg::Data
{
	class ISortableGlobalObject
	{
	public:
		//0�� ���� ���� �;� �Ѵ�. ������ ���� �ʰ� Update ������ Ȯ������ ����.
		virtual unsigned int GetPriorityIndex() abstract;
	};
}


