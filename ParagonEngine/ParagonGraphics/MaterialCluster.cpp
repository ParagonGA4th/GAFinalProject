#include "MaterialCluster.h"
//#include "AssimpBufferParser.h"
#include <cassert>
#include <d3d11.h>

namespace Pg::Graphics
{
	MaterialCluster::MaterialCluster() : _atsList()
	{
		_atsList.fill(nullptr);

		//for (int i = 0; i < _debugList.size(); i++)
		//{
		//	//it.first = 
		//	_debugList[i].first = (eAssetTextureType)i;
		//	_debugList[i].second = false;
		//}
	}

	Pg::Graphics::RenderTexture2D* MaterialCluster::GetTextureByType(eAssetTextureType type)
	{
		//없으면 nullptr 반환, 있으면 RenderTexture2D* 반환.
		return _atsList[type];
	}


	

}
