#include "LightmapManager.h"
#include "../ParagonHelper/CSVHelper.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonData/ParagonDefines.h"
#include "RenderLightmapData.h"
#include "RenderObject3DList.h"
#include "LowDX11Storage.h"
#include "Asset3DModelData.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonUtil/Log.h"

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
		sceneName.append(".pgscene");
		if (!_lightmapStorage.contains(sceneName))
		{
			//Scene의 이름으로 등록된 Lightmap이 없으면, 아무 행동도 하지 않고 리턴.
			_isSceneUseLightmap = false;

			return;
		}

		//라이트맵을 사용한다고 표시.
		_isSceneUseLightmap = true;

		//무조건 이 시점에서는 있게 된다.
		RenderLightmapData* tRenderLightmapData = _lightmapStorage.at(sceneName).get();
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		//Texture2DArray : D3D11.0 기준으로 Elements 512개는 적어도 가능.
		//internal_LightmapArray : t2에 올린다.

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &tNullSRV);
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &(tRenderLightmapData->_lightmapTextureArray->GetSRV()));

		//만약 성공했으면 _isAligned가 true면 추가 재정렬 필요 X. 
		//if (!(tRenderLightmapData->_isAligned))
		//{
			//SingleLightmapSet의 재정렬은 재정렬대로 RenderObject3DList에 정리해서 넣어야 하고 ( 매 프레임 ) 
			//ConstantBuffer에 매번 넣어야 할 것이다.
			//Texture를 실제로 넣는 것은 Scene 단위로 한번, 여기서.
			//또한, 실제로 값을 넣는 것은 Instanced / CulledOppositeInstanced의 인덱스에 달려 있는 것이니,
			//문제가 없을 것. 벡터 나눈다고 문제 발생X!

		for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedStaticList)
		{

			//만약 "MeshName" 안에 "shadow" 있으면 라이트맵 굽는 용도로 활용된 것이니 무시.
			if (bModel->GetFileName().find("shadow") != std::string::npos || bModel->GetFileName().find("Shadow") != std::string::npos)
			{
				//존재한다는 얘기. 해당 내용 기록하지 않고, 다음 노드로.
				continue;
			}

			//Model 개별.
			//SV_InstanceID는 개별 VB에서 Vertex구조체 하나 지날 때마다 나온 것이다.
			//사이즈 맞추기.
			//해당 벡터 : Model 각각이고,  SV_InstanceID와 해당할 것이다.
			assert((bInstancedPairList->_instancedStaticPairVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING) && "제한된 인스턴싱 카운트 능가!");
			bInstancedPairList->_instancedLightMapSetVec.resize(bInstancedPairList->_instancedStaticPairVec.size());


			//이제 개별적으로 SV_InstanceID에 따른 정보를 넣어준다.
			for (int i = 0; i < bInstancedPairList->_instancedLightMapSetVec.size(); i++)
			{
				Pg::Data::BaseRenderer* tBaseRenderer = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer();
				Pg::Data::StaticMeshRenderer* tStaticRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);
				std::string tModelName = Pg::Util::Helper::ResourceHelper::GetNameFromPath(tStaticRenderer->GetMeshFilePath());
				std::string tObjName = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer()->_object->GetName();

				//이를 기반으로 값을 찾자!
				SingleLightMapSet& toBeCopiedTo = bInstancedPairList->_instancedLightMapSetVec.at(i);
				SingleLightMapSet& toBeCopiedFrom = tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName);

				//POD니 memcpy 가능.
				memcpy(&toBeCopiedTo, &toBeCopiedFrom, sizeof(SingleLightMapSet));
			}
		}

		for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
		{
			//Model 개별.
			//SV_InstanceID는 개별 VB에서 Vertex구조체 하나 지날 때마다 나온 것이다.
			//사이즈 맞추기.
			//해당 벡터 : Model 각각이고,  SV_InstanceID와 해당할 것이다.
			assert((bInstancedPairList->_instancedStaticPairVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING) && "제한된 인스턴싱 카운트 능가!");
			bInstancedPairList->_instancedLightMapSetVec.resize(bInstancedPairList->_instancedStaticPairVec.size());


			//이제 개별적으로 SV_InstanceID에 따른 정보를 넣어준다.
			for (int i = 0; i < bInstancedPairList->_instancedLightMapSetVec.size(); i++)
			{
				Pg::Data::BaseRenderer* tBaseRenderer = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer();
				Pg::Data::StaticMeshRenderer* tStaticRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);
				std::string tModelName = Pg::Util::Helper::ResourceHelper::GetNameFromPath(tStaticRenderer->GetMeshFilePath());
				std::string tObjName = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer()->_object->GetName();

				//이를 기반으로 값을 찾자!	
				SingleLightMapSet& toBeCopiedTo = bInstancedPairList->_instancedLightMapSetVec.at(i);
				SingleLightMapSet& toBeCopiedFrom = tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName);

				//POD니 memcpy 가능.
				memcpy(&toBeCopiedTo, &toBeCopiedFrom, sizeof(SingleLightMapSet));
			}
		}
		//이제 재정렬이 끝났다!
		//tRenderLightmapData->_isAligned = true;
	//}
	}

	void LightmapManager::Temp(const Pg::Data::Scene* scene)
	{
		std::string sceneName = scene->GetSceneNameConst();
		sceneName.append(".pgscene");
		if (!_lightmapStorage.contains(sceneName))
		{
			return;
		}
		//무조건 이 시점에서는 있게 된다.
		RenderLightmapData* tRenderLightmapData = _lightmapStorage.at(sceneName).get();

		//Texture2DArray : D3D11.0 기준으로 Elements 512개는 적어도 가능.
		//internal_LightmapArray : t2에 올린다.

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &tNullSRV);
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &(tRenderLightmapData->_lightmapTextureArray->GetSRV()));
	}



}