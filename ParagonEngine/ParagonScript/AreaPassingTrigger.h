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
		
		//�ڽ��� Trigger, �ڽſ� �ε��� Collider���� ������ �޾Ƽ� Ȱ���ϰ� �ȴ�.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		//��� ��ü���� �ٸ� Area�� Index�� ǥ���ؾ� �ϱ⿡,
		//VISITABLE�� ǥ���ϱ�.
		BEGIN_VISITABLES(Pg::DataScript::AreaPassingTrigger);
		VISITABLE(int, _areaIndex);
		END_VISITABLES;

	public:
		void StoreDesignatedAreaHandler(BaseAreaHandler* areaHandler);

	private:
		BaseAreaHandler* _areaHandler{ nullptr };
	};
}


