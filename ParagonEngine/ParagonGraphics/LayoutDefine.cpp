#include "LayoutDefine.h"
#include "LowDX11Storage.h"

#include <d3d11.h>
#include <fstream>
#include <cassert>
#include <vector>

#ifdef _DEBUG
#define PG_1ST_STATIC_SHADER_PATH	L"..\\Builds\\x64\\Debug\\FirstStatic_VS.cso"
#define PG_1ST_SKINNED_SHADER_PATH	L"..\\Builds\\x64\\Debug\\FirstSkinned_VS.cso"
#else
#define PG_1ST_STATIC_SHADER_PATH	L"..\\Builds\\x64\\Release\\FirstStatic_VS.cso"
#define PG_1ST_SKINNED_SHADER_PATH	L"..\\Builds\\x64\\Release\\FirstSkinned_VS.cso"
#endif // _DEBUG

namespace Pg::Graphics
{
	ID3D11InputLayout* LayoutDefine::_static1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_skinned1stLayout = nullptr;

	void LayoutDefine::Initialize()
	{
		HRESULT hr = S_OK;

		LowDX11Storage* _DXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* tD3DDevice = _DXStorage->_device;

		//FirstStatic_VS.cso
		ID3DBlob* tStatic1stByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_1ST_STATIC_SHADER_PATH, &(tStatic1stByteCode));
		if (FAILED(hr)) { assert(false); }

		// Static Mesh ÀÎÇ² ±¸Á¶Ã¼
		D3D11_INPUT_ELEMENT_DESC vin1stStaticDesc[] =
		{
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 52,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATERIALID",	0, DXGI_FORMAT_R32_UINT,			0, 64,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		// Static Mesh ÀÎÇ² ·¹ÀÌ¾Æ¿ô ¸¸µé±â.
		hr = tD3DDevice->CreateInputLayout(vin1stStaticDesc, ARRAYSIZE(vin1stStaticDesc), tStatic1stByteCode->GetBufferPointer(),
			tStatic1stByteCode->GetBufferSize(), &_static1stLayout);
		if (FAILED(hr)) { assert(false); }
			
		
		//FirstSkinned_VS.cso
		ID3DBlob* tSkinned1stByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_1ST_SKINNED_SHADER_PATH, &(tSkinned1stByteCode));
		if (FAILED(hr)) { assert(false); }

		// Skinned Mesh ÀÎÇ² ±¸Á¶Ã¼
		D3D11_INPUT_ELEMENT_DESC vin1stSkinnedDesc[] =
		{
			{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	24,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	36,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	52,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"MATERIALID",		0, DXGI_FORMAT_R32_UINT,			0,	64,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	0, DXGI_FORMAT_R32_UINT,			0,	68,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	1, DXGI_FORMAT_R32_UINT,			0,	72,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	2, DXGI_FORMAT_R32_UINT,			0,	76,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	3, DXGI_FORMAT_R32_UINT,			0,	80,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		0, DXGI_FORMAT_R32_FLOAT,			0,	84,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		1, DXGI_FORMAT_R32_FLOAT,			0,	88,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		2, DXGI_FORMAT_R32_FLOAT,			0,	92,	D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Static Mesh ÀÎÇ² ·¹ÀÌ¾Æ¿ô ¸¸µé±â.
		hr = tD3DDevice->CreateInputLayout(vin1stSkinnedDesc, ARRAYSIZE(vin1stSkinnedDesc), tSkinned1stByteCode->GetBufferPointer(),
			tSkinned1stByteCode->GetBufferSize(), &_skinned1stLayout);
	}

	ID3D11InputLayout* LayoutDefine::GetStatic1stLayout()
	{
		return _static1stLayout;
	}

	ID3D11InputLayout* LayoutDefine::GetSkinned1stLayout()
	{
		return _skinned1stLayout;
	}


	LayoutDefine::Vin1stStatic::Vin1stStatic(DirectX::XMFLOAT3 posVal) :
		posL(posVal), normalL( 0.0f, 0.0f,0.0f ), tangentL( 0.0f, 0.0f,0.0f ),
		color( 1.0f,0.0f,0.0f,1.0f ), tex( 0.f,0.f,0.f ), matID(0)
	{
		//
	}

	LayoutDefine::Vin1stStatic::Vin1stStatic(DirectX::XMFLOAT3 posVal, DirectX::XMFLOAT3 normalVal, 
		DirectX::XMFLOAT3 tangentVal, DirectX::XMFLOAT4 colorVal, DirectX::XMFLOAT3 texVal, unsigned int matIDVal) :
		posL(posVal), normalL(normalVal), tangentL(tangentVal), color(colorVal), tex(texVal), matID(matIDVal)
	{
		//
	}

	

	

	

}
