#include "AssetModelDataDefine.h"

namespace Pg::Graphics
{
	Node_AssetData::Node_AssetData(Node_AssetData* parentNode)
	{
		//Parent 할당.
		this->_parentNode = parentNode;

		//그냥 리셋.
		this->_relTransform = DirectX::XMMatrixIdentity();
	}

	Node_AssetData::~Node_AssetData()
	{

	}

	Bone_AssetData::Bone_AssetData() :
		_name(), _numWeights(0), _offsetMatrix(DirectX::SimpleMath::Matrix::Identity)
	{

	}

}