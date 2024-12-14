#pragma once
#include "ScriptInterface.h"
#include "../ParagonData/VisualEffectRenderObject.h"

namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Graphics { class PgGraphics; }
}

namespace Pg::Data
{
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class ArtifactBox : public ScriptInterface<ArtifactBox>
	{
		DEFINE_PARAGON_SCRIPT(ArtifactBox);

	public:
		ArtifactBox(Pg::Data::GameObject* obj);
		~ArtifactBox();

		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		bool _isOpen{ false };
		Pg::Data::VisualEffectRenderObject* _instructionRO{ nullptr };

	private:
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };

		//Pg::Data::GameObject* _interactionUI;
		//Pg::Data::ImageRenderer* _interaction;

		bool _onTriggerStay{ false };
	};
}