#pragma
#include "ScriptInterface.h"
#include <map>

namespace Pg::API::Time { class PgTime; }

/// <summary>
/// ����(�޺�) ���� ��ũ��Ʈ
/// Combo :
/// HIT : 1 / 2 / 3
/// POWER: 1 / 1.2 / 1.5
/// KNOCK-BACK : 0 / 0.5 / 1.0
/// ���ѹݺ� ���ư��鼭.
/// ��ũ��Ʈ�� �ƴ϶� CombatSystem�� ���������� ���� ȣ���� ��.
/// ������Ʈ�� �ƴ�!
/// </summary>
namespace Pg::DataScript
{
	class ComboSystem //: public ScriptInterface<ComboSystem>				
	{
	public:
		//�̱��� Script ��ũ�η� �ٲ㳦. -> �����ھ��� �ȵǰ�, ���� �ᵵ �ٸ� �������� Awake���� ���ٽ��!
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
		//���ʴ�� 1 / 2 / 3 �̴�.
		inline static const float DAMAGE_MULTIPLIER[3]
		{
			1.f, 1.2f, 1.5f
		};

	public:
		//CombatSystem�� ȣ���Ѵ�.
		void SystemStart();
		void SystemUpdate();

		// Combo�� ���� reset �ϴ� �Լ�(Time�� reset���� ����)
		void ReSetCombo();

		/// <summary>
		/// Hit ������ ���� bool �� ����(ȭ�� �߻� �ÿ��� ȣ��)
		/// </summary>
		/// <param name="isHit">true=Enemy Hit / false = OtherHit</param>
		void HitObject(bool isHit);

		// ���� combo�� ���� �������� �Լ�
		int GetComboCount();

	private:
		Pg::API::Time::PgTime* _deltaTime{ nullptr };
		std::pair<int, float> _prevCombo{ 0, 0.f };

		int _comboCount{ 0 };
		float _time{ 0.f };
		float _delay{ 1.0f };
	};
}

