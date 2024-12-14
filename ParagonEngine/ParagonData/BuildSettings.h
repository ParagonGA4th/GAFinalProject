#pragma once

namespace Pg::Data
{
	struct BuildSettings
	{
		//��ã�� ��ü���� �ִ� ��ü��
		int maxCrowdNumber{ 1024 };

		// ��ã�� ��ü���� �ִ� �浹�ݰ�
		float maxAgentRadius{ 0.6f };

		// ������ �ִ� ���
		float walkableSlopeAngle{ 30.f };

		// ���� �� �ִ� ����
		float walkableClimb{ 0.2f };

		// õ���� �ּ� ����
		float walkableHeight{ 0.3f };

		// x��,z�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
		float divisionSizeXZ{ 0.3f };

		// y�� ���� ������ ����, ������ �������� ������ �� ������������, �׺���̼� ����� ��ã�� �ý����� ���ϰ� �ð� �ȴ�.
		float divisionSizeY{ 0.2f };

		// ���� ������ xz������ 250*330, y������ 200�� ���� ���ҵǴ� ������ ���İ��� �׺���̼� ���嵵 �Ǹ鼭 ��ã�⵵ �������� �ϰ� �Ǵ� ������.
		// xz������ 743* 989�� ���� ������ �ǵ� ū ���ϴ� ����.
		float tileSize{ 48.f };
		float agentRadius{ 0.6f };
		float edgeMaxError{ 1.3f };
	};
}
