#pragma once

#include <vector>
#include "EngineDLLExporter.h"

/// <summary>
/// Tween에 요구되는 개별 객체. 
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
		PARAGON_ENGINE_DLL TweenSystem();		// Singleton으로 쓰일 예정, singleton-cpp.
		PARAGON_ENGINE_DLL ~TweenSystem();

		enum {PRELOADED_TWEEN_CNT = 100};

	public:
		void Initialize();
		void Update();
		void Finalize();

	private:
			// 사용하고 있는 닷트윈 객체의 벡터
		std::vector<Pg::Util::Tween*> _tweensInAction;
		// 사용이 끝나서 지우기 위한 닷트윈 객체의 벡터
		std::vector<Pg::Util::Tween*> _deleteObj;

	private:
		//이미 많은 Tween을 만들어놓고, 링킹해놓기!
		std::vector<Pg::Util::Tween*> _preloadedDoTween;

	public:
		//API로 익스포팅용 : 트윈을 만들기.
		PARAGON_ENGINE_DLL Pg::Util::Tween* CreateTween();
		PARAGON_ENGINE_DLL void KillAllTweens();
		
	};
}
