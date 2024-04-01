#pragma once
#include "StaticCollider.h"
#include "DebugData.h"
#include "../ParagonMath/PgMath.h"

#include "data_factory.h"

namespace Pg::Data
{
	class GameObject;

	class StaticBoxCollider : public StaticCollider, Pg::Factory::Data::RegisteredInFactory<StaticCollider, StaticBoxCollider, GameObject*>
	{
	public:
		StaticBoxCollider(GameObject* owner);

		static StaticCollider* CreateInstance(GameObject* go) { return new StaticBoxCollider(go); }
		static const char* GetFactoryKey() { return "class Pg::Data::StaticBoxCollider"; }

	public:
		virtual void Update() override;

		float GetWidth() const;
		float GetHeight() const;
		float GetDepth() const;

		void SetLayer(int lay);
		int GetLayer();

	public:
		void SetScale(float w, float h, float d);

		Pg::Data::BoxInfo _boxInfo;

	private:
		float _width;
		float _height;
		float _depth;

		int _layer;
	};
}

