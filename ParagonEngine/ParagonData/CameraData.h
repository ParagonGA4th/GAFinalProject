#pragma once
#include "../ParagonMath/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib, "..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

//��ȯ����� �� ������
namespace Pg::Data
{
	struct CameraData
	{
	// �������� ä�� ī�޶� ������
	public:
		// ī�޶��� �̵��� ȸ��
		Pg::Math::PGFLOAT3 _position;
		Pg::Math::PGQuaternion _rotation;

		// ī�޶� ����
		float _nearZ;
		float _farZ;
		float _fovY;
		Pg::Math::PGFLOAT4X4 _viewMatrix;
		
	// �׷��Ƚ����� ä�� ī�޶� ������ 
	public: 
		Pg::Math::PGFLOAT4X4 _projMatrix;
		float _aspect;
		float _nearWindowHeight;
		float _farWindowHeight;

	public:
		//�÷��̾��� ��ġ�� ��ũ��Ʈ���� ����� �� �ְ�!
		Pg::Math::PGFLOAT3 _playerPosition{ 0.f,0.f,0.f };

		//Pg::Math::PGFLOAT4X4 _mainLightPerspectiveViewProjMatrix;
	};
}
