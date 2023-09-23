#pragma once
#include <string>

/// <summary>
/// IRenderInfo를 업데이트하기 위해, 
/// 렌더러를 업데이트할 때 필요한 파일 스트링 경로 등을 포함하는 구조체.
/// (*) Shader, Material, 3DModelData를 관리할 것.
/// </summary>

namespace Pg::Core
{
	struct RenderTextData
	{
		std::string model3dPath;
		//std::string materialPath; //TBA
	};
}
