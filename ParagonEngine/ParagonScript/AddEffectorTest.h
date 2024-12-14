#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class Transform;
	class Camera;
}

namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class AddEffectorTest : public ScriptInterface<AddEffectorTest>
	{
		DEFINE_PARAGON_SCRIPT(AddEffectorTest);

	public:
		AddEffectorTest(Pg::Data::GameObject* obj);
		~AddEffectorTest();

	private:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;


	private:
		void InUpdate_EffectTest();
		void InUpdate_MaterialChange();
		void InUpdate_Fade();

	private:
		Pg::API::Graphics::PgGraphics* _pgGraphics;
		Pg::API::Input::PgInput* _pgInput;

		Pg::Data::VisualEffectRenderObject* tObj{ nullptr };
		void* _ptrRenderMat{ nullptr };
	};
}


