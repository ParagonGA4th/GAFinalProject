#pragma once

#include <vector>
#include "EngineDLLExporter.h"

/// <summary>
/// Tween�� �䱸�Ǵ� ���� ��ü. 
/// Special Thanks To : @chloe980504
/// </summary>

namespace Pg::Util
{
	class Tween;
}

namespace Pg::Engine
{
	class TweenSystem
	{
	public:
		PARAGON_ENGINE_DLL TweenSystem();		// Singleton���� ���� ����, singleton-cpp.
		PARAGON_ENGINE_DLL ~TweenSystem();

		enum {PRELOADED_TWEEN_CNT = 100};

	public:
		void Initialize();
		void Update();
		void Finalize();

	private:
			// ����ϰ� �ִ� ��Ʈ�� ��ü�� ����
		std::vector<Pg::Util::Tween*> _tweensInAction;
		// ����� ������ ����� ���� ��Ʈ�� ��ü�� ����
		std::vector<Pg::Util::Tween*> _deleteObj;

	private:
		//�̹� ���� Tween�� ��������, ��ŷ�س���!
		std::vector<Pg::Util::Tween*> _preloadedDoTween;

	public:
		//API�� �ͽ����ÿ� : Ʈ���� �����.
		PARAGON_ENGINE_DLL Pg::Util::Tween* CreateTween();
		PARAGON_ENGINE_DLL void KillAllTweens();
		
	};
}
