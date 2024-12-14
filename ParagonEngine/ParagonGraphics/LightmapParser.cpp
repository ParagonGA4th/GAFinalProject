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

		//이제부터 XML을 읽어들인다.
		pugi::xml_node tRoot = doc.child("Root");
		//LightmapList
		{
			pugi::xml_node tLightmapList = tRoot.child("LightmapList");

			//모든 Linear Texture2D* 저장.
			std::vector<RenderTexture2D*> _textureLinearList;
			for (pugi::xml_node tIndiLightmapNode = tLightmapList.first_child();
				tIndiLightmapNode; tIndiLightmapNode = tIndiLightmapNode.next_sibling())
			{
				std::string tLightmapName = tIndiLightmapNode.text().get();

				RenderTexture2D* tFoundTexture = static_cast<RenderTexture2D*>(
					GraphicsResourceManager::Instance()->GetResourceByName(tLightmapName, Data::Enums::eAssetDefine::_TEXTURE2D).get());

				_textureLinearList.push_back(tFoundTexture);
			}

			//값이 Texture를 가져와서 왔을 것이다.
			//ResourceManager의 관리를 받지 않는다.
			tRet->_lightmapTextureArray = std::make_unique<RenderTexture2DArray>(Data::Enums::eAssetDefine::_TEXTURE2DARRAY, lightmapPath);

			//Internal Load대신, 명시적으로 바로 로드함.
			AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();
			//TextureArray 내부 인덱스 == Lightmap ID
			t2DLoader->MultipleRenderTexture2DToTexture2DArray(_textureLinearList.data(), _textureLinearList.size(), tRet->_lightmapTextureArray.get());
		}

		//실제로 Map 관련 상세 정보 연동. 
		pugi::xml_node tRenderObjectList = tRoot.child("RenderObjectList");
		for (pugi::xml_node tRoNode = tRenderObjectList.first_child();
			tRoNode; tRoNode = tRoNode.next_sibling())
		{
			//언리얼이랑 유니티 이름이 같아야 해당됨.
			std::string tObjName = tRoNode.child("ObjectName").text().get();
			//실제 순서대로 매핑해야. -> 별도로 보관했다가 한꺼번에 옮기는 게 나을 듯!
			std::string tMeshName = tRoNode.child("MeshName").text().get();

			//만약 "MeshName" 안에 "shadow" 있으면 라이트맵 굽는 용도로 활용된 것이니 무시.
			if (tMeshName.find("shadow") != std::string::npos || tMeshName.find("Shadow") != std::string::npos)
			{
				//존재한다는 얘기. 해당 내용 기록하지 않고, 다음 노드로.
				continue;
			}

			UINT tLightmapID = std::stoul(tRoNode.child("LightmapIndex").text().get());

			pugi::xml_node tUvScaleNode = tRoNode.child("UVScale");
			DirectX::XMFLOAT2 tUvScale = { std::stof(tUvScaleNode.child("x").text().get()), std::stof(tUvScaleNode.child("y").text().get()) };

			pugi::xml_node tUvOffsetNode = tRoNode.child("UVOffset");
			DirectX::XMFLOAT2 tUvOffset = { std::stof(tUvOffsetNode.child("x").text().get()), std::stof(tUvOffsetNode.child("y").text().get()) };

			//무조건 Insertion이 되어야 한다. Assign될 경우, 이미 있던 요소가 중복으로 사라진 것이다.  
			//그렇기에, 확정지어야.
			// Model Name // Object Name / SingleLightmapSet.
			//없으면 만들기. 있으면 무시.
			tRet->_beforeAlignMaps.try_emplace(tMeshName, std::unordered_map<std::string, SingleLightMapSet>());

			//이제 Model별로 전달된 오브젝트 있을 때, 별개의 unordered_map에서 값을 찾는다.
			auto it = tRet->_beforeAlignMaps.at(tMeshName).insert_or_assign(tObjName, SingleLightMapSet(tUvScale, tUvOffset, tLightmapID));
			//Assign된 것이다. 이러면. 의도되지 않은 동작.
			//if (!it.second)
			//{
			//	assert(false && "이미 있던 요소가 중복으로 사라졌다!");
			//}
		}

		return tRet;
	}
}