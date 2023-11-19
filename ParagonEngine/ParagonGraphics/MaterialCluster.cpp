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
		//???? ??? nullptr, ?????? RenderTexture2D*?? ??????.
		return _atsList[type];
	}


	

}
