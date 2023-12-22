#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class GameObject;

	class StaticBoxCollider : public StaticCollider
	{
	public:
		StaticBoxCollider(GameObject* owner);

	public:
		virtual void Update() override;

		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

	public:
		void SetScale(float w, float h, float d);

		Pg::Data::BoxInfo _boxInfo;

	private:
		float _width;
		float _height;
		float _depth;
	};
}

