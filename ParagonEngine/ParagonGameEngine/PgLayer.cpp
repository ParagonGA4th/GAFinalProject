#include "PgLayer.h"

namespace Pg::Engine
{
	std::map<uint32_t, std::bitset<32>> layerData = {};

	void PgLayer::ValidateLayer(uint32_t _layerID)
	{
		assert(MIN_LAYER <= _layerID && _layerID <= MAX_LAYER && "Invalid layer ID");
	}

	void PgLayer::SetCollisionData(uint32_t _layerID, const std::initializer_list<uint32_t>& _collisionLayers)
	{

	}

	void PgLayer::SetCollisionLayer(uint32_t _layerID, uint32_t _collisionLayerID, bool _value)
	{
		ValidateLayer(_layerID);
		ValidateLayer(_collisionLayerID);

		if (_value)
		{
			layerData[_layerID].set(_collisionLayerID);
			layerData[_collisionLayerID].set(_layerID);
		}
		else
		{
			layerData[_layerID].reset(_collisionLayerID);
			layerData[_collisionLayerID].reset(_layerID);
		}
		
	}

	bool PgLayer::CanCollide(uint32_t _layerID0, uint32_t _layerID1)
	{

	}

	void PgLayer::Clear()
	{
		layerData.clear();
	}

}