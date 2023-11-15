#pragma once
#include "AssetTextureType.h"
#include "../ParagonData/ParagonDefines.h"

#include <string>
#include <array>
#include <vector>

/// <summary>
/// FBX 내부에 임베딩되어 있는 텍스쳐를 빼서 보관한다. 
/// 나중에 내용물은 바뀔 수도 있다. 에디터에서 Texture만 바뀌는 경우,
/// 내용물이 대체된다.
/// 
/// 각각 하나의 Material에 대응!
/// </summary>

struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTexture2D;
}

namespace Pg::Graphics
{
	class MaterialCluster
	{
		friend class Pg::Graphics::Helper::AssimpBufferParser;

	public:
		MaterialCluster();
		////AssetTextureType에 기반해서 해당하는 Texture SRV를 반환한다.
		//ID3D11ShaderResourceView* GetTextureSrvByType(eAssetTextureType type);
		////AssetTextureType에 기반해서 해당하는 Texture Path를 반환한다.
		//std::string GetTexturePathByType(eAssetTextureType type);

		//나중에는 Path만 특정 Type으로 넣어서 Map을 바꿀 수 있는 세터가 있어야 한다.

	private:
		//각 인덱스가 AssetTextureType과 1대1 대응한다.
		std::array<RenderTexture2D*, Pg::Defines::MAX_MATERIAL_PER_MODEL> _atsList;
	};
}


