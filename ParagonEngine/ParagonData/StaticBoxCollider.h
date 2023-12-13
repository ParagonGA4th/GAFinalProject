#pragma once
#include "StaticCollider.h"

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

	private:
		float _width;
		float _height;
		float _depth;
	};
}

