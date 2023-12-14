#pragma once
#include <DirectXMath.h>

/// <summary>
/// 기본적으로 형태가 정해져서 쓰이는 Constant Buffer에 대응되는 구조체들 정의.
/// </summary>

namespace Pg::Graphics
{
	constexpr int PG_MAX_BONECOUNT = 100;

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
			// gCBuf_Materials[10] //후에 Material이 들어가면 추가되어야.
		};

		struct cbPerObjectSkinned 
		{
			DirectX::XMMATRIX gCBuf_Bones[PG_MAX_BONECOUNT];
		};

		struct cbCubemap
		{
			DirectX::XMMATRIX gCBuf_Bones[PG_MAX_BONECOUNT];
		};
	};
}
