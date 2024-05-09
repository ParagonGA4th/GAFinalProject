#pragma once
#include "../ParagonData/AssetDefines.h"
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

		//리소스가 해당 위치에 존재하는지 알려주는 Wrapper 함수.
		 static bool IsFileExist(const std::string& filePath);

		//다양한 상대적 경로 기록 양식을 하나의 양식으로 강제로 통합해 저장해주는 함수이다. 
		 static std::string ForcePathUniform(const std::string& filePath);

		//ForcePathUniform + Relative Path인 경우 Full Path로 만들어준다.
		 static std::string ForcePathUniformFull(const std::string& filePath);

		//eAssetDefine이 Graphics 리소스 소속인지, Engine 리소스 소속인지 구분해준다. (리소스가 추가되면서 점점 업데이트되어야 할 함수)
		 static bool IsGraphicsResource(Pg::Data::Enums::eAssetDefine define);

		//그 자체로 : 렌더될 수 없는지 <-1> / 2D <0> / 3D <1> 인지를 구분해준다.
		//IsGraphicsResource를 포괄한다.
		 static short IsPlainRenderable(Pg::Data::Enums::eAssetDefine define);

		 //리소스의 확장자가 DDS인지, 아닌지를 알려준다.
		 static bool IsResourceDDS(const std::string& filePath);
		 static bool IsResourceDDS(const std::wstring& filePath);

		 //리소스의 확장자가 TGA인지, 아닌지를 알려준다.
		 static bool IsResourceTGA(const std::string& filePath);
		 static bool IsResourceTGA(const std::wstring& filePath);

		 //리소스의 확장자가 EXR인지 (Unity Lightmap 호환 포맷), 아닌지를 알려준다.
		 static bool IsResourceEXR(const std::string& filePath);
		 static bool IsResourceEXR(const std::wstring& filePath);

		 static std::string GetNameFromPath(const std::string& filePath);

		 //디버그 / 릴리즈 모드를 구별해서, 만약 릴리즈 모드일 경우 Debug->Release로 String 값을 바꾼다.
		 static std::string IfReleaseChangeDebugText(const std::string& filePath);
		 static std::wstring IfReleaseChangeDebugTextW(const std::wstring& filePath);
	private:

	};
}


