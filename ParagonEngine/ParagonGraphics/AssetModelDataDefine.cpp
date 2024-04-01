#include "AssetModelDataDefine.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/Transform.h"

namespace Pg::Graphics
{
	Node_AssetData::Node_AssetData(Node_AssetData* parentNode) : _parentNode(parentNode)
	{
		//Parent 이미 할당함.
	}

	Node_AssetData::~Node_AssetData()
	{
		//
	}

	Skinned_AssetData::Skinned_AssetData() :_meshGlobalInverseTransform(DirectX::SimpleMath::Matrix::Identity)
	{
		//
	}

}