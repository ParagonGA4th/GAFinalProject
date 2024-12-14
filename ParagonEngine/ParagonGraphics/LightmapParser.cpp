#include "LightmapParser.h"
#include "../ParagonHelper/pugixml.hpp"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonUtil/CustomAssert.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "AssetBasic2DLoader.h"

#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"

namespace Pg::Graphics
{
	LightmapParser::LightmapParser()
	{

	}

	LightmapParser::~LightmapParser()
	{

	}

	std::unique_ptr<RenderLightmapData> LightmapParser::LoadPgLightmap(const std::string& lightmapPath, const std::string& sceneName)
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		std::unique_ptr<RenderLightmapData> tRet = std::make_unique<RenderLightmapData>(lightmapPath, sceneName);

		pugi::xml_document doc;
		doc.load_file(lightmapPath.c_str());

		//�������� XML�� �о���δ�.
		pugi::xml_node tRoot = doc.child("Root");
		//LightmapList
		{
			pugi::xml_node tLightmapList = tRoot.child("LightmapList");

			//��� Linear Texture2D* ����.
			std::vector<RenderTexture2D*> _textureLinearList;
			for (pugi::xml_node tIndiLightmapNode = tLightmapList.first_child();
				tIndiLightmapNode; tIndiLightmapNode = tIndiLightmapNode.next_sibling())
			{
				std::string tLightmapName = tIndiLightmapNode.text().get();

				RenderTexture2D* tFoundTexture = static_cast<RenderTexture2D*>(
					GraphicsResourceManager::Instance()->GetResourceByName(tLightmapName, Data::Enums::eAssetDefine::_TEXTURE2D).get());

				_textureLinearList.push_back(tFoundTexture);
			}

			//���� Texture�� �����ͼ� ���� ���̴�.
			//ResourceManager�� ������ ���� �ʴ´�.
			tRet->_lightmapTextureArray = std::make_unique<RenderTexture2DArray>(Data::Enums::eAssetDefine::_TEXTURE2DARRAY, lightmapPath);

			//Internal Load���, ��������� �ٷ� �ε���.
			AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
			//TextureArray ���� �ε��� == Lightmap ID
			t2DLoader->MultipleRenderTexture2DToTexture2DArray(_textureLinearList.data(), _textureLinearList.size(), tRet->_lightmapTextureArray.get());
		}

		//������ Map ���� �� ���� ����. 
		pugi::xml_node tRenderObjectList = tRoot.child("RenderObjectList");
		for (pugi::xml_node tRoNode = tRenderObjectList.first_child();
			tRoNode; tRoNode = tRoNode.next_sibling())
		{
			//�𸮾��̶� ����Ƽ �̸��� ���ƾ� �ش��.
			std::string tObjName = tRoNode.child("ObjectName").text().get();
			//���� ������� �����ؾ�. -> ������ �����ߴٰ� �Ѳ����� �ű�� �� ���� ��!
			std::string tMeshName = tRoNode.child("MeshName").text().get();

			//���� "MeshName" �ȿ� "shadow" ������ ����Ʈ�� ���� �뵵�� Ȱ��� ���̴� ����.
			if (tMeshName.find("shadow") != std::string::npos || tMeshName.find("Shadow") != std::string::npos)
			{
				//�����Ѵٴ� ���. �ش� ���� ������� �ʰ�, ���� ����.
				continue;
			}

			UINT tLightmapID = std::stoul(tRoNode.child("LightmapIndex").text().get());

			pugi::xml_node tUvScaleNode = tRoNode.child("UVScale");
			DirectX::XMFLOAT2 tUvScale = { std::stof(tUvScaleNode.child("x").text().get()), std::stof(tUvScaleNode.child("y").text().get()) };

			pugi::xml_node tUvOffsetNode = tRoNode.child("UVOffset");
			DirectX::XMFLOAT2 tUvOffset = { std::stof(tUvOffsetNode.child("x").text().get()), std::stof(tUvOffsetNode.child("y").text().get()) };

			//������ Insertion�� �Ǿ�� �Ѵ�. Assign�� ���, �̹� �ִ� ��Ұ� �ߺ����� ����� ���̴�.  
			//�׷��⿡, Ȯ�������.
			// Model Name // Object Name / SingleLightmapSet.
			//������ �����. ������ ����.
			tRet->_beforeAlignMaps.try_emplace(tMeshName, std::unordered_map<std::string, SingleLightMapSet>());

			//���� Model���� ���޵� ������Ʈ ���� ��, ������ unordered_map���� ���� ã�´�.
			auto it = tRet->_beforeAlignMaps.at(tMeshName).insert_or_assign(tObjName, SingleLightMapSet(tUvScale, tUvOffset, tLightmapID));
			//Assign�� ���̴�. �̷���. �ǵ����� ���� ����.
			//if (!it.second)
			//{
			//	assert(false && "�̹� �ִ� ��Ұ� �ߺ����� �������!");
			//}
		}

		return tRet;
	}
}