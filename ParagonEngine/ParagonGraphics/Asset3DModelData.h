#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/ParagonDefines.h"
#include <array>
#include <vector>

/// <summary>
/// 실제 3DModel과 1대1 대응되는 클래스. 
/// 그래픽 엔진 구상 시 사용으로 World->Screen Space의 인풋 레이아웃이 고정되었기에,
/// 자체적인 VB-IB 연동도 같이 하게 된다. (다른 Material, 다른 시점/애니메이션 스키닝은 RenderObject 딴에서)
/// </summary>

struct ID3D11Buffer;

namespace Pg::Graphics
{
	struct Scene_AssetData;
	struct Skinned_AssetData;
	class MaterialCluster;

	namespace Helper
	{
		class AssimpBufferParser;
	}
	namespace Loader
	{
		class AssetBasic3DLoader;
	}
}

namespace Pg::Graphics
{
	class Asset3DModelData : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Helper::AssimpBufferParser;
		friend class Pg::Graphics::Loader::AssetBasic3DLoader;

	public:
		Asset3DModelData(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~Asset3DModelData();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		bool IsSkinned();
		MaterialCluster* GetMaterialByIndex(short index);


		// 여기서, 실질적으로 Mesh 관련된 정보를 보관하는 
		// 다른 요소 투입 예정. AssetSceneData 대신.

		bool _isSkinned = false;
		Scene_AssetData* _assetSceneData = nullptr;
		Skinned_AssetData* _assetSkinnedData = nullptr;

		//해당 Vector의 인덱스는, 매터리얼의 인덱스와 같다.
		std::vector<MaterialCluster*> _materialClusterList;

		//Vertex Buffer
		ID3D11Buffer* _vertexBuffer;

		//Index Buffer
		ID3D11Buffer* _indexBuffer;
	};
}



