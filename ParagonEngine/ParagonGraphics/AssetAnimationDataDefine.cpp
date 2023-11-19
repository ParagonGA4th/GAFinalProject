#include "AssetAnimationDataDefine.h"

namespace Pg::Graphics
{
	NodeAnim_AssetData::NodeAnim_AssetData()
	{

	}

	NodeAnim_AssetData::~NodeAnim_AssetData()
	{

	}

	BoneInfo_AssetData::BoneInfo_AssetData()
	{
		_boneOffset = DirectX::SimpleMath::Matrix::Identity;
		_finalTransformation = DirectX::SimpleMath::Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}



}