#include "AssetBasic2DLoader.h"
#include "LowDX11Storage.h"

#include "RenderTexture2D.h"
#include "RenderFont.h"
#include "RenderCubemap.h"

#include "DX11Headers.h"
#include "LayoutDefine.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <dxtk/DDSTextureLoader.h>
#include <dxtk/WICTextureLoader.h>

namespace Pg::Graphics::Loader
{
	using Pg::Util::Helper::ResourceHelper;

	AssetBasic2DLoader::AssetBasic2DLoader() : _DXStorage(LowDX11Storage::GetInstance())
	{
		
	}

	void AssetBasic2DLoader::LoadTexture2D(const std::string& path, RenderTexture2D* outTextureData)
	{
		//한글 Path는 넘어오지 못하겠지만, 일단은.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		if (ResourceHelper::IsResourceDDS(path))
		{
			HR(DirectX::CreateDDSTextureFromFile(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
		}
		else
		{
			HR(DirectX::CreateWICTextureFromFile(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
		}
	}

	void AssetBasic2DLoader::LoadFont(const std::string& path, RenderFont* outFontData)
	{
		//한글 Path는 넘어오지 못하겠지만, 일단은.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		outFontData->_font = std::make_unique<DirectX::SpriteFont>(_DXStorage->_device, tWStrPath.c_str());
	}

	void AssetBasic2DLoader::LoadCubemap(const std::string& path, RenderCubemap* outCubemapData)
	{
		//한글 Path는 넘어오지 못하겠지만, 일단은.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		PlaceCubemapBuffers(outCubemapData);
		PlaceCubemapSRV(tWStrPath, outCubemapData);
		//Sampler는 파이프라인 별도로 작동할 것이다.
	}

	void AssetBasic2DLoader::PlaceCubemapBuffers(RenderCubemap* outCubemapData)
	{
		std::vector<LayoutDefine::VinCubemap> VBData;
		std::vector<UINT> IBData;

		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{-100.0f, 100.0f, -100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{100.0f, 100.0f, -100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{-100.0f, 100.0f, 100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{100.0f, 100.0f, 100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{100.0f, -100.0f, -100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{-100.0f, -100.0f, -100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{-100.0f, -100.0f, 100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });
		VBData.emplace_back(LayoutDefine::VinCubemap{ DirectX::XMFLOAT3{100.0f, -100.0f, 100.0f}, DirectX::XMFLOAT2{0.0f, 0.0f} });

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
		VBDesc.ByteWidth = VBData.size() * sizeof(LayoutDefine::VinCubemap);
		VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VBDesc.CPUAccessFlags = 0;
		VBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA VBInitData;
		VBInitData.pSysMem = &(VBData[0]);
		VBInitData.SysMemPitch = 0;
		VBInitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		HR(_DXStorage->_device->CreateBuffer(&VBDesc, &VBInitData, &(outCubemapData->_VB)));

		// Buffer Description
		D3D11_BUFFER_DESC IBDesc;
		IBDesc.Usage = D3D11_USAGE_DEFAULT;
		IBDesc.ByteWidth = IBData.size() * sizeof(UINT);
		IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IBDesc.CPUAccessFlags = 0;
		IBDesc.MiscFlags = 0;

		// Subresource Data
		D3D11_SUBRESOURCE_DATA IBInitData;
		IBInitData.pSysMem = &(IBData[0]);
		IBInitData.SysMemPitch = 0;
		IBInitData.SysMemSlicePitch = 0;

		// Create the Index buffer.
		HR(_DXStorage->_device->CreateBuffer(&IBDesc, &IBInitData, &(outCubemapData->_IB)));
	}

	void AssetBasic2DLoader::PlaceCubemapSRV(const std::wstring& path, RenderCubemap* outCubemapData)
	{
		ID3D11Resource* tTexture = nullptr;
		assert(ResourceHelper::IsResourceDDS(path));
		HR(DirectX::CreateDDSTextureFromFile(_DXStorage->_device, path.c_str(), &tTexture, &(outCubemapData->_srv)));
	}

}