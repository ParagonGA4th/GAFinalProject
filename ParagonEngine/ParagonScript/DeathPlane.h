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

		//�ڽ��� Trigger, �ڽſ� �ε��� Collider���� ������ �޾Ƽ� Ȱ���ϰ� �ȴ�.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	
	public:
		void StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler);
		

	private:
		BaseAreaHandler* _areaHandler{ nullptr };
	
	};
}


