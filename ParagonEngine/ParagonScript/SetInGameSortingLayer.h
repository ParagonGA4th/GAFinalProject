#pragma once
#include "ScriptInterface.h"
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class ImageRenderer;
}

namespace Pg::DataScript
{
	class SetInGameSortingLayer : public ScriptInterface<SetInGameSortingLayer>
	{
		DEFINE_PARAGON_SCRIPT(SetInGameSortingLayer);

	public:
		SetInGameSortingLayer(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		BEGIN_VISITABLES(Pg::DataScript::SetInGameSortingLayer);
		VISITABLE(int, _sortingLayer);
		END_VISITABLES;

	private:
		Pg::Data::ImageRenderer* _imageRenderer{ nullptr };
	};
}


