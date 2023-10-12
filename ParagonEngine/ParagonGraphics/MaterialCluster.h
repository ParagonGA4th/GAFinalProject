#pragma once
#include "AssetStructSRV.h"
#include "../ParagonData/ParagonDefines.h"

#include <array>
#include <vector>
/// <summary>
/// FBX 내부에 임베딩되어 있는 텍스쳐를 빼서 보관한다.
/// </summary>

namespace Pg::Graphics
{
	class MaterialCluster
	{
	public:
		//Material의 인덱스(N번 인덱스의 Material)와 1대1로 대응할 값의 참조를 가져온다.
		std::vector<AssetTextureSRV>& GetMaterialATSByIndex(int index);

		//숫자를 그대로 쓰는 것은 힘들다. 단순 숫자 enum으로 어떤 Resource가 어디로 대응하는지
		//가리킬 수 있어야 한다.!
		//ID3D11ShaderResourceView* GetMaterialSRV();

	private:
		//AssetMaterialData의 인덱스랑 1대1 대응할 것이다.
		std::array<std::vector<AssetTextureSRV>, Pg::Defines::MAX_MATERIAL_PER_MODEL> _assetSRV;
	};
}


