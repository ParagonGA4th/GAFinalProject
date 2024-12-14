#include "ComboSystem.h"
#include "../ParagonAPI/PgTime.h"

#include <assert.h>
#include <singleton-cpp/singleton.h>

//P

namespace Pg::DataScript
{
	void ComboSystem::SystemStart()
	{
		auto& tdelta = singleton<Pg::API::Time::PgTime>();
		_deltaTime = &tdelta;
	}

	void ComboSystem::SystemUpdate()
	{
		_time += _deltaTime->GetDeltaTime();
	}

	void ComboSystem::ReSetCombo()
	{
		// 모든 것을 초기화
		_comboCount = 0;
		_prevCombo.first = 0;
		_prevCombo.second = 0;
	}

	void ComboSystem::HitObject(bool isHit)
	{
		if (!isHit) ReSetCombo(); // enemy가 아닌 오브젝트를 Hit 했을 경우 모든 콤보 리셋
		else
		{
			// 콤보 딜레이
			if (_time - _prevCombo.second < 0)
			{
				assert(false);
			}
			else if (_time - _prevCombo.second <= _delay)
			{
				// 딜레이 보다 짧은 시간 내에 공격에 성공 한다면 combo++
				if (_comboCount == MAXIMUM_HIT_COUNT) _comboCount = 1;
				else _comboCount++;

				// 성공한 콤보 저장
				_prevCombo.first = _comboCount;
				_prevCombo.second = _time;
			}
			else
			{
				// 딜레이 보다 늦은 시간 내에 공격 한다면 처음 부터 다시 시작
				_comboCount = 1;

				_prevCombo.first = _comboCount;
				_prevCombo.second = _time;
			}
		}
	}

	int ComboSystem::GetComboCount()
	{
		return _comboCount;
	}

}

