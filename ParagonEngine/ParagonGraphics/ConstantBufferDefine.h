#pragma once
#include "../ParagonData/ParagonDefines.h"
#include "IndividualLightsGPU.h"
#include "SingleLightmapSet.h"
#include <DirectXMath.h>

/// <summary>
/// �⺻������ ���°� �������� ���̴� Constant Buffer�� �����Ǵ� ����ü�� ����.
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
			uint32_t gCBuf_ObjID;
			uint32_t gCBuf_MatID;
		};

		struct cbPerObjectSkinnedNodes
		{
			DirectX::XMMATRIX gCBuf_Nodes[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		};

		struct cbPerObjectSkinnedBones
		{
			DirectX::XMMATRIX gCBuf_Bones[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		};

		//ObjMat ��� ����.
		struct cbPerObjMatBase
		{
			DirectX::XMMATRIX gCBuf_World;
		};

		//Alpha ����, VS / PS ���� ���ε�.
		struct cbAlphaDefault
		{
			DirectX::XMMATRIX gCBuf_CameraViewProj; //VP
			float gCBuf_ObjectAlpha; //������Ʈ ���� ���� �ø���, 0-1.
			DirectX::XMFLOAT3 gCBuf_LeftPadding; // �е�.
		};

		//���� �����̹Ƿ� �ϳ��� ���.
		//struct cbPerObjMatSkinned
		//{
		//	DirectX::XMMATRIX gCBuf_Bones[Pg::Defines::ASSET_MAXIMUM_BONE_NUMBER_PER_MESH];
		//};

		struct cbSceneInfo
		{
			DirectX::XMMATRIX gCBuf_ViewMatrix;
			DirectX::XMMATRIX gCBuf_ProjMatrix;
			DirectX::XMMATRIX gCBuf_ViewProjMatrix;
			DirectX::XMMATRIX gCBuf_InvViewMatrix;

			DirectX::XMFLOAT2 gCBuf_ScreenWidthHeight;
			float gCBuf_RadianTimeLoop{ 0.f };
			int gCBuf_OnlyShowLMIndex;

			DirectX::XMFLOAT3 gCBuf_EyePosition;
			int gCBuf_IsSceneUseLightmap;

			//HLSL������ Bool, C++������ int�� ���ε�.
			int gCBuf_isVignetteOn;
			int gCBuf_isBloomOn;
			int gCBuf_isLutOn;
			int gCBuf_isMotionBlurOn;
		};

		struct cbRenderingInfo
		{
			float _Camera_NearPlane;
			float _Camera_FarPlane;
			float tPadding1;
			float tPadding2;

			DirectX::XMMATRIX _lightView;
			DirectX::XMMATRIX _lightProj;
			DirectX::XMMATRIX _lightViewProj;

			DirectX::XMFLOAT3 _indep_MainLightDir;
			float _indep_MainLightRadiance;

			DirLightGPU _dirLightArray[10];
			unsigned int _dirLightCount;
			unsigned int _pad1[3];

			SpotLightGPU _spotLightArray[10];
			unsigned int _spotLightCount;
			unsigned int _pad2[3];

			PointLightGPU _pointLightArray[10];
			unsigned int _pointLightCount;
			unsigned int _pad3[3];
		};

		struct SingleObjMatIdSet
		{
			unsigned int objID;
			unsigned int matID;
		};

		struct cbLightmapCollection
		{
			SingleLightMapSet gBuf_LightMapSet[Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING];
		};

		struct cbSwitchableViewProj
		{
			DirectX::XMMATRIX _viewProj;
		};
		//struct cbObjMatIDCollection
		//{
		//	SingleObjMatIdSet gBuf_ObjMatIdSet[Pg::Defines::MAXIMUM_OBJECT_COUNT_PER_INSTANCING];
		//};
	};
}
