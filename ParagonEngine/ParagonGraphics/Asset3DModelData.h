#pragma once
#include "D3DBufferInfo.h"
#include "TextureCluster.h"

#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/ParagonDefines.h"
#include <array>

/// <summary>
/// 실제 3DModel과 1대1 대응되는 클래스. 
/// 그래픽 엔진 구상 시 사용으로 World->Screen Space의 인풋 레이아웃이 고정되었기에,
/// 자체적인 VB-IB 연동도 같이 하게 된다. (다른 Material, 다른 시점/애니메이션 스키닝은 RenderObject 딴에서)
/// </summary>

struct ID3D11Buffer;

namespace Pg::Graphics
{
	class AssetSceneData;
}

namespace Pg::Graphics
{
	class Asset3DModelData : public Pg::Data::Resources::GraphicsResource
	{
	public:
		Asset3DModelData(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~Asset3DModelData();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		bool _isSkinned = false;
		AssetSceneData* _assetSceneData = nullptr;
		D3DBufferInfo _d3dBufferInfo;
		std::array<TextureCluster, Pg::Defines::MAX_MATERIAL_PER_MODEL> _modelMaterialList;
	private:
	};
}




