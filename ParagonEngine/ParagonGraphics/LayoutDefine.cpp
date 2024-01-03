#include "LayoutDefine.h"
#include "LowDX11Storage.h"

#include <d3d11.h>
#include <fstream>
#include <cassert>
#include <vector>

#ifdef _DEBUG
#define PG_1ST_STATIC_SHADER_PATH		L"..\\Builds\\x64\\Debug\\FirstStatic_VS.cso"
#define PG_1ST_SKINNED_SHADER_PATH		L"..\\Builds\\x64\\Debug\\FirstSkinned_VS.cso"
#define PG_DEFERRED_QUAD_SHADER_PATH	L"..\\Builds\\x64\\Debug\\SecondStage_VS.cso"
#define PG_PRIMITIVE_SHADER_PATH		L"..\\Builds\\x64\\Debug\\PrimitiveVS.cso"
#define PG_CUBEMAP_SHADER_PATH			L"..\\Builds\\x64\\Debug\\CubemapVS.cso"
#else
#define PG_1ST_STATIC_SHADER_PATH		L"..\\Builds\\x64\\Release\\FirstStatic_VS.cso"
#define PG_1ST_SKINNED_SHADER_PATH		L"..\\Builds\\x64\\Release\\FirstSkinned_VS.cso"
#define PG_DEFERRED_QUAD_SHADER_PATH	L"..\\Builds\\x64\\Release\\SecondStage_VS.cso"
#define PG_PRIMITIVE_SHADER_PATH		L"..\\Builds\\x64\\Release\\PrimitiveVS.cso"
#define PG_CUBEMAP_SHADER_PATH			L"..\\Builds\\x64\\Release\\CubemapVS.cso"
#endif // _DEBUG

namespace Pg::Graphics
{
	ID3D11InputLayout* LayoutDefine::_static1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_skinned1stLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_deferredQuadLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_cubemapLayout = nullptr;
	ID3D11InputLayout* LayoutDefine::_wireframePrimitiveLayout = nullptr;

	void LayoutDefine::Initialize()
	{
		CreateStatic1stLayout();
		CreateSkinned1stLayout();
		CreateDeferredQuadLayout();
		CreateWireframePrimitiveLayout();
		CreateCubemapLayout();
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

	void LayoutDefine::CreateStatic1stLayout()
	{
		HRESULT hr = S_OK;

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		//FirstStatic_VS.cso
		ID3DBlob* tStatic1stByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_1ST_STATIC_SHADER_PATH, &(tStatic1stByteCode));
		if (FAILED(hr)) { assert(false); }

		D3D11_INPUT_ELEMENT_DESC vin1stStaticDesc[] =
		{
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 52,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, 64,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"ALPHA",		0, DXGI_FORMAT_R32_FLOAT,			0, 72,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"OBJECTID",	0, DXGI_FORMAT_R32_UINT,			0, 76,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		// Static Mesh 인풋 레이아웃 만들기.
		hr = _device->CreateInputLayout(vin1stStaticDesc, ARRAYSIZE(vin1stStaticDesc), tStatic1stByteCode->GetBufferPointer(),
			tStatic1stByteCode->GetBufferSize(), &_static1stLayout);
		if (FAILED(hr)) { assert(false); }
	}

	void LayoutDefine::CreateSkinned1stLayout()
	{
		//FirstSkinned_VS.cso
		HRESULT hr = S_OK;

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tSkinned1stByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_1ST_SKINNED_SHADER_PATH, &(tSkinned1stByteCode));
		if (FAILED(hr)) { assert(false); }

		// Skinned Mesh 인풋 구조체
		D3D11_INPUT_ELEMENT_DESC vin1stSkinnedDesc[] =
		{
			{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 52,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",		1, DXGI_FORMAT_R32G32_FLOAT,		0, 64,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"ALPHA",			0, DXGI_FORMAT_R32_FLOAT,			0, 72,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"OBJECTID",		0, DXGI_FORMAT_R32_UINT,			0, 76,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	0, DXGI_FORMAT_R32_UINT,			0, 80,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	1, DXGI_FORMAT_R32_UINT,			0, 84,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	2, DXGI_FORMAT_R32_UINT,			0, 88,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDINDICES",	3, DXGI_FORMAT_R32_UINT,			0, 92,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		0, DXGI_FORMAT_R32_FLOAT,			0, 96,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		1, DXGI_FORMAT_R32_FLOAT,			0, 100,	D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BLENDWEIGHT",		2, DXGI_FORMAT_R32_FLOAT,			0, 104,	D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Static Mesh 인풋 레이아웃 만들기.
		hr = _device->CreateInputLayout(vin1stSkinnedDesc, ARRAYSIZE(vin1stSkinnedDesc), tSkinned1stByteCode->GetBufferPointer(),
			tSkinned1stByteCode->GetBufferSize(), &_skinned1stLayout);
	}

	void LayoutDefine::CreateDeferredQuadLayout()
	{
		//SecondStage_VS.cso
		HRESULT hr = S_OK;

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tDefQuadByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_DEFERRED_QUAD_SHADER_PATH, &(tDefQuadByteCode));
		if (FAILED(hr)) { assert(false); }

		D3D11_INPUT_ELEMENT_DESC quadDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = _device->CreateInputLayout(quadDesc, ARRAYSIZE(quadDesc), tDefQuadByteCode->GetBufferPointer(), tDefQuadByteCode->GetBufferSize(), &_deferredQuadLayout);
	}

	void LayoutDefine::CreateWireframePrimitiveLayout()
	{
		//PrimitiveVS.cso
		HRESULT hr = S_OK;

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_PRIMITIVE_SHADER_PATH, &(tByteCode));
		if (FAILED(hr)) { assert(false); }
		
		D3D11_INPUT_ELEMENT_DESC tDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		hr = _device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_wireframePrimitiveLayout);
	}

	void LayoutDefine::CreateCubemapLayout()
	{
		//CubemapVS.cso
		HRESULT hr = S_OK;

		LowDX11Storage* tDXStorage = LowDX11Storage::GetInstance();
		ID3D11Device* _device = tDXStorage->_device;
		ID3D11DeviceContext* _devcon = tDXStorage->_deviceContext;

		ID3DBlob* tByteCode = nullptr;
		hr = D3DReadFileToBlob(PG_CUBEMAP_SHADER_PATH, &(tByteCode));
		if (FAILED(hr)) { assert(false); }

		D3D11_INPUT_ELEMENT_DESC tDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		hr = _device->CreateInputLayout(tDesc, ARRAYSIZE(tDesc), tByteCode->GetBufferPointer(), tByteCode->GetBufferSize(), &_cubemapLayout);
	}

	//개별적인 요소 Layout 생성자.
	LayoutDefine::Vin1stStatic::Vin1stStatic(DirectX::XMFLOAT3 posVal) :
		_posL(posVal), _normalL(0.0f, 0.0f, 0.0f), _tangentL(0.0f, 0.0f, 0.0f),
		_color(1.0f, 0.0f, 0.0f, 1.0f), _texUV(0.f, 0.f, 0.f), _lightmapUV(0.f, 0.f), _alpha(0.f), _objectID(0)
	{
		//
	}

	LayoutDefine::Vin1stStatic::Vin1stStatic(DirectX::XMFLOAT3 posVal, DirectX::XMFLOAT3 normalVal, DirectX::XMFLOAT3 tangentVal, 
		DirectX::XMFLOAT4 colorVal, DirectX::XMFLOAT3 texVal, DirectX::XMFLOAT2 lightmapUVVal, float alphaVal, unsigned int objIDVal) :
		_posL(posVal), _normalL(normalVal), _tangentL(tangentVal),
		_color(colorVal), _texUV(texVal), _lightmapUV(lightmapUVVal), _alpha(alphaVal), _objectID(0)
	{

	}

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
