#include "AssetManager.h"
#include "ProcessMain.h"
#include "IEngine.h"
#include "IGraphics.h"

#include "../ParagonData/ParagonDefines.h"
#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonUtil/ResourceHelper.h"
#include "../ParagonUtil/Log.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <algorithm>
#include <filesystem>
#include <cassert>
#include <set>
#include <iterator>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

namespace Pg::Core::Manager
{
	using Pg::Util::Helper::ResourceHelper;

	AssetManager::AssetManager()
	{
		//
	}

	AssetManager::~AssetManager()
	{

	}

	void AssetManager::Initialize(Pg::Core::ProcessMain* core) 
	{
		_coreMain = core;
		_perFrameToLoadResources.reserve(30);
		_perFrameToUnloadResources.reserve(30);

		//디폴트 리소스들이 로드되는 함수. 필수!
		LoadDefaultResources();

		//Scene 단위 리소스 연동이 완료되지 않으면, 여기서 로드되지 않은 리소스들은 사용되지 못함.
		TemporaryLoadResources();
	}

	void AssetManager::Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics)
	{
		//실제 Loading 로직.
		for (auto& it : _perFrameToLoadResources)
		{
			if (Pg::Util::Helper::ResourceHelper::IsGraphicsResource(it.second))
			{
				graphics->LoadResource(it.first, it.second);
			}
			else
			{
				engine->LoadResource(it.first, it.second);
			}
			
			//실제로 로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//실제 Unloading 로직.
		for (auto& it : _perFrameToUnloadResources)
		{
			graphics->UnloadResource(it);
			engine->UnloadResource(it);

			//실제로 언로드되었으면, AssetManager의 목록에 연동해서 관리해야 한다.
			this->_resourceMap.erase(it);
		}

		//다음 Iteration을 위해 Clear.
		_perFrameToLoadResources.clear();
		_perFrameToUnloadResources.clear();

		//2차 발생 리소스들이 Graphics에서 나오지 않았다 체크해준다.
		CheckForGraphicsToProcessLoad(graphics);
	}

	void AssetManager::AssureNoNameDuplicates()
	{
		std::vector<std::string> _pathNamesVector;
		_pathNamesVector.reserve(_resourceMap.size());

		//std::set
		for (auto& [bPathName, bAssetDefine] : _resourceMap)
		{
			std::filesystem::path tPath = bPathName;
			std::string tName = tPath.filename().generic_string();
			assert(!tName.empty());

			_pathNamesVector.push_back(tName);
		}
		std::sort(_pathNamesVector.begin(), _pathNamesVector.end());
		std::vector<std::string>::iterator it = std::unique(_pathNamesVector.begin(), _pathNamesVector.end());

		//중복 요소가 있는지 확인.
		bool tWasUnique = (it == _pathNamesVector.end());


		PG_WARN("Main 브랜치와 합친 뒤, 해당 줄의 리소스 중복 검사 로직 활성화해야.");
		//if (!tWasUnique)
		//{
		//	//assert false 하기 전에, 오류 코드 등 출력.
		//	std::set<std::string> _tempDupNameResourceSet;
		//
		//	//마지막으로 중복되지 않은 Iterator
		//	auto tRend = std::make_reverse_iterator(std::prev(it));
		//	for (auto endIt = _pathNamesVector.rbegin(); endIt != tRend; endIt++)
		//	{
		//		_tempDupNameResourceSet.insert(*endIt);
		//	}
		//
		//	//std::unique 자체로 생기는 "" 제거. (크기를 바꾸지 않기 때문에)
		//	_tempDupNameResourceSet.erase("");
		//	
		//	std::string tDebugLogString = "Duplicates : \n";
		//	for (auto& itt : _tempDupNameResourceSet)
		//	{
		//		tDebugLogString += " < ";
		//		tDebugLogString += itt;
		//		tDebugLogString += " > ";
		//	}
		//
		//	tDebugLogString += "\n";
		//	tDebugLogString += "Total Repeated Count : ";
		//	tDebugLogString += std::to_string(_tempDupNameResourceSet.size());
		//
		//	PG_ERROR(tDebugLogString.c_str());
		//
		//	assert(false && "여기서 걸리면 중복되는 파일 이름을 가진 다른 경로의 리소스들이 있다는 것이다. 제거해야.");
		//}
	}

	bool AssetManager::IsExistResource(const std::string& filepath)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		auto res = std::find_if(_resourceMap.begin(), _resourceMap.end(),
			[&tFilePath](const std::pair<std::string, Pg::Data::Enums::eAssetDefine>& val)
			-> bool {return (val.first == tFilePath); });

		bool tIsFound = (res != _resourceMap.end()) ? true : false;
		return tIsFound;
	}

	void AssetManager::LoadResource(const std::string& filepath, Pg::Data::Enums::eAssetDefine define)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//Load하기 전에, 이미 목록에 없는지 체크!
		if (!IsExistResource(tFilePath))
		{
			auto tInfo = std::make_pair(tFilePath, define);
			this->_perFrameToLoadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::UnloadResource(const std::string& filepath)
	{
		std::string tFilePath = Pg::Util::Helper::ResourceHelper::ForcePathUniform(filepath);

		//Unload하기 전에, 이미 목록에 있는지 체크!
		if (IsExistResource(tFilePath))
		{
			auto tInfo = tFilePath;
			this->_perFrameToUnloadResources.emplace_back(tInfo);
		}
	}

	void AssetManager::CheckForGraphicsToProcessLoad(Pg::Core::IGraphics* graphics)
	{
		auto tSecondaryResources = graphics->SendAddedSecondaryResources();

		if (tSecondaryResources == nullptr)
		{
			return;
		}

		//만약 있을 경우, 리스트에 목록을 추가한다.
		for (auto& it : *tSecondaryResources)
		{
			//기존에 키가 없을 경우만 넣어준다.
			_resourceMap.insert(std::make_pair(it.first, it.second));
		}

		//그래픽스 Secondary List 클리어.
		graphics->ClearSecondaryResourcesList();
	}

	void AssetManager::TemporaryLoadResources()
	{
		using Pg::Data::Enums::eAssetDefine;

		//CreateResource를 임시로 여기에 호출.

	

		//현재 파이프라인에는 요구되지 않으나, 리플렉션을 보기 위해.
		//LoadResource("../Builds/x64/Debug/AppendTestVS.cso", eAssetDefine::_RENDER_VERTEXSHADER);
		
		LoadResource("../Resources/3DModels/StaticMesh/LavaWoodCone/LavaWoodCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/JustCopyCube/JustCopyCube.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/SimpleCube/simplecube.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/twcylinder/twcylinder.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/RoadLavaCone/RoadLavaCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/WoodRoadCone/WoodRoadCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/StaticMesh/TwoRoadWoodTorus/TwoRoadWoodTorus.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/BasicMesh/Cube/Cube.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/BasicMesh/Sphere/Sphere.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/BasicMesh/Capsule/Capsule.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/BasicMesh/Plane/plane.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/Textures/tw_normal.png", Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		LoadResource("../Resources/Textures/tw_diffuse.png", Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		LoadResource("../Resources/Textures/wook.jpg", Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		LoadResource("../Resources/Fonts/NotoSansKR_16.spritefont", Pg::Data::Enums::eAssetDefine::_FONT);
		LoadResource("../Resources/Fonts/NotoSansKR_13.spritefont", Pg::Data::Enums::eAssetDefine::_FONT);

		//ShaderMaterial로 추가한 요소들.
		//LoadResource("../Resources/3DModels/StaticMesh/RoadLavaCone/RoadLavaCone.fbm/road_1_diffuseOriginal.png", Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		//LoadResource("../Resources/3DModels/StaticMesh/RoadLavaCone/RoadLavaCone.fbm/lava_1_diffuseOriginal.png", Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
		LoadResource("../Builds/x64/Debug/AppendObjectTestVS.cso", eAssetDefine::_RENDER_VERTEXSHADER);
		LoadResource("../Builds/x64/Debug/AppendObjectTestPS.cso", eAssetDefine::_RENDER_PIXELSHADER);
		LoadResource("../ShaderResources/Materials/RoadLavaConeTestMat.pgmat", Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL);
		LoadResource("../Resources/Textures/room.dds", eAssetDefine::_CUBEMAP);
		//LoadResource("../Resources/3DModels/AnimMesh/twcylinder/twcylinder.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
	}

	void AssetManager::LoadDefaultResources()
	{
		using Pg::Data::Enums::eAssetDefine;

		LoadResource(Pg::Defines::ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH, eAssetDefine::_TEXTURE2D);
		LoadResource(Pg::Defines::ASSET_DEFAULT_NORMAL_TEXTURE_PATH, eAssetDefine::_TEXTURE2D);
	}



}
