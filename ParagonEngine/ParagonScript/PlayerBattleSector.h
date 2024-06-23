#pragma once
#include "../ParagonData/GameObject.h"
#include "IObserver.h"
#include "IScriptResettable.h"

/// <summary>
/// (구) PlayerBattleBehavior가 담당하는 역할.
/// </summary>
namespace Pg::DataScript
{
	class PlayerBattleSector : public IObserver, public IScriptResettable
	{
		friend class PlayerHandler;
	public:
		PlayerBattleSector(Pg::Data::GameObject* obj);

		//얘네들은 Component에 의해 작동되는 것 X, Script 자체에서 수명을 관리한다.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void LateUpdate();

	public:
		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

	private:
		Pg::Data::GameObject* _object;


	};
}


