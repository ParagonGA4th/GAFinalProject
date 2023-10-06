#pragma once
#include "../ParagonMath/PgMath.h"
#include "AssetDefines.h"
#include <string>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

/// <summary>
/// IRenderInfo를 업데이트하기 위해, 
/// 렌더러를 업데이트할 때 필요한 파일 스트링 경로 등을 포함하는 구조체.
/// 렌더할 때 필요한 정보를 전달한다.
/// (*) Shader, Material, 3DModelData를 관리할 것.
/// </summary>

namespace Pg::Data
{
	using Pg::Data::Enums::eAssetDefine;

	struct RenderTextData
	{
		RenderTextData();

		//어떤 종류의 리소스인지를 명시한다.
		eAssetDefine _assetDefine;

		//FLOAT3, 현재 오브젝트가 위치한 포지션.
		Pg::Math::PGFLOAT3 position;
		//쿼터니언으로 회전 정보 전달.
		Pg::Math::PGQuaternion rotation;
		//FLOAT3, 현재 오브젝트가 위치한 스케일.
		Pg::Math::PGFLOAT3 scale;
		
		//현재 사용하는 Visual Resource의 경로.
		std::string resourcePath;
		//std::string materialPath; //TBA
	};

	inline RenderTextData::RenderTextData() :
		_assetDefine(eAssetDefine::_NONE), position(0.f, 0.f, 0.f),
		rotation(0.f, 0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), resourcePath()
	{
		
	}

}
