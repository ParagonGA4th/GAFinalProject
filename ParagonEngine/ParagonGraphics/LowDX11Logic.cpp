#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

#include <vector>
#include <string>

namespace Pg::Graphics
{
	LowDX11Logic::LowDX11Logic(LowDX11Storage* DXStorage)
		:_DXStorage(DXStorage),
		hr(NULL)
	{

	}

	HRESULT LowDX11Logic::CreateDevice()
	{
		// D3D11 Device 생성
		hr = D3D11CreateDevice(
			NULL,															// [in, optional]	IDXGIAdapter				*pAdapter
			D3D_DRIVER_TYPE_HARDWARE,										//					D3D_Driver_Type				DriverType
			NULL,															//					HMODULE						Software
			D3D11_CREATE_DEVICE_DEBUG,									//					UINT						Flags
			NULL,															// [in, optional]	const D3D_FEATURE_LEVEL		*pFeatureLevels
			NULL,															//					UINT						FeatureLevels
			D3D11_SDK_VERSION,												//					UINT						SDKVersion
			&(_DXStorage->_device),														// [out, optional]	ID3D11Device				**ppDevice
			NULL,															// [out, optional]	D3D_FEATUER_LEVEL			*pFeatureLevel
			&(_DXStorage->_deviceContext)												// [out, optional]	ID3D11DeviceContext			**ppImmediateContext
		);

		return hr;
	}

	HRESULT LowDX11Logic::CreateSwapChain(int screenWidth, int screenHeight)
	{
		// Swap Chain Description 정의
		_DXStorage->_swapChainDesc.Width = screenHeight;
		_DXStorage->_swapChainDesc.Height = 0;
		_DXStorage->_swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_DXStorage->_swapChainDesc.Stereo = FALSE;
		_DXStorage->_swapChainDesc.SampleDesc.Count = 1;
		_DXStorage->_swapChainDesc.SampleDesc.Quality = 0;
		_DXStorage->_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_DXStorage->_swapChainDesc.BufferCount = 2;
		_DXStorage->_swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		_DXStorage->_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		_DXStorage->_swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		_DXStorage->_swapChainDesc.Flags = NULL;

		// DXGI Factory 생성
		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&_DXStorage->_factory));

		if (hr != S_OK)
		{
			return hr;
		}

		// hWnd 사용하여 스왑체인 생성
		hr = _DXStorage->_factory->CreateSwapChainForHwnd(
			_DXStorage->_device,
			_DXStorage->_hWnd,
			&(_DXStorage->_swapChainDesc),
			NULL,
			NULL,
			&(_DXStorage->_swapChain)
		);

		return hr;
	}

	HRESULT LowDX11Logic::CreateMainRenderTarget()
	{
		hr = _DXStorage->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&(_DXStorage->_backBuffer));

		if (hr != S_OK)
		{
			return hr;
		}

		hr = _DXStorage->_device->CreateRenderTargetView(_DXStorage->_backBuffer, nullptr, &(_DXStorage->_mainRTV));

		return hr;
	}

	HRESULT LowDX11Logic::CreateDepthStencilViewAndState()
	{
		/// Depth Stencil Buffer
		// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
		_DXStorage->_backBuffer->GetDesc(&_DXStorage->_depthStencilBufferDesc);

		// Depth-Stencil Buffer를 위한 Texture Resource Description 구조체 정의
		// 백버퍼의 속성을 가져온 후, Format과 BindFlags만 바꾸어 쓴다.
		_DXStorage->_depthStencilBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		_DXStorage->_depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		// Depth-Stencil Buffer 생성
		hr = _DXStorage->_device->CreateTexture2D(&(_DXStorage->_depthStencilBufferDesc), NULL, &(_DXStorage->_depthStencilBuffer));

		if (hr != S_OK)
			return hr;


		/// Depth Stencil View
		//// Depth-Stencil Description 구조체 정의
		ZeroMemory(&(_DXStorage->_depthStencilDesc), sizeof(D3D11_DEPTH_STENCIL_DESC));
		_DXStorage->_depthStencilDesc.DepthEnable = true;
		_DXStorage->_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		_DXStorage->_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Depth-Stencil State 생성
		// (depth-stencil state는 OM 스테이지에 depth-stencil 테스트를 수행하는 방법을 전달한다)
		_DXStorage->_device->CreateDepthStencilState(&(_DXStorage->_depthStencilDesc), &(_DXStorage->_depthStencilState));

		//// Depth-Stencil View 생성
		//// (Resource로 View를 생성해야 파이프라인에 바인드할 수 있다)

		_DXStorage->_depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		_DXStorage->_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		_DXStorage->_depthStencilViewDesc.Flags = 0;

		hr = _DXStorage->_device->CreateDepthStencilView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_depthStencilViewDesc), &(_DXStorage->_depthStencilView));

		if (hr != S_OK)
			return hr;


		/// Depth Buffer 시각화를 위한 SRV 생성
		_DXStorage->_shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		_DXStorage->_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		_DXStorage->_shaderResourceViewDesc.Texture2D.MipLevels = -1;


		hr = _DXStorage->_device->CreateShaderResourceView(_DXStorage->_depthStencilBuffer, &(_DXStorage->_shaderResourceViewDesc), &(_DXStorage->_depthStencilSRV));

		return hr;
	}

	HRESULT LowDX11Logic::CreateRasterizerStates()
	{
		// Solid
		ZeroMemory(&(_DXStorage->_solidDesc), sizeof(D3D11_RASTERIZER_DESC));
		_DXStorage->_solidDesc.FillMode = D3D11_FILL_SOLID;
		_DXStorage->_solidDesc.CullMode = D3D11_CULL_BACK;
		_DXStorage->_solidDesc.FrontCounterClockwise = false;
		_DXStorage->_solidDesc.DepthClipEnable = true;

		hr = _DXStorage->_device->CreateRasterizerState(&(_DXStorage->_solidDesc), &(_DXStorage->_solidState));

		if (hr != S_OK)
			return hr;

		// Wireframe
		ZeroMemory(&(_DXStorage->_wireframeDesc), sizeof(D3D11_RASTERIZER_DESC));
		_DXStorage->_wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		_DXStorage->_wireframeDesc.CullMode = D3D11_CULL_BACK;
		_DXStorage->_wireframeDesc.FrontCounterClockwise = false;
		_DXStorage->_wireframeDesc.DepthClipEnable = true;

		hr = _DXStorage->_device->CreateRasterizerState(&(_DXStorage->_wireframeDesc), &(_DXStorage->_wireframeState));

		return hr;
	}

	void LowDX11Logic::SetRasterizerrStates(ID3D11RasterizerState* rasterizerState)
	{
		_DXStorage->_deviceContext->RSSetState(rasterizerState);

		return;
	}

	void LowDX11Logic::CreateAndSetViewports()
	{
		// Viewport 구조체 생성
		CD3D11_VIEWPORT viewport(
			0.0f,
			0.0f,
			(float)_DXStorage->_screenWidth,
			(float)_DXStorage->_screenHeight
		);

		// Viewport 지정
		_DXStorage->_deviceContext->RSSetViewports(1, &viewport);
	}

	void LowDX11Logic::PrepareRenderTargets()
	{
		// Set Depth Stencil State
		_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

		// Clear Main RTV and DSV
		_DXStorage->_deviceContext->ClearRenderTargetView(_DXStorage->_mainRTV, _DXStorage->_backgroundColor);
		_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void LowDX11Logic::BindRenderTargets()
	{

		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
	}

	void LowDX11Logic::UnbindRenderTargets()
	{
		// TODO: null RTV 바인딩
	}

	void LowDX11Logic::Draw()
	{
		
	}

	void LowDX11Logic::Present()
	{
		_DXStorage->_swapChain->Present(0, 0);
	}

	void LowDX11Logic::SetupCube()
	{
		struct MeshVertex
		{
			float3 position;
			float3 color;
		};

		std::vector<MeshVertex> VBData;
		std::vector<int> IBData;

		VBData.emplace_back(MeshVertex{ float3 {-1.0f, 1.0f, -1.0f}, float3{1.0f, 0.0f, 0.0f} });
		VBData.emplace_back(MeshVertex{ float3 {1.0f, 1.0f, -1.0f}, float3{0.0f, 1.0f, 0.0f} });
		VBData.emplace_back(MeshVertex{ float3 {-1.0f, 1.0f, 1.0f}, float3{0.0f, 0.0f, 1.0f} });
		VBData.emplace_back(MeshVertex{ float3 {1.0f, 1.0f, 1.0f}, float3{1.0f, 1.0f, 0.0f} });
		VBData.emplace_back(MeshVertex{ float3 {1.0f, -1.0f, -1.0f}, float3{0.0f, 1.0f, 1.0f} });
		VBData.emplace_back(MeshVertex{ float3 {-1.0f, -1.0f, -1.0f}, float3{1.0f, 0.0f, 1.0f} });
		VBData.emplace_back(MeshVertex{ float3 {-1.0f, -1.0f, 1.0f}, float3{1.0f, 1.0f, 1.0f} });
		VBData.emplace_back(MeshVertex{ float3 {1.0f, -1.0f, 1.0f}, float3{0.0f, 0.0f, 0.0f} });

		IBData.emplace_back(0);
		IBData.emplace_back(1);
		IBData.emplace_back(2);

		IBData.emplace_back(2);
		IBData.emplace_back(1);
		IBData.emplace_back(3);

		IBData.emplace_back(7);
		IBData.emplace_back(2);
		IBData.emplace_back(3);

		IBData.emplace_back(2);
		IBData.emplace_back(7);
		IBData.emplace_back(6);

		IBData.emplace_back(3);
		IBData.emplace_back(1);
		IBData.emplace_back(4);

		IBData.emplace_back(4);
		IBData.emplace_back(7);
		IBData.emplace_back(3);

		IBData.emplace_back(5);
		IBData.emplace_back(2);
		IBData.emplace_back(6);

		IBData.emplace_back(5);
		IBData.emplace_back(0);
		IBData.emplace_back(2);

		IBData.emplace_back(7);
		IBData.emplace_back(4);
		IBData.emplace_back(6);

		IBData.emplace_back(6);
		IBData.emplace_back(4);
		IBData.emplace_back(5);

		IBData.emplace_back(0);
		IBData.emplace_back(4);
		IBData.emplace_back(1);

		IBData.emplace_back(0);
		IBData.emplace_back(5);
		IBData.emplace_back(4);

		// Buffer Description
		D3D11_BUFFER_DESC VBDesc;
		VBDesc.Usage = D3D11_USAGE_DEFAULT;
		VBDesc.ByteWidth = VBData.size() * sizeof(MeshVertex);
		VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags = 0;
		VBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA VBInitData;
		VBInitData.pSysMem = &(VBData[0]);
		VBInitData.SysMemPitch = 0;
		VBInitData.SysMemSlicePitch = 0;

		ID3D11Buffer* VB;

		// Create the vertex buffer.
		HRESULT hr = _DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &VB);

		// Buffer Description
		D3D11_BUFFER_DESC IBDesc;
		IBDesc.Usage = D3D11_USAGE_DEFAULT;
		IBDesc.ByteWidth = IBData.size() * sizeof(int);
		IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IBDesc.CPUAccessFlags = 0;
		IBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA IBInitData;
		IBInitData.pSysMem = &(IBData[0]);
		IBInitData.SysMemPitch = 0;
		IBInitData.SysMemSlicePitch = 0;

		ID3D11Buffer* IB;

		// Create the Index buffer.
		hr = _DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &IB);

		UINT stride = sizeof(MeshVertex);
		UINT offset = 0;

		_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
		_DXStorage->_deviceContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

		/// InputLayout 생성 및 바인딩
		// Input Element Description 구조체
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

		};

		// InputLayout 생성
		hr = _DXStorage->_device->CreateInputLayout(vertexDesc, 2, _DXStorage->_VertexShaderByteCode->GetBufferPointer(),
			_DXStorage->_VertexShaderByteCode->GetBufferSize(), &(_DXStorage->_inputLayout));

		_DXStorage->_deviceContext->IASetInputLayout(_DXStorage->_inputLayout);
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void LowDX11Logic::DrawCube()
	{
		//create
		_DXStorage->_deviceContext->DrawIndexed(36, 0, 0);
	}

	HRESULT LowDX11Logic::ResizeSwapChainBuffers(int screenWidth, int screenHeight)
	{
		hr = _DXStorage->_swapChain->ResizeBuffers(2, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		if (hr != S_OK)
		{
			return hr;
		}
	}

	void LowDX11Logic::SetVertexShader(std::wstring CSOFilePath)
	{
		hr = D3DReadFileToBlob(CSOFilePath.c_str(), &(_DXStorage->_VertexShaderByteCode));
		_DXStorage->_device->CreateVertexShader(_DXStorage->_VertexShaderByteCode->GetBufferPointer(), _DXStorage->_VertexShaderByteCode->GetBufferSize(), NULL, &(_DXStorage->_VertexShader));
	
		_DXStorage->_deviceContext->VSSetShader(_DXStorage->_VertexShader, nullptr, 0);
		
	}

	void LowDX11Logic::SetPixelShader(std::wstring CSOFilePath)
	{
		hr = D3DReadFileToBlob(CSOFilePath.c_str(), &(_DXStorage->_PixelShaderByteCode));
		_DXStorage->_device->CreatePixelShader(_DXStorage->_PixelShaderByteCode->GetBufferPointer(), _DXStorage->_PixelShaderByteCode->GetBufferSize(), NULL, &(_DXStorage->_PixelShader));
	
		_DXStorage->_deviceContext->PSSetShader(_DXStorage->_PixelShader, nullptr, 0);
	}

}