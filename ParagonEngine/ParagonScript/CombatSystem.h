#pragma once
#include "ScriptInterface.h"
#include "IEvent.h"
#include "ScriptEventHelper.h"
#include <functional>
#include <string>
#include <variant>
#include <cassert>

/// 실제로 오브젝트를 사용하는 클래스들의 전방선언이 있을 곳.
namespace Pg::DataScript
{
	class PlayerHandler;
	class CombatSystem;
	class BaseMonster;
}

/// <summary>
/// 게임 안에서 컴뱃을 관리하는 시스템. 싱글턴 스크립트로 관리된다.
/// 일종의 Event Manager 형태 + 멤버변수 하드코딩 할당으로 운영이 될 것. (싱글-쓰레드만 지원)
/// Reference : https://bastian.rieck.me/blog/2015/event_system_cxx11/
/// 현재로서는 스크립트로서 적용된 오브젝트가 없다.
/// </summary>

namespace Pg::DataScript
{
	class CombatSystem : public ScriptInterface<CombatSystem>
	{
		//이 매크로를 쓴다면, 생성자를 멋대로 쓰면 안된다.
		//다른 생성자에 멋대로 Singleton CombatSystem 가져오면 안됨!
		//또한, DontDestroyOnLoad 오브젝트에 가져다가 써야 할 것.
		DEFINE_PARAGON_SCRIPT_SINGLETON(CombatSystem);
	
	public:
		virtual void Start() override;

		// 대부분의 로직을 FixedUpdate()에서 활용했으면 좋겠다. 
		// Update에서 로직을 처리하고 - FixedUpdate에서 CombatSystem - 
		// LateUpdate에서 받은 오브젝트가 이에 맞는 처리를 해준다.
		virtual void FixedUpdate() override;

	public:
		// Player의 프로퍼티를 직접 변경하는 Wrapper이다. 무조건 Combat System을 통해서 게임 로직을 진행해야
		// 꼬이지 않게 할 것이다.
		
		//플레이어에게 들어오는 개별적인 로직은 따로 분리됨.
		void ChangePlayerHealth(float level);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(float level);

	private:
		//내부의 데이터들을 전부 리셋한다.
		void ResetAll();

		//개별적으로 Subscribe해도 되지만, 좀더 일부 요소는 편하게 작동시킬 수 있게 한다.
		//이 함수를 통해 플레이어를 Combat System에게 부착시킨다. 
		void RegisterPlayer(Pg::Data::GameObject* obj);

		//여기에는 Monster들의 상위 BaseClass / Interface가 존재해야 한다.
		void RegisterSingleMonster(Pg::Data::GameObject* obj);

		//게임 안에서 쓰이는 모든 투사체들의 상위 BaseClass / Interface가 존재해야 한다.
		void RegisterProjectiles(Pg::Data::GameObject* obj);


	private:
		PlayerHandler* _player{ nullptr };
		std::vector<Pg::Data::GameObject*> _monsterList;
		std::vector<Pg::Data::GameObject*> _projectileList;


	public: //Combat System이 일종의 이벤트 시스템처럼 동작하는 부분이다.
		
		//이벤트가 호출될 때마다, 해당 식별자를 받고, 맵을 오가서 Observer 전체를 호출.
		void Subscribe(const std::string& descriptor, SlotType&& slot);

		//자신에 관심 있는 모든 옵서버들한테 이벤트들을 보낼 수 있다.
		void Post(const IEvent& eventVal, UsedVariant usedVariant1, UsedVariant usedVariant2) const;

	private:
		std::map<std::string, std::vector<SlotType>> _observers;
		CombatSystem* _combatSystem;
	};
}



