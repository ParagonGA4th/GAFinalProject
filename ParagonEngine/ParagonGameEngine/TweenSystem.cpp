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

		//100�� ���� ä���.
		std::generate(_preloadedDoTween.begin(), _preloadedDoTween.end(), []() {
			return new Pg::Util::Tween(); });
	
		//std::generate �������� üũ.
		assert(_preloadedDoTween.at(0) != _preloadedDoTween.at(1));
	}

	Pg::Util::Tween* TweenSystem::CreateTween()
	{
		///���⼭ ������Ʈ Ǯ���� ���Ǿ��.
		Pg::Util::Tween* tVal{ nullptr };

		for (auto& it : _preloadedDoTween)
		{
			//�� ���� �� ����.
			if (!it->GetIsUsed())
			{
				tVal = it;
				break;
			}
		}

		if (tVal != nullptr)
		{
			//���������� pull�� ����ҿ��� Ȱ��� �� �ִ�. 
			tVal->ResetSelf();
			_tweensInAction.push_back(tVal);
		}
		else
		{
			//�� �̻��� Tween�� Ȱ��ȴٴ� ����. 
			//�߰��� ��������.
			PG_TRACE("100�� �̻��� Tween ���ÿ� Ȱ��ǰ� ����. => �߰� ������.");

			Pg::Util::Tween* temp = new Pg::Util::Tween();
			temp->ResetSelf();
			_tweensInAction.push_back(temp);
		}

		//����Ǿ���, vector.back()���� ���� Ȱ��� ���� ����.
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
				// ���� ������Ʈ�� ��Ƶδ� ���Ϳ� ���� ����� ���� ���ͷ����͸� �ִ´�
				if (tween->_onCompleteFunc != nullptr)
				{
					tween->_onCompleteFunc();
				}
				_deleteObj.push_back(tween);
			}
		}

		// ���� ������Ʈ�� ��Ƶ� ���Ϳ� ���� ���͸� ���ؼ� �����
		auto findObj = _deleteObj.begin();
		for (Pg::Util::Tween* i : _deleteObj)
		{
			auto dotweenIterator = std::find(_tweensInAction.begin(), _tweensInAction.end(), *findObj);
			if (dotweenIterator != _tweensInAction.end())
			{
				//���Ⱑ ����� �ڵ�. �� �̻� Delete�� ���� �ʴ´�.
				(*dotweenIterator)->ResetSelf();
				_tweensInAction.erase(dotweenIterator); //ERASE-REMOVEIF
			}
			++findObj;
		}

		// ���� ������Ʈ�� ��Ƶ� ���͸� Ŭ�����Ѵ�
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
		//�ڽ� ���� ����.
		for (auto& it : _tweensInAction)
		{
			it->ResetSelf();
		}
		
		//OnCompleteFunc ���� ���ϰ� ��. �ƿ� �� �׿������� �Ŷ�.
		
		//�� ��Ÿ�� ��ü ��Ʈ����.
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
