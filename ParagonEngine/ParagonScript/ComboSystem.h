#pragma
#include "ScriptInterface.h"
#include <map>

namespace Pg::Data { class GameObject; }
namespace Pg::API 
{
	namespace Time { class PgTime; }
	namespace Input { class PgInput; }
}

/// <summary>
/// 공격(콤보) 관련 스크립트
/// </summary>
namespace Pg::DataScript
{
	class ComboSystem : public ScriptInterface<ComboSystem>				
	{
		DEFINE_PARAGON_SCRIPT(ComboSystem);

	public:
		ComboSystem(Pg::Data::GameObject* obj);

	public:
		virtual void Start() override;
		virtual void Update() override;

		// Combo의 값을 reset 하는 함수
		void ReSetCombo();

		/// <summary>
		/// Hit 판정에 따라 bool 값 조절(화살 발사 시에만 호출)
		/// </summary>
		/// <param name="isHit">true=Enemy Hit / false = OtherHit</param>
		void HitObject(bool isHit);

		// 현재 combo의 값을 가져오는 함수
		int GetComboCount();

	private:
		Pg::API::Time::PgTime* _deltaTime;
		Pg::API::Input::PgInput* _pgInput;
		std::pair<int, float> _prevCombo;

		int _comboCount;
		float _time;
		float _delay;
	};
}

