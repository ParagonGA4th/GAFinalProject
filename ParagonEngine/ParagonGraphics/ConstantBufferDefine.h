#pragma once
#include "../ParagonData/ParagonDefines.h"
#include <DirectXMath.h>

/// <summary>
/// 기본적으로 형태가 정해져서 쓰이는 Constant Buffer에 대응되는 구조체들 정의.
/// </summary>

namespace Pg::Graphics
{
	//constexpr int PG_MAX_BONECOUNT = 256;
	//constexpr int PG_MAX_NODECOUNT = 256;

	class ConstantBufferDefine
	{
	public:
		struct cbPerObjectBase
		{
			DirectX::XMMATRIX gCBuf_World;
			DirectX::XMMATRIX gCBuf_WorldInvTranspose;
			DirectX::XMMATRIX gCBuf_WorldView;
			DirectX::XMMATRIX gCBuf_WorldViewProj;
			DirectX::XMFLOAT3 gCBuf_CameraPositionW;
		};

		struct cbPerObjectSkinnedNodes
		{
			DirectX::XMMATRIX gCBuf_Nodes[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		};

		struct cbPerObjectSkinnedBones
		{
			DirectX::XMMATRIX gCBuf_Bones[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		};

		
		//ObjMat 기록 위해.
		struct cbPerObjMatBase
		{
			DirectX::XMMATRIX gCBuf_World;
			DirectX::XMMATRIX gCBuf_WorldViewProj;
		};

		//같은 구성이므로 하나로 사용.
		//struct cbPerObjMatSkinned
		//{
		//	DirectX::XMMATRIX gCBuf_Bones[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		//};

		struct cbSceneInfo
		{
			DirectX::XMMATRIX gCBuf_ViewMatrix;
			DirectX::XMMATRIX gCBuf_ProjMatrix;
			DirectX::XMFLOAT3 gCBuf_EyePosition;
		};
	};
}
