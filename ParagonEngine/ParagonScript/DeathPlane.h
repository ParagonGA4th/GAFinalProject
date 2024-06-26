#pragma once
#include "ScriptInterface.h"
#include "BaseAreaHandler.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::DataScript
{
	class DeathPlane : public ScriptInterface<DeathPlane>
	{
		DEFINE_PARAGON_SCRIPT(DeathPlane);

	public:
		DeathPlane(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

		//자신이 Trigger, 자신에 부딪힌 Collider들의 정보를 받아서 활용하게 된다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	
	public:
		void StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler);
		

	private:
		BaseAreaHandler* _areaHandler{ nullptr };
	
	};
}


