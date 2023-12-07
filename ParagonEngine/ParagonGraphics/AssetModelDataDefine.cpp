#include "AssetModelDataDefine.h"
#include "../ParagonData/ParagonDefines.h"
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

	Skinned_AssetData::Skinned_AssetData() : _numFormationBone(0)
	{
		//Bone의 수만큼 GPU에 들어갈 벡터의 크기를 설정해야 한다. (ASSET_MAXIMUM_BONE_NUMBER_PER_MESH)
		_boneTransformVector.resize(Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH);
		std::fill(_boneTransformVector.begin(), _boneTransformVector.end(), DirectX::SimpleMath::Matrix::Identity);
	
		//우선적으로, Global Inverse Transform Vector 초기화.
		_meshGlobalInverseTransform = DirectX::SimpleMath::Matrix::Identity;
	}

}