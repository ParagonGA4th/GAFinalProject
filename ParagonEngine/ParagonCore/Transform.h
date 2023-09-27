#pragma once
#include "Component.h"
#include "../ParagonAPI/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// 변지상의 Transform 클래스.
/// 일단 
/// </summary>
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
		Transform(GameObject* obj);

		//Get 함수들
		PGFLOAT3 GetPosition() const;
		PGQuaternion GetRotation() const;
		PGFLOAT3 GetScale() const;

		//Set 함수들
		void SetPosition(const PGFLOAT3& pos);
		void SetRotation(const PGQuaternion& rot);
		void SetScale(const PGFLOAT3& scale);

	private:
		//PRS
		PGFLOAT3 _position;
		PGQuaternion _rotation;
		PGFLOAT3 _scale;
	};
}

