#pragma
#include "ScriptInterface.h"
#include <map>

namespace Pg::Data { class GameObject; }
namespace Pg::API 
{
	namespace Time
	{
		class PgTime;
	}
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

		void ReSetCombo();
		void HitObject(bool isHit);
		int GetComboCount();

	private:
		Pg::API::Time::PgTime* _deltaTime;
		std::pair<int, float> _prevCombo;
		int _comboCount;
		float _time;
	};
}

