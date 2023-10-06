#pragma once
#include "../ParagonData/AssetDefines.h"
#include <string>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
namespace Pg::Core
{
	class IEngine abstract
	{
	public:
		virtual void Initialize(float width, float height) abstract;

		virtual void Update() abstract;

		virtual void Finalize() abstract;

		//그래픽스 엔진에서 필요한 정보
		//virtual Scene* GetCurrentScene();

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 로드하는 함수.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager와의 교류를 위해, 엔진 리소스 매니저에 리소스를 언로드하는 함수.
		virtual void UnloadResource(const std::string& filePath) abstract;
	};
}