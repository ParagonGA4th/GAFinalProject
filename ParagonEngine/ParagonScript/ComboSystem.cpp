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
		// ��� ���� �ʱ�ȭ
		_comboCount = 0;
		_prevCombo.first = 0;
		_prevCombo.second = 0;
	}

	void ComboSystem::HitObject(bool isHit)
	{
		if (!isHit) ReSetCombo(); // enemy�� �ƴ� ������Ʈ�� Hit ���� ��� ��� �޺� ����
		else
		{
			// �޺� ������
			if (_time - _prevCombo.second < 0)
			{
				assert(false);
			}
			else if (_time - _prevCombo.second <= _delay)
			{
				// ������ ���� ª�� �ð� ���� ���ݿ� ���� �Ѵٸ� combo++
				if (_comboCount == MAXIMUM_HIT_COUNT) _comboCount = 1;
				else _comboCount++;

				// ������ �޺� ����
				_prevCombo.first = _comboCount;
				_prevCombo.second = _time;
			}
			else
			{
				// ������ ���� ���� �ð� ���� ���� �Ѵٸ� ó�� ���� �ٽ� ����
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

