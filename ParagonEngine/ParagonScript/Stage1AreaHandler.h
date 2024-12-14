#pragma once
#include "ScriptInterface.h"
#include "BaseAreaHandler.h"

namespace Pg::DataScript
{
	class Stage1AreaHandler : public ScriptInterface<Stage1AreaHandler>, public BaseAreaHandler
	{
		DEFINE_PARAGON_SCRIPT(Stage1AreaHandler);

	public:
		//�ڽ��� ���� Stage�� Area ��ü ����.
		enum {ENTIRE_AREA_COUNT = 3};

	public:
		Stage1AreaHandler(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Start() override;
		virtual void Update() override;

	};
}


