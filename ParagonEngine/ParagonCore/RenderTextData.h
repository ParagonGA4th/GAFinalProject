#pragma once
#include <string>
#include "../ParagonAPI/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

/// <summary>
/// IRenderInfo를 업데이트하기 위해, 
/// 렌더러를 업데이트할 때 필요한 파일 스트링 경로 등을 포함하는 구조체.
/// 렌더할 때 필요한 정보를 전달한다.
/// (*) Shader, Material, 3DModelData를 관리할 것.
/// </summary>

namespace Pg::Core
{
	struct RenderTextData
	{
		RenderTextData();

		//3D인지, 3D가 아닌지를 체크하기 위해. (후)
		bool is3D;

		//FLOAT3, 현재 오브젝트가 위치한 포지션.
		Pg::Math::PGFLOAT3 position;
		//쿼터니언으로 회전 정보 전달.
		Pg::Math::PGQuaternion rotation;
		//FLOAT3, 현재 오브젝트가 위치한 스케일.
		Pg::Math::PGFLOAT3 scale;
		
		//현재 사용하는 3DModel의 경로.
		std::string model3dPath;
		//std::string materialPath; //TBA
	};

	inline RenderTextData::RenderTextData() :
		is3D(true), position(0.f, 0.f, 0.f), 
		rotation(0.f, 0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), model3dPath()
	{
		
	}

}
