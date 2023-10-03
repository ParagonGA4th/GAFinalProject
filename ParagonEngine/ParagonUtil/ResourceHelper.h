#pragma once
#include "../ParagonCore/AssetDefines.h"
#include <map>
#include <string>

/// <summary>
/// AssetManager를 도와주는 리소스 헬퍼, 리소스 관리에 도움을 준다.
/// 리소스가 추가되면서, 구성이 계속 업데이트되어야 한다.
/// </summary>

namespace Pg::Util::Helper
{
	class ResourceHelper
	{
	public:
		ResourceHelper();
		~ResourceHelper();

		//다양한 상대적 경로 기록 양식을 하나의 양식으로 강제로 통합해 저장해주는 함수이다. 
		static std::string ForcePathUniform(const std::string& filePath);

		//eAssetDefine이 Graphics 리소스 소속인지, Engine 리소스 소속인지 구분해준다. (리소스가 추가되면서 점점 업데이트되어야 할 함수)
		static bool IsGraphicsResource(Pg::Core::Enums::eAssetDefine define);

		//그 자체로 : 렌더될 수 없는지 <-1> / 2D <0> / 3D <1> 인지를 구분해준다.
		//IsGraphicsResource를 포괄한다.
		static short IsPlainRenderable(Pg::Core::Enums::eAssetDefine define);

	private:

	};
}


