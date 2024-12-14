#include "TweenSystem.h"
#include "../ParagonUtil/Tween.h"
#include "../ParagonUtil/TweenTimer.h"
#include "../ParagonUtil/Log.h"
#include <algorithm>

namespace Pg::Engine
{
	TweenSystem::TweenSystem()
	{

	}

	TweenSystem::~TweenSystem()
	{

	}

	void TweenSystem::Initialize()
	{
		_preloadedDoTween.resize(PRELOADED_TWEEN_CNT);

		//100개 만들어서 채우기.
		std::generate(_preloadedDoTween.begin(), _preloadedDoTween.end(), []() {
			return new Pg::Util::Tween(); });
	
		//std::generate 정상인지 체크.
		assert(_preloadedDoTween.at(0) != _preloadedDoTween.at(1));
	}

	Pg::Util::Tween* TweenSystem::CreateTween()
	{
		///여기서 오브젝트 풀링이 사용되어야.
		Pg::Util::Tween* tVal{ nullptr };

		for (auto& it : _preloadedDoTween)
		{
			//안 쓰일 때 리턴.
			if (!it->GetIsUsed())
			{
				tVal = it;
				break;
			}
		}

		if (tVal != nullptr)
		{
			//정상적으로 pull된 저장소에서 활용될 수 있다. 
			tVal->ResetSelf();
			_tweensInAction.push_back(tVal);
		}
		else
		{
			//그 이상의 Tween이 활용된다는 얘기다. 
			//추가로 저장하자.
			PG_TRACE("100개 이상의 Tween 동시에 활용되고 있음. => 추가 생성함.");

			Pg::Util::Tween* temp = new Pg::Util::Tween();
			temp->ResetSelf();
			_tweensInAction.push_back(temp);
		}

		//어찌되었든, vector.back()에서 새로 활용될 값이 들어간다.
		return _tweensInAction.back();
	}

	void TweenSystem::Update()
	{
		for (Pg::Util::Tween* tween : _tweensInAction)
		{
			if (tween->_timer->_isActive == true)
			{
				tween->_timer->Update();
			}
			else
			{
				// 끝난 오브젝트를 모아두는 벡터에 현재 사용이 끝난 이터레이터를 넣는다
				if (tween->_onCompleteFunc != nullptr)
				{
					tween->_onCompleteFunc();
				}
				_deleteObj.push_back(tween);
			}
		}

		// 지울 오브젝트를 모아둔 벡터와 현재 벡터를 비교해서 지운다
		auto findObj = _deleteObj.begin();
		for (Pg::Util::Tween* i : _deleteObj)
		{
			auto dotweenIterator = std::find(_tweensInAction.begin(), _tweensInAction.end(), *findObj);
			if (dotweenIterator != _tweensInAction.end())
			{
				//여기가 지우는 코드. 더 이상 Delete는 하지 않는다.
				(*dotweenIterator)->ResetSelf();
				_tweensInAction.erase(dotweenIterator); //ERASE-REMOVEIF
			}
			++findObj;
		}

		// 지울 오브젝트를 모아둔 벡터를 클리어한다
		_deleteObj.clear();
	}

	void TweenSystem::Finalize()
	{
		for (auto& it : _preloadedDoTween)
		{
			if (it != nullptr)
			{
				delete it;
			}
		}
	}

	void TweenSystem::KillAllTweens()
	{
		//자신 모든거 리셋.
		for (auto& it : _tweensInAction)
		{
			it->ResetSelf();
		}
		
		//OnCompleteFunc 실행 못하게 함. 아예 다 죽여버리는 거라서.
		
		//두 런타임 객체 깨트리기.
		if (!_tweensInAction.empty())
		{
			_tweensInAction.clear();
		}

		if (!_deleteObj.empty())
		{
			_deleteObj.clear();
		}
	}
	

}
