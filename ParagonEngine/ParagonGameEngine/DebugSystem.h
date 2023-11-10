#pragma once
#include "../ParagonMath/PgMath.h"
#include "DebugData.h"
#include <vector>
#include <string>

/// <summary>
/// 변지상의 DebugSystem.
/// 기본적으로 Collider의 Debug를 띄울 예정이다.
/// 2023.11.09
/// </summary>
namespace Pg::Engine
{
	class DebugSystem
	{
	public:
		void Initialize();
		void Finalize();

	public:


		void SetDebugMode(bool isdebug);
	private:
		std::vector<BoxInfo> _boxVec;

		//디버그 모드 플래그
		bool _isDebug;
	};

}

