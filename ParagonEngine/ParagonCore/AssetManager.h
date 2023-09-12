#pragma once
#include "Singleton.h"

/// <summary>
/// 게임 엔진 내의 모든 애셋/리소스 내용을 총괄할 매니저.
/// </summary>

namespace Pg::Core::Manager
{
	class AssetManager : public Singleton<AssetManager>
	{
	public:
		AssetManager();
		~AssetManager();



	private:
		
		



	};
}
