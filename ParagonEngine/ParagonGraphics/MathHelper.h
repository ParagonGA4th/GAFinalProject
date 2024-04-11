#pragma once
#include "../ParagonMath/PgMath.h"
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

/// <summary>
/// 그래픽스 내부에서 수학 처리 등을 위한 헬퍼 클래스.
/// </summary>

namespace Pg::Graphics::Helper
{
	class MathHelper
	{
	public:
		static DirectX::XMFLOAT2 PG2XM_FLOAT2(Pg::Math::PGFLOAT2 value);
		static DirectX::XMFLOAT3 PG2XM_FLOAT3(Pg::Math::PGFLOAT3 value);
		static DirectX::XMFLOAT4 PG2XM_FLOAT4(Pg::Math::PGFLOAT4 value);
		static DirectX::XMVECTOR PG2XM_VECTOR(Pg::Math::PGFLOAT4 value);
		static DirectX::XMFLOAT4 PG2XM_QUATERNION(Pg::Math::PGQuaternion value);
		static DirectX::XMFLOAT3X3 PG2XM_FLOAT3X3(Pg::Math::PGFLOAT3X3 value);
		static DirectX::XMFLOAT4X4 PG2XM_FLOAT4X4(Pg::Math::PGFLOAT4X4 value);
		static DirectX::XMMATRIX PG2XM_MATRIX(Pg::Math::PGFLOAT4X4 value);

		static bool IsEqualXMFloat3X3(DirectX::XMFLOAT3X3 first, DirectX::XMFLOAT3X3 second);
		static bool IsEqualXMFloat4X4(DirectX::XMFLOAT4X4 first, DirectX::XMFLOAT4X4 second);

		//Assimp의 Matrix를 SimpleMath로 바꿔준다.
		static DirectX::SimpleMath::Matrix AI2SM_MATRIX(const aiMatrix4x4& mat);
		static DirectX::SimpleMath::Quaternion AI2SM_QUATERNION(const aiQuaternion& quat);
		static DirectX::SimpleMath::Vector3 AI2SM_VECTOR3(const aiVector3D& vec);
		
		//Assimp의 Matrix를 DirectXMath로 바꿔준다.
		static DirectX::XMMATRIX AI2XM_MATRIX(const aiMatrix4x4& matrix);

		//ScreenSpace <-> WorldSpace 관련.
		static DirectX::XMVECTOR UnprojectScreenPointToWorldSpaceRay(DirectX::XMVECTOR screenPoint, DirectX::XMMATRIX viewProjection, DirectX::XMVECTOR viewportSize, DirectX::XMVECTOR viewportOrigin);
		static DirectX::XMVECTOR GetWorldPointFromScreenPoint_Depth(float screenPtX, float screenPtY, DirectX::XMMATRIX viewProjection, DirectX::XMMATRIX projectionMatrix,
			DirectX::XMVECTOR viewportSize, DirectX::XMVECTOR viewportOrigin, float depth);

		
		//Experimental
		static void DecomposeAssembleMatrix(DirectX::SimpleMath::Matrix& mat);
		//static void DecomposeAssembleFlipMatrix(DirectX::SimpleMath::Matrix& mat);
		static DirectX::SimpleMath::Quaternion QuaternionSlerpNoFlip(const DirectX::SimpleMath::Quaternion& q1, const DirectX::SimpleMath::Quaternion& q2, float t);
	
		static DirectX::XMFLOAT3 RotateVectorAroundAxis(DirectX::XMFLOAT3 vecToRotate, DirectX::XMFLOAT3 rotAxis, float angleInRad);

	};
}


