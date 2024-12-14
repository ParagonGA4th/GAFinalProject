#include "LayoutDefine.h"
#include "LowDX11Storage.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonData/ParagonDefines.h"
#include "../ParagonHelper/ResourceHelper.h"

#include <d3d11.h>
#include <fstream>
#include <dxtk/VertexTypes.h>
#include <cassert>
#include <vector>

//#ifdef _DEBUG
//#define PG_1ST_STATIC_SHADER_PATH				L"..\\Builds\\x64\\Debug\\FirstStatic_VS.cso"
//#define PG_1ST_SKINNED_SHADER_PATH				L"..\\Builds\\x64\\Debug\\FirstSkinned_VS.cso"
//#define PG_DEFERRED_QUAD_SHADER_PATH			L"..\\Builds\\x64\\Debug\\FinalStage_VS.cso"
//#define PG_PRIMITIVE_SHADER_PATH				L"..\\Builds\\x64\\Debug\\PrimitiveVS.cso"
//#define PG_CUBEMAP_SHADER_PATH					L"..\\Builds\\x64\\Debug\\CubemapVS.cso"
//#define PG_INDIVIDUAL_PER_OBJMAT_STATIC_PATH	L"..\\Builds\\x64\\Debug\\Individual_PerObjMatStaticVS.cso"
//#define PG_INDIVIDUAL_PER_OBJMAT_SKINNED_PATH	L"..\\Builds\\x64\\Debug\\Individual_PerObjMatSkinnedVS.cso"
//#define PG_1ST_INSTANCED_SHADER_PATH			L"..\\Builds\\x64\\Debug\\FirstInstanced_VS.cso"
//#else
//#define PG_1ST_STATIC_SHADER_PATH				L"..\\Builds\\x64\\Release\\FirstStatic_VS.cso"
//#define PG_1ST_SKINNED_SHADER_PATH				L"..\\Builds\\x64\\Release\\FirstSkinned_VS.cso"
//#define PG_DEFERRED_QUAD_SHADER_PATH			L"..\\Builds\\x64\\Release\\FinalStage_VS.cso"
//#define PG_PRIMITIVE_SHADER_PATH				L"..\\Builds\\x64\\Release\\PrimitiveVS.cso"
//#define PG_CUBEMAP_SHADER_PATH					L"..\\Builds\\x64\\Release\\CubemapVS.cso"
//#define PG_INDIVIDUAL_PER_OBJMAT_STATIC_PATH	L"..\\Builds\\x64\\Release\\Individual_PerObjMatStaticVS.cso"
//#define PG_INDIVIDUAL_PER_OBJMAT_SKINNED_PATH	L"..\\Builds\\x64\\Release\\Individual_PerObjMatSkinnedVS.cso"
//#define PG_1ST_INSTANCED_SHADER_PATH			L"..\\Builds\\x64\\Release\\FirstInstanced_VS.cso"
//#endif // _DEBUG

namespace Pg::Graphics
{
	using Pg::Util::Helper::ResourceHelper;
	using namespace Pg::Defines;

	ID3D11InputLayout* LayoutDefine::_instanced1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_static1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_skinned1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_deferredQuadLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_cubemapLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_wireframePrimitiveLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_spriteCustomLayout = nullptr;
	//ID3D11InputLayout* LayoutDefine::_vinPerObjMatStaticLayout = nullptr;
	//ID3D11InputLayout* LayoutDefine::_vinPerObjMatSkinnedLayout = nullptr;

	void LayoutDefine::Initialize()
	{
		CreateInstanced1stLayout();
		CreateStatic1stLayout();
		CreateSkinned1stLayout();
		CreateDeferredQuadLayout();
		CreateWireframePrimitiveLayout();
		CreateCubemapLayout();
		CreateSpriteCustomLayout();
		//CreatePerObjMatStaticLayout();
		//CreatePerObjMatSkinnedLayout();
	}

	ID3D11InputLayout* LayoutDefine::GetInstanced1stLayout()
	{
		return _instanced1stLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetStatic1stLayout()
	{
		return _static1stLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetSkinned1stLayout()
	{
		return _skinned1stLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetDeferredQuadLayout()
	{
		return _deferredQuadLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetWireframePrimitiveLayout()
	{
		return _wireframePrimitiveLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetCubemapLayout()
	{
		return _cubemapLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetSpriteCustomLayout()
	{
		return _spriteCustomLayout;
	}

	//ID3D11InputLayout* LayoutDefine::GetPerObjMatStaticLayout()
	//{
	//	return _vinPerObjMatStaticLayout;
	//}
	//
	//ID3D11InputLayout* LayoutDefine::GetPerObjMatSkinnedLayout()
	//{
	//	return _vinPerObjMatSkinnedLayout;
	//}

	void LayoutDefine::CreateInstanced1stLayout()
	{
		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		//FIRST_INSTANCED_VS_DIRECTORY
		ID3DBlob* tInstanced1stByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_INSTANCED_VS_DIRECTORY).c_str(), &(tInstanced1stByteCode)));

		D3D11_INPUT_ELEMENT_DESC vin1stInstancedDesc[] =
		{
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"MESH_MATID",	0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TRANSFORM",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	2, 0,D3D11_INPUT_PER_INSTANCE_DATA,	1},
			{"TRANSFORM",	1, DXGI_FORMAT_R32G32B32A32_FLOAT,	2, 16,D3D11_INPUT_PER_INSTANCE_DATA,	1},
			{"TRANSFORM",	2, DXGI_FORMAT_R32G32B32A32_FLOAT,	2, 32,D3D11_INPUT_PER_INSTANCE_DATA,	1},
			{"TRANSFORM",	3, DXGI_FORMAT_R32G32B32A32_FLOAT,	2, 48,D3D11_INPUT_PER_INSTANCE_DATA,	1},
			{"OBJECTID",	0, DXGI_FORMAT_R32_UINT,			2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,	1},
			{"MATERIALID",	0, DXGI_FORMAT_R32_UINT,			2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA,	1},
			
		};

		// Instanced (static) Mesh ÀÎÇ² ·¹ÀÌ¾Æ¿ô ¸¸µé±â.
		HR(_device->CreateInputLayout(vin1stInstancedDesc, ARRAYSIZE(vin1stInstancedDesc), tInstanced1stByteCode->GetBufferPointer(),
			tInstanced1stByteCode->GetBufferSize(), &_instanced1stLayout));
	}

	void LayoutDefine::CreateStatic1stLayout()
	{
		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		//FirstStatic_VS.cso
		ID3DBlob* tStatic1stByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_STATIC_VS_DIRECTORY).c_str(), &(tStatic1stByteCode)));

		D3D11_INPUT_ELEMENT_DESC vin1stStaticDesc[] =
		{
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MESH_MATID",	0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
		};														

		// Static Mesh ÀÎÇ² ·¹ÀÌ¾Æ¿ô ¸¸µé±â.
		HR(_device->CreateInputLayout(vin1stStaticDesc, ARRAYSIZE(vin1stStaticDesc), tStatic1stByteCode->GetBufferPointer(),
			tStatic1stByteCode->GetBufferSize(), &_static1stLayout));
	}

	void LayoutDefine::CreateSkinned1stLayout()
	{
		//FirstSkinned_VS.cso

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tSkinned1stByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(FIRST_SKINNED_VS_DIRECTORY).c_str(), &(tSkinned1stByteCode)));

		// Skinned Mesh ÀÎÇ² ±¸Á¶Ã¼
		D3D11_INPUT_ELEMENT_DESC vin1stSkinnedDesc[] =
		{
			{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MESH_MATID",		0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	0, DXGI_FORMAT_R32_UINT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	1, DXGI_FORMAT_R32_UINT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	2, DXGI_FORMAT_R32_UINT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	3, DXGI_FORMAT_R32_UINT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		1, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		2, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		3, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",			0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",			0, DXGI_FORMAT_R32G32B32_FLOAT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Static Mesh ÀÎÇ² ·¹ÀÌ¾Æ¿ô ¸¸µé±â.
		HR(_device->CreateInputLayout(vin1stSkinnedDesc, ARRAYSIZE(vin1stSkinnedDesc), tSkinned1stByteCode->GetBufferPointer(),
			tSkinned1stByteCode->GetBufferSize(), &_skinned1stLayout));
	}

	void LayoutDefine::CreateDeferredQuadLayout()
	{
		//SecondStage_VS.cso

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tDefQuadByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(FINAL_STAGE_VS_DIRECTORY).c_str(), &(tDefQuadByteCode)));

		D3D11_INPUT_ELEMENT_DESC quadDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HR(_device->CreateInputLayout(quadDesc, ARRAYSIZE(quadDesc), tDefQuadByteCode->GetBufferPointer(), tDefQuadByteCode->GetBufferSize(), &_deferredQuadLayout));
	}

	void LayoutDefine::CreateWireframePrimitiveLayout()
	{
		//PrimitiveVS.cso

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(PRIMTIVE_VS_DIRECTORY).c_str(), &(tByteCode)));
		
		D3D11_INPUT_ELEMENT_DESC tDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		HR(_device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_wireframePrimitiveLayout));
	}

	void LayoutDefine::CreateCubemapLayout()
	{
		//CubemapVS.cso

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(CUBEMAP_VS_DIRECTORY).c_str(), &(tByteCode)));

		D3D11_INPUT_ELEMENT_DESC tDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HR(_device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_cubemapLayout));
	}

	void LayoutDefine::CreateSpriteCustomLayout()
	{
		//SpriteInputUsageTest_VS.cso
		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tByteCode = nullptr;
		HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(SPRITE_VERTEX_SHADER_TEST_DIRECTORY).c_str(), &(tByteCode)));

		//Vertex Shader - DXTKÀÇ VertexPositionColorTexture¸¦ °¡Á®¿È.
		HR(_device->CreateInputLayout(DirectX::VertexPositionColorTexture::InputElements, 
			DirectX::VertexPositionColorTexture::InputElementCount, tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_spriteCustomLayout));
	}


	//void LayoutDefine::CreatePerObjMatStaticLayout()
	//{
	//	LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
	//	ID3D11Device* _device = tDXStorage->_device;
	//	ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;
	//
	//	ID3DBlob* tByteCode = nullptr;
	//	HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(INDIVIDUAL_PEROBJMAT_STATIC_VS_DIRECTORY).c_str(), &(tByteCode)));
	//
	//	D3D11_INPUT_ELEMENT_DESC tDesc[] =
	//	{
	//		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"MESH_MATID",	0, DXGI_FORMAT_R32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"OBJECTID",	0, DXGI_FORMAT_R32_UINT,			1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"MATERIALID",	0, DXGI_FORMAT_R32_UINT,			1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	//	};
	//
	//	HR(_device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_vinPerObjMatStaticLayout));
	//}
	//
	//{
	//	LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
	//	ID3D11Device* _device = tDXStorage->_device;
	//	ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;
	//
	//	ID3DBlob* tByteCode = nullptr;
	//	HR(D3DReadFileToBlob(ResourceHelper::IfReleaseChangeDebugTextW(INDIVIDUAL_PEROBJMAT_SKINNED_VS_DIRECTORY).c_str(), &(tByteCode)));
	//
	//	D3D11_INPUT_ELEMENT_DESC tDesc[] =
	//	{
	//		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"MESH_MATID",		0, DXGI_FORMAT_R32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDINDICES",	0, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDINDICES",	1, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDINDICES",	2, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDINDICES",	3, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDWEIGHT",		0, DXGI_FORMAT_R32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDWEIGHT",		1, DXGI_FORMAT_R32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"BLENDWEIGHT",		2, DXGI_FORMAT_R32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"NODE_INDEX",		0, DXGI_FORMAT_R32_UINT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"OBJECTID",		0, DXGI_FORMAT_R32_UINT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//		{"MATERIALID",		0, DXGI_FORMAT_R32_UINT,		1, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	};
	//
	//	HR(_device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_vinPerObjMatSkinnedLayout));
	//}

	LayoutDefine::VinCubemap::VinCubemap(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv) :
		posL(pos), tex(uv)
	{
		//
	}

	LayoutDefine::VinWireframePrimitive::VinWireframePrimitive(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col) :
		posL(pos), color(col)
	{
		//
	}

	LayoutDefine::VinDeferredQuad::VinDeferredQuad(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 norm, DirectX::XMFLOAT2 uv) :
		posL(pos), normalL(norm), tex(uv) 
	{
		//
	}
}