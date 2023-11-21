#pragma once
#include "DynamicCollider.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"

/// <summary>
/// 변지상의 BoxCollider
/// Box형 충돌 디버그를 띄우기 위한 정보만 컴포넌트로 가지고 있을 예정
/// 2023.10.22
/// </summary>

namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Data
{
	class BoxCollider : public DynamicCollider
	{
	public:
		BoxCollider(GameObject* owner);

		void Update();

		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

		void SetScale(float w, float h, float d);

		Pg::Data::BoxInfo _boxInfo;
	private:
		float _width;
		float _height;
		float _depth;


	};
}

