#pragma once
#include "DynamicCollider.h"
#include "../ParagonData/DebugData.h"

namespace Pg::Data
{
	class GameObject;

	class SphereCollider : public DynamicCollider
	{
	public:
		SphereCollider(GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;
		
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float rad);

		Pg::Data::SphereInfo _sphereInfo;

	private:
		float _rad;
	};
}

