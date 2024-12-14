#pragma once
#include "../ParagonData/AssetDefines.h"

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
	class ProcessMain;
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
		
		//기본 리소스 세팅 전을 위해.
		void InitializeDefaults();

		//AssetManager 초기화. Engine/Graphics의 리소스 매니저를 받아온다.
		void Initialize(Pg::Core::ProcessMain* core, const std::string& resourceListPath);

		//실제로 세부 Graphics, Engine의 리소스 매니저를 조종하기 위해, 로직 업데이트.
		void Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics);

		//애셋 매니저 특성상, 같은 이름을 가진 파일들이 여러 개 있으면 안된다.
		void AssureNoNameDuplicates();

		//리소스가 실제로 중앙 AssetManager 안에 존재하는지를 판단해준다. (*같은 파일 경로로 다른 Asset Enum 들어갈 오류의 경우의 수 곧 생각해야.)
		bool IsExistResource(const std::string& filepath);

		//리소스를 로드할 Queue에 넣는다.
		void LoadResource(const std::string& filepath, Pg::Data::Enums::eAssetDefine define);

		//리소스를 언로드할 Queue에 넣는다.
		void UnloadResource(const std::string& filepath);
	
		//어셋 종류 기반 전체 Path List 반환.
		std::vector<std::string> GetResourcesPathByDefine(Pg::Data::Enums::eAssetDefine define);
		
		//어셋 종류 기반 전체 Name List 반환.
		std::vector<std::string> GetResourcesNameByDefine(Pg::Data::Enums::eAssetDefine define);

		//어셋 중 Material을 가지고 올 때, Default Material을 빼고 반환.
		std::vector<std::string> GetMaterialNamesWithoutDefault();

	private:
		//FBX를 들여올 때 임베딩된 텍스쳐 연동 등, 
		//역으로 그래픽스/엔진에서 피할 수 없게 로드 뒤에 추가해야 하는 리소스 존재 -> 호환 위한 함수.
		void CheckForGraphicsToProcessLoad(Pg::Core::IGraphics* graphics);

	private:
		//특정 위치에 쓰일 리소스가 없을 경우 사용되는 디폴트 리소스 로드.
		void LoadDefaultResources();

		//임시로 미리 로드할 리소스들을 로드해놓는다!
		void LoadResourcesFromCSV(const std::string& resourceListPath);

	private:
		Pg::Core::ProcessMain* _coreMain;

		//실제로 보관하는 리소스 맵. <파일 경로 / Asset의 종류>
		std::unordered_map<std::string, Pg::Data::Enums::eAssetDefine> _resourceMap;

		//매 프레임마다 업데이트되는 리소스 페어 (로드 예정)
		std::vector<std::pair<std::string, Pg::Data::Enums::eAssetDefine>> _perFrameToLoadResources;

		//매 프레임마다 업데이트되는 리소스 페어 (언로드 예정)
		std::vector<std::string> _perFrameToUnloadResources;
	};
}
