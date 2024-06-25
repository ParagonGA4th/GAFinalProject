#pragma once
#include "ScriptInterface.h"
#include "BaseAreaHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class AreaPassingTrigger : public ScriptInterface<AreaPassingTrigger>
	{
		DEFINE_PARAGON_SCRIPT(AreaPassingTrigger);

	public:
		AreaPassingTrigger(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		
		//자신이 Trigger, 자신에 부딪힌 Collider들의 정보를 받아서 활용하게 된다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		//얘는 객체별로 다른 Area의 Index를 표현해야 하기에,
		//VISITABLE로 표현하기.
		BEGIN_VISITABLES(Pg::DataScript::AreaPassingTrigger);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		void StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler);

	private:
		BaseAreaHandler* _areaHandler{ nullptr };
	};
}


