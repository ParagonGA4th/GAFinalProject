#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::API
{
	namespace Graphics { class PgGraphics; }
}


namespace Pg::Data
{
	class BoxCollider;
}

namespace Pg::DataScript
{
	class StairHelper : public ScriptInterface<StairHelper>
	{
		DEFINE_PARAGON_SCRIPT(StairHelper);

	public:
		StairHelper(Pg::Data::GameObject* obj);
		~StairHelper();
		virtual void Awake() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

		void EffectHelper(bool isActive);


		bool _onTriggerStay;

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		Pg::Data::VisualEffectRenderObject* _instructionRO{ nullptr };
	};
}


