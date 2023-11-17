#include "MaterialCluster.h"
#include <cassert>
#include <d3d11.h>

namespace Pg::Graphics
{
	MaterialCluster::MaterialCluster() : _atsList()
	{
		_atsList.fill(nullptr);
	}

	Pg::Graphics::RenderTexture2D* MaterialCluster::GetTextureByType(eAssetTextureType type)
	{
		//없을 경우 nullptr, 있으면 RenderTexture2D*를 반환한다.
		return _atsList[type];
	}


	

}
