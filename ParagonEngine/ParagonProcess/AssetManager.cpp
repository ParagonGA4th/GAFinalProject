#include "AssetManager.h"
#include "ProcessMain.h"
#include "IEngine.h"
#include "IGraphics.h"

#include "../ParagonGraphics/GraphicsResourceManager.h"
#include "../ParagonGameEngine/EngineResourceManager.h"

#include "../ParagonUtil/ResourceHelper.h"

//<ResourcesList>
#include "../ParagonGraphics/RenderMaterial.h"
//</ResourcesList>

#include <algorithm>
#include <cassert>

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
		//CreateResource를 임시로 여기에 호출.
		//LoadResource("../Resources/3DModels/TexturedMultiCubes/TexturedMultiCubeMultiMesh.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/TexturedMultiCubes/TexturedMultiCubeMultiMeshSeams.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/TexturedMultiCubes/TMultiCube_test001.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/MultiMatMesh/LavaWoodCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/MultiMatMesh/RoadLavaCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/3DModels/MultiMatMesh/WoodRoadCone.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/SkinnedExample.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/Boss_Test_Deform.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/Boss_Test_NonDeform.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/BasicAnimatedCubeEdited.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/Boss_Test_NonDeform_MultiMat.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		//LoadResource("../Resources/3DModels/Animated/test_timmy003.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		LoadResource("../Resources/Textures/LPDisk.jpg", Pg::Data::Enums::eAssetDefine::_2DTEXTURE);
		LoadResource("../Resources/Fonts/NotoSansKR_16.spritefont", Pg::Data::Enums::eAssetDefine::_FONT);
		//LoadResource("../Resources/3DModels/MultiMatMesh/diffuseonly.fbx", Pg::Data::Enums::eAssetDefine::_3DMODEL);
		
	}

}
