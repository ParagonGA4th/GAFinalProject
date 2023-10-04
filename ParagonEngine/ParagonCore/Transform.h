#pragma once
#include "Component.h"
#include "../ParagonAPI/PgMath.h"

#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 변지상의 Transform 클래스.
/// 일단 
/// </summary>
/// 
/// 23.09.25 오수안
/// 쿼터니언, 오일러 변환 함수 추가
/// 
namespace Pg::Core
{
	class GameObject;
}

namespace Pg::Core
{
	using namespace Pg::Math;

	class Transform : public Component
	{
	public:
		// 임시 기본생성자
		Transform() = default;
		Transform(GameObject* obj);

		// Get 월드 함수들
		PGFLOAT3 GetPosition() const;
		PGQuaternion GetRotation() const;
		PGFLOAT3 GetScale() const;

		// Get 로컬 함수들
		PGFLOAT3 GetLocalScale();
		PGFLOAT3 GetLocalPosition();
		PGQuaternion GetLocalRotation();


		// Set 월드 함수들
		void SetPosition(const PGFLOAT3& pos);
		void SetRotation(const PGQuaternion& rot);
		void SetScale(const PGFLOAT3& scale);

		// Set 로컬 함수들
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(PGFLOAT3& pos);
		void SetLocalRotation(float w, float x, float y, float z);
		void SetLocalRotation(PGQuaternion& rot);
		void SetLocalScale(float x, float y, float z);
		void SetLocalScale(PGFLOAT3& sca);
		void SetLocalRotationEuler(float x, float y, float z);
		void SetLocalRotationEuler(PGFLOAT3& euler);

		// 오일러 && 쿼터니언 변환 함수들
		PGQuaternion EulerToQuaternion(float x, float y, float z);
		PGFLOAT3 QuaternionToEuler(const PGQuaternion& quaternion);

		

		// 종속성 세팅 함수
		bool HasParent();
		std::shared_ptr<Transform> GetParent();


	private:
		// 부모, 자식 객체를 가리키는 transform
		std::shared_ptr<Transform> _parent;
		std::vector<std::shared_ptr<Transform>> _children;

		//PRS
		PGFLOAT3 _position;
		PGQuaternion _rotation;
		PGFLOAT3 _scale;
	};
}

