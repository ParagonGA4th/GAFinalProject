#pragma once

/// <summary>
/// 그래픽스 엔진의 각종 디버깅 데이터들을 씬으로 보내기 위한 구조체
/// 
/// 23. 11. 15. 김정식
/// </summary>
namespace Pg::Data
{
	struct GraphicsDebugData
	{
		unsigned int _renderedObjectCount;
	};
}