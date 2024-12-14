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
			//����Ʈ�� �����͸� �����ϱ� ���ؼ� �ε��ؼ� �ִ´�. 1 Scene = 1 LightmapData
			_lightmapStorage.insert(std::make_pair(sceneName, std::move(_lightmapParser->LoadPgLightmap(xmlPath, sceneName))));
		}
	}



	void LightmapManager::SetGPULightmapDataWithScene(const Pg::Data::Scene* scene, void* renderObjectList)
	{
		std::string sceneName = scene->GetSceneNameConst();
		sceneName.append(".pgscene");
		if (!_lightmapStorage.contains(sceneName))
		{
			//Scene�� �̸����� ��ϵ� Lightmap�� ������, �ƹ� �ൿ�� ���� �ʰ� ����.
			_isSceneUseLightmap = false;

			return;
		}

		//����Ʈ���� ����Ѵٰ� ǥ��.
		_isSceneUseLightmap = true;

		//������ �� ���������� �ְ� �ȴ�.
		RenderLightmapData* tRenderLightmapData = _lightmapStorage.at(sceneName).get();
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		//Texture2DArray : D3D11.0 �������� Elements 512���� ��� ����.
		//internal_LightmapArray : t2�� �ø���.

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &tNullSRV);
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &(tRenderLightmapData->_lightmapTextureArray->GetSRV()));

		//���� ���������� _isAligned�� true�� �߰� ������ �ʿ� X. 
		//if (!(tRenderLightmapData->_isAligned))
		//{
			//SingleLightmapSet�� �������� �����Ĵ�� RenderObject3DList�� �����ؼ� �־�� �ϰ� ( �� ������ ) 
			//ConstantBuffer�� �Ź� �־�� �� ���̴�.
			//Texture�� ������ �ִ� ���� Scene ������ �ѹ�, ���⼭.
			//����, ������ ���� �ִ� ���� Instanced / CulledOppositeInstanced�� �ε����� �޷� �ִ� ���̴�,
			//������ ���� ��. ���� �����ٰ� ���� �߻�X!

		for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedStaticList)
		{

			//���� "MeshName" �ȿ� "shadow" ������ ����Ʈ�� ���� �뵵�� Ȱ��� ���̴� ����.
			if (bModel->GetFileName().find("shadow") != std::string::npos || bModel->GetFileName().find("Shadow") != std::string::npos)
			{
				//�����Ѵٴ� ���. �ش� ���� ������� �ʰ�, ���� ����.
				continue;
			}

			//Model ����.
			//SV_InstanceID�� ���� VB���� Vertex����ü �ϳ� ���� ������ ���� ���̴�.
			//������ ���߱�.
			//�ش� ���� : Model �����̰�,  SV_InstanceID�� �ش��� ���̴�.
			assert((bInstancedPairList->_instancedStaticPairVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING) && "���ѵ� �ν��Ͻ� ī��Ʈ �ɰ�!");
			bInstancedPairList->_instancedLightMapSetVec.resize(bInstancedPairList->_instancedStaticPairVec.size());


			//���� ���������� SV_InstanceID�� ���� ������ �־��ش�.
			for (int i = 0; i < bInstancedPairList->_instancedLightMapSetVec.size(); i++)
			{
				Pg::Data::BaseRenderer* tBaseRenderer = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer();
				Pg::Data::StaticMeshRenderer* tStaticRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);
				std::string tModelName = Pg::Util::Helper::ResourceHelper::GetNameFromPath(tStaticRenderer->GetMeshFilePath());
				std::string tObjName = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer()->_object->GetName();

				//�̸� ������� ���� ã��!
				try
				{
					//�̸� ������� ���� ã��!	
					SingleLightMapSet& toBeCopiedTo = bInstancedPairList->_instancedLightMapSetVec.at(i);

					SingleLightMapSet* toBeCopiedFrom = nullptr;
					if (tRenderLightmapData->_beforeAlignMaps.at(tModelName).contains(tObjName))
					{
						toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
					}
					else
					{
						if (tObjName.find("nocol_") != std::string::npos)
						{
							//nocol_�� �����ϰ� ã�ƺ���.
							tObjName = tObjName.substr(6);
							toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
						}
						else if (tObjName.substr(tObjName.length() - 3) == ".00")
						{
							tObjName = tObjName.substr(0, tObjName.length() - 3);
							toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
						}
						else
						{
							PG_ERROR("{0}�� nocol_ / .00 ������ �ƴϴ�.", tObjName);
						}
					}
					//SingleLightMapSet& toBeCopiedFrom = tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName);

					if (toBeCopiedFrom != nullptr)
					{
						//POD�� memcpy ����.
						memcpy(&toBeCopiedTo, toBeCopiedFrom, sizeof(SingleLightMapSet));
					}
					else
					{
						PG_ERROR("Not Normal");
					}
				}
				catch (std::exception& e)
				{
					PG_ERROR("{0}�� Scene/Lightmap �� �ϳ��� ����.", tObjName);
				}
				
			}
		}

		for (auto& [bModel, bInstancedPairList] : tRenderObjectList->_instancedCulledOppositeStaticList)
		{
			//Model ����.
			//SV_InstanceID�� ���� VB���� Vertex����ü �ϳ� ���� ������ ���� ���̴�.
			//������ ���߱�.
			//�ش� ���� : Model �����̰�,  SV_InstanceID�� �ش��� ���̴�.
			assert((bInstancedPairList->_instancedStaticPairVec.size() <= Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING) && "���ѵ� �ν��Ͻ� ī��Ʈ �ɰ�!");
			bInstancedPairList->_instancedLightMapSetVec.resize(bInstancedPairList->_instancedStaticPairVec.size());


			//���� ���������� SV_InstanceID�� ���� ������ �־��ش�.
			for (int i = 0; i < bInstancedPairList->_instancedLightMapSetVec.size(); i++)
			{
				Pg::Data::BaseRenderer* tBaseRenderer = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer();
				Pg::Data::StaticMeshRenderer* tStaticRenderer = static_cast<Pg::Data::StaticMeshRenderer*>(tBaseRenderer);
				std::string tModelName = Pg::Util::Helper::ResourceHelper::GetNameFromPath(tStaticRenderer->GetMeshFilePath());
				std::string tObjName = bInstancedPairList->_instancedStaticPairVec.at(i)._instancedRenderObject->GetBaseRenderer()->_object->GetName();

				try
				{
					//�̸� ������� ���� ã��!	
					SingleLightMapSet& toBeCopiedTo = bInstancedPairList->_instancedLightMapSetVec.at(i);

					SingleLightMapSet* toBeCopiedFrom = nullptr;
					if (tRenderLightmapData->_beforeAlignMaps.at(tModelName).contains(tObjName))
					{
						toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
					}
					else
					{
						if (tObjName.find("nocol_") != std::string::npos)
						{
							//nocol_�� �����ϰ� ã�ƺ���.
							tObjName = tObjName.substr(6);
							toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
						}
						else if (tObjName.substr(tObjName.length() - 3) == ".00")
						{
							tObjName = tObjName.substr(0, tObjName.length() - 3);
							toBeCopiedFrom = &(tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName));
						}
						else
						{
							PG_ERROR("{0}�� nocol_ / .00 ������ �ƴϴ�.", tObjName);
						}
					}
					//SingleLightMapSet& toBeCopiedFrom = tRenderLightmapData->_beforeAlignMaps.at(tModelName).at(tObjName);
						
					if (toBeCopiedFrom != nullptr)
					{
						//POD�� memcpy ����.
						memcpy(&toBeCopiedTo, toBeCopiedFrom, sizeof(SingleLightMapSet));
					}
					else
					{
						PG_ERROR("Not Normal");
					}

					////POD�� memcpy ����.
					//memcpy(&toBeCopiedTo, toBeCopiedFrom, sizeof(SingleLightMapSet));
				}
				catch (std::exception& e)
				{
					PG_ERROR("{0}�� Scene/Lightmap �� �ϳ��� ����.", tObjName);
				}
			}
		}

		assert("");
		//���� �������� ������!
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
		//������ �� ���������� �ְ� �ȴ�.
		RenderLightmapData* tRenderLightmapData = _lightmapStorage.at(sceneName).get();

		//Texture2DArray : D3D11.0 �������� Elements 512���� ��� ����.
		//internal_LightmapArray : t2�� �ø���.

		ID3D11ShaderResourceView* tNullSRV = nullptr;
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &tNullSRV);
		LowDX11Storage::GetInstance()->_deviceContext->PSSetShaderResources(2, 1, &(tRenderLightmapData->_lightmapTextureArray->GetSRV()));
	}



}