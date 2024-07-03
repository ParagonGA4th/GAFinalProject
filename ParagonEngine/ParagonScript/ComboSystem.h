#pragma
#include "ScriptInterface.h"
#include <map>

namespace Pg::API::Time { class PgTime; }

/// <summary>
/// 공격(콤보) 관련 스크립트
/// Combo :
/// HIT : 1 / 2 / 3
/// POWER: 1 / 1.2 / 1.5
/// KNOCK-BACK : 0 / 0.5 / 1.0
/// 무한반복 돌아가면서.
/// 스크립트가 아니라 CombatSystem이 종속적으로 만들어서 호출할 것.
/// 컴포넌트가 아님!
/// </summary>
namespace Pg::DataScript
{
	class ComboSystem //: public ScriptInterface<ComboSystem>				
	{
	public:
		//싱글턴 Script 매크로로 바꿔낌. -> 생성자쓰면 안되고, 갖다 써도 다른 곳에서는 Awake부터 갖다써야!
		//DEFINE_PARAGON_SCRIPT_SINGLETON(ComboSystem);
		static ComboSystem* GetInstance()
		{
			static ComboSystem inst;
			return &inst;
		}
	private:
		ComboSystem() {};

	public:
		inline static const int MAXIMUM_HIT_COUNT = 3;
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		//차례대로 1 / 2 / 3 이다.
		inline static const float DAMAGE_MULTIPLIER[3]
		{
			1.f, 1.2f, 1.5f
		};

	public:
		//CombatSystem이 호출한다.
		void SystemStart();
		void SystemUpdate();

		// Combo의 값을 reset 하는 함수(Time은 reset되지 않음)
		void ReSetCombo();

		/// <summary>
		/// Hit 판정에 따라 bool 값 조절(화살 발사 시에만 호출)
		/// </summary>
		/// <param name="isHit">true=Enemy Hit / false = OtherHit</param>
		void HitObject(bool isHit);

		// 현재 combo의 값을 가져오는 함수
		int GetComboCount();

	private:
		Pg::API::Time::PgTime* _deltaTime{ nullptr };
		std::pair<int, float> _prevCombo{ 0, 0.f };

		int _comboCount{ 0 };
		float _time{ 0.f };
		float _delay{ 1.0f };
	};
}

