#include "MaterialCluster.h"
#include <cassert>
namespace Pg::Graphics
{
	
	std::vector<Pg::Graphics::AssetTextureSRV>& MaterialCluster::GetMaterialATSByIndex(int index)
	{
		assert(index <= Pg::Defines::MAX_MATERIAL_PER_MODEL);
		return _assetSRV[index];
	}

}
