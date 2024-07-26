#include "BaseMonster.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgGraphics.h"
#include "../ParagonUtil/Log.h"
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	BaseMonsterInfo::BaseMonsterInfo(float fullHealth, float attackPower) :
		_hp(fullHealth), _fullHealthValue(fullHealth), _damage(attackPower)
	{
	
	}

	BaseMonsterInfo::~BaseMonsterInfo()
	{
		if (_hitVO != nullptr)
		{
			_pgGraphics->RemoveEffectObject(_hitVO);
			_pgGraphics->RemoveEffectObject(_fireHitVO);
			_pgGraphics->RemoveEffectObject(_iceHitVO);

			delete _hitVO;
			delete _fireHitVO;
			delete _iceHitVO;
		}
	}

	void BaseMonsterInfo::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		assert(false && "이거 자체로 호출되면 오류다!");
	}

	void BaseMonsterInfo::ResetAll()
	{
		assert(false && "이거 자체로 호출되면 오류다.");
	}

	void BaseMonsterInfo::ChangeMonsterHp(float level)
	{
		_hp = std::clamp<float>(_hp + level, 0.0f, _fullHealthValue);

		//죽으면 OnDead 호출.
		if (_hp <= std::numeric_limits<float>::epsilon())
		{
			_onDead();
		}
	}

	void BaseMonsterInfo::SetMonsterDamage(float damage)
	{
		_damage = damage;
	}

	void BaseMonsterInfo::SetMonsterHp(float hp)
	{
		_hp = hp;
	}

	float BaseMonsterInfo::GetMonsterHp()
	{
		return _hp;
	}

	float BaseMonsterInfo::GetMonsterDamage()
	{
		return _damage;
	}

	bool BaseMonsterInfo::IsMonsterDead()
	{
		return _hp <= std::numeric_limits<float>::epsilon();
	}

	void BaseMonsterInfo::SetMonsterSpeedRatio(float speed)
	{
		_speed = speed;
	}

	float BaseMonsterInfo::GetMonsterSpeedRatio()
	{
		return _speed;
	}

	void BaseMonsterInfo::StartBaseMonsterLogic()
	{
		_isActivatedEffect = false;
		_isActivatedTime = 0.f;

		_getPgTime = &singleton<Pg::API::Time::PgTime>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();

		_hitVO = new Pg::Data::VisualEffectRenderObject;
		_fireHitVO = new Pg::Data::VisualEffectRenderObject;
		_iceHitVO = new Pg::Data::VisualEffectRenderObject;

		_pgGraphics->RegisterEffectObject("Effect_Normal_Hit", _hitVO);
		_pgGraphics->RegisterEffectObject("Effect_Fire", _fireHitVO);
		_pgGraphics->RegisterEffectObject("Effect_Ice", _iceHitVO);

		_ptrEffectFrameIndex = _pgGraphics->GetEffectTextureIndexPointer("Effect_Normal_Hit");
		_ptrEffectFireFrameIndex = _pgGraphics->GetEffectTextureIndexPointer("Effect_Fire");
		_ptrEffectIceFrameIndex = _pgGraphics->GetEffectTextureIndexPointer("Effect_Ice");
	}

	//베이스 몬스터의 로직  Update 문 내부에 위치해야.
	void BaseMonsterInfo::UpdateBaseMonsterLogic(Pg::Data::GameObject* obj)
	{
		_hitVO->SetActive(false);
		_fireHitVO->SetActive(false);
		_iceHitVO->SetActive(false);

		if (_isActivatedEffect)
		{
			_isActivatedTime += _getPgTime->GetDeltaTime();
			
			//PG_WARN("SHOWING");
			_chosenVO->_position = obj->_transform._position;
			_chosenVO->_scale = { 9,9,9 };
			_chosenVO->SetActive(true);

			if (_isActivatedTime < 0.1f) { (*_chosenPtrEffect) = 0; }
			else if (_isActivatedTime < 0.2f) { (*_chosenPtrEffect) = 1; }
			else if (_isActivatedTime < 0.3f) { (*_chosenPtrEffect) = 2; }
			else { (*_chosenPtrEffect) = 3; }

			if (_isActivatedTime > HIT_EFFECT_LASTING_TIME)
			{
				_isActivatedEffect = false;
				_isActivatedTime = 0.f;
			}
		}
	}

	//밑에서 호출해야.
	void BaseMonsterInfo::OnHitEnableHitEffect(int hitType)
	{
		switch (hitType)
		{
			case -1:
			{
				_chosenVO = _iceHitVO;
				_chosenPtrEffect = _ptrEffectIceFrameIndex;
			}
			break;
			case 0:
			{
				_chosenVO = _hitVO;
				_chosenPtrEffect = _ptrEffectFrameIndex;
			}
			break;
			case 1:
			{
				_chosenVO = _fireHitVO;
				_chosenPtrEffect = _ptrEffectFireFrameIndex;
			}
			break;
			default: { assert(false); } break;
		}

		_isActivatedEffect = true;
		_isActivatedTime = 0.f;
	}

	

}