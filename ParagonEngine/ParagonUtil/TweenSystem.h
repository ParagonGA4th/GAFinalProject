#include <vector>
#include "UtilDLLExporter.h"

/// <summary>
/// Tween에 요구되는 개별 객체. 
/// Special Thanks To : @chloe980504
/// </summary>

namespace Pg::Util
{
	class Tween;
}

namespace Pg::Util
{
	class TweenSystem
	{
	public:
		PARAGON_UTIL_DLL TweenSystem();		// Singleton으로 쓰일 예정, singleton-cpp.
		PARAGON_UTIL_DLL ~TweenSystem();

	public:
		void Initialize();
		void Update();
		void Finalize();

	private:
			// 사용하고 있는 닷트윈 객체의 벡터
		std::vector<Tween*> _dotweens;
		// 사용이 끝나서 지우기 위한 닷트윈 객체의 벡터
		std::vector<Tween*> _deleteObj;

	public:
		//API로 익스포팅용 : 트윈을 만들기.
		PARAGON_UTIL_DLL Tween* CreateTween();
		
	};
}
