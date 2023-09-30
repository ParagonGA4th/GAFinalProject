#pragma once
#include "AssetDefines.h"

#include <string>
#include <unordered_map>
#include <vector>

/// <summary>
/// 게임 엔진 내의 모든 애셋/리소스 내용을 총괄할 매니저.
/// 실질적인 기능은 Engine, Graphics의 하위 리소스 매니저들이 담당하고, 
/// 본 AssetManager의 역할은 일종의 공유되는 장부와 비슷하다.
/// singleton-cpp로 인스턴스가 공유된다.
/// </summary>

namespace Pg::Core
{
	class CoreMain;
	class IEngine;
	class IGraphics;
}

namespace Pg::Core::Manager
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();
		
		//AssetManager 초기화. Engine/Graphics의 리소스 매니저를 받아온다.
		void Initialize(Pg::Core::CoreMain* core);

		//실제로 세부 Graphics, Engine의 리소스 매니저를 조종하기 위해, 로직 업데이트.
		void Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics);

		//리소스가 실제로 중앙 AssetManager 안에 존재하는지를 판단해준다.
		bool IsExistResource(const std::string& filepath, Pg::Core::Enums::eAssetDefine define);

		//리소스를 로드할 Queue에 넣는다.
		void LoadResource(const std::string& filepath, Pg::Core::Enums::eAssetDefine define);

		//리소스를 언로드할 Queue에 넣는다.
		void UnloadResource(const std::string& filepath);

	private:
		Pg::Core::CoreMain* _coreMain;

		//실제로 보관하는 리소스 맵. <파일 경로 / Asset의 종류>
		std::unordered_map<std::string, Pg::Core::Enums::eAssetDefine> _resourceMap;

		//매 프레임마다 업데이트되는 리소스 페어 (로드 예정)
		std::vector<std::pair<std::string, Pg::Core::Enums::eAssetDefine>> _perFrameToLoadResources;

		//매 프레임마다 업데이트되는 리소스 페어 (언로드 예정)
		std::vector<std::string> _perFrameToUnloadResources;
	};
}
