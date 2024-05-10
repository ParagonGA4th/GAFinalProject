#include "LightmapManager.h"
#include "../ParagonHelper/CSVHelper.h"
#include "RenderLightmapData.h"
#include "RenderObject3DList.h"

namespace Pg::Graphics
{
	LightmapManager::LightmapManager()
	{
		_lightmapParser = std::make_unique<LightmapParser>();
	}

	LightmapManager::~LightmapManager()
	{

	}

	void LightmapManager::Initialize(const std::string& resourceListPath)
	{
		LoadLightmapCSVList(resourceListPath);
	}

	void LightmapManager::LoadLightmapCSVList(const std::string& resourceListPath)
	{
		std::string tPath = resourceListPath + "/13_PgLightmaps.csv";
		auto tPathVec = Pg::Util::Helper::CSVHelper::ReturnFilePathFromLightmapCSV(tPath);

		for (auto& [xmlPath, sceneName] : tPathVec)
		{
			//라이트맵 데이터를 보관하기 위해서 로드해서 넣는다. 1 Scene = 1 LightmapData
			_lightmapStorage.insert(std::make_pair(sceneName, std::move(_lightmapParser->LoadPgLightmap(xmlPath, sceneName))));
		}
	}

	void LightmapManager::SetGPULightmapDataWithScene(const Pg::Data::Scene* scene, void* renderObjectList)
	{
		std::string sceneName = scene->GetSceneNameConst();
		if (!_lightmapStorage.contains(sceneName))
		{
			//Scene의 이름으로 등록된 Lightmap이 없으면, 아무 행동도 하지 않고 리턴.
			return;
		}

		RenderLightmapData* tRenderLightmapData = _lightmapStorage.at(sceneName).get();
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		//만약 성공했으면 _isAligned가 true면 추가 재정렬 필요 X. 
		if (!(tRenderLightmapData->_isAligned))
		{
			//SingleLightmapSet의 재정렬은 재정렬대로 RenderObject3DList에 정리해서 넣어야 하고 ( 매 프레임 ) 
			//ConstantBuffer에 매번 넣어야 할 것이다.
			//Texture를 실제로 넣는 것은 Scene 단위로 한번, 여기서.

			for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedStaticList)
			{
				//Model 
				//오브젝트 이름 + 
			}
			
			for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
			{

			}




			//Texture2DArray : D3D11.0 기준으로 Elements 512개는 적어도 가능.


			//이제 재정렬이 끝났다!
			tRenderLightmapData->_isAligned = true;
		}

		//실제 tRenderObjectList 내부를 다룰 때 :
		//같은 의미로, 이미 SingleLightmapSet의 벡터 리스트가 비어있으면 건드릴 필요 X.
		//여기서 SingleLightmapSet 넣어야 한다. 같은 인덱스!


	}

}