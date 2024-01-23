#include "AssetBasic2DLoader.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"

#include "RenderTexture1D.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "RenderTextureCube.h"
#include "RenderFont.h"
#include "RenderCubemap.h"

#include "DX11Headers.h"
#include "LayoutDefine.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <dxtk/DDSTextureLoader.h>
#include <dxtk/WICTextureLoader.h>
#include <fstream>
#include <tuple>

namespace Pg::Graphics::Loader
{
	using Pg::Util::Helper::ResourceHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;

	AssetBasic2DLoader::AssetBasic2DLoader() : _DXStorage(LowDX11Storage::GetInstance())
	{
		
	}

	void AssetBasic2DLoader::LoadTexture1D(const std::string& path, RenderTexture1D* outTextureData)
	{
		LoadInternalRenderTexture2D(path, outTextureData);
	}

	void AssetBasic2DLoader::LoadTexture2D(const std::string& path, RenderTexture2D* outTextureData)
	{
		LoadInternalRenderTexture2D(path, outTextureData);
	}

	void AssetBasic2DLoader::LoadTexture2DArray(const std::string& path, RenderTexture2DArray* outTextureData)
	{
		std::ifstream in(path);
		std::string tContentString = "";
		in >> tContentString;
		
		std::vector<std::string> tSingleTextureNameVec;
		Pg::Graphics::Helper::GraphicsResourceHelper::ReadPGT2ARRContents(tContentString, tSingleTextureNameVec);

		std::vector<RenderTexture2D*> tSingleRenderTexture2DArray;
		for (int i = 0; i < tSingleTextureNameVec.size(); i++)
		{
			auto tRes2D = GraphicsResourceManager::Instance()->GetResourceByName(tSingleTextureNameVec.at(i), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
			tSingleRenderTexture2DArray.push_back(static_cast<RenderTexture2D*>(tRes2D.get()));
		}

		MultipleRenderTexture2DToTexture2DArray(tSingleRenderTexture2DArray.data(), tSingleRenderTexture2DArray.size(), outTextureData);
	}

	void AssetBasic2DLoader::LoadGIF(const std::string& path, RenderTexture2DArray* outTextureData)
	{
		assert(false && "texassemble을 통해 지원을 시작해야 이게 허용");
	}

	void AssetBasic2DLoader::LoadTextureCube(const std::string& path, RenderTextureCube* outTextureData)
	{
		LoadInternalRenderTexture2D(path, outTextureData);
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

	void AssetBasic2DLoader::LoadInternalRenderTexture2D(const std::string& path, RenderTexture* outTextureData)
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

	void AssetBasic2DLoader::MultipleRenderTexture2DToTexture2DArray(RenderTexture2D** textureSrc, unsigned int cnt, RenderTexture2DArray* outTextureData)
	{
		//개별적인 Texture2D의 Width / Height / MipLevels / Format / ArraySize.
		//하나로 통일되어야 하는데, 이를 더블체크하는 코드.
		std::vector<std::tuple<UINT, UINT, UINT, DXGI_FORMAT, UINT>> tSingleTextureStoreVec;
		std::vector<ID3D11Texture2D*> tTexture2DStoreVec;

		for (int i = 0; i < cnt; i++)
		{
			ID3D11Texture2D* tTexture2D = nullptr;
			HR(textureSrc[i]->GetResource()->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tTexture2D));
			tTexture2DStoreVec.push_back(tTexture2D);

			D3D11_TEXTURE2D_DESC tTextureDesc;
			tTexture2D->GetDesc(&tTextureDesc);

			//이로서 리소스의 기본적인 Width, Height, Mipmap 레벨 등을 받아올 수 있게 되었다.
			tSingleTextureStoreVec.push_back(std::make_tuple(tTextureDesc.Width, tTextureDesc.Height, tTextureDesc.MipLevels,
				tTextureDesc.Format, tTextureDesc.ArraySize));
		}

		//들어온 Texture2D들이 Texture2DArray를 만들기 위해 적합한지를 판단.
		UINT tUniformWidth = std::get<0>(tSingleTextureStoreVec.at(0));
		UINT tUniformHeight = std::get<1>(tSingleTextureStoreVec.at(0));
		UINT tUniformMipLevels = std::get<2>(tSingleTextureStoreVec.at(0));
		DXGI_FORMAT tUniformDXGIFormat = std::get<3>(tSingleTextureStoreVec.at(0));
		UINT tUniformArraySize = std::get<4>(tSingleTextureStoreVec.at(0));
		
		for (auto& [bWidth, bHeight, bMipLevels, bFormat, bArraySize] : tSingleTextureStoreVec)
		{
			//모든 Texture2D들은 같은 프로퍼티를 공유해야 Texture2DArray로 만들어질 수 있다.
			assert(tUniformWidth == bWidth);
			assert(tUniformHeight == bHeight);
			assert(tUniformMipLevels == bMipLevels);
			assert(tUniformDXGIFormat == bFormat);
			assert(tUniformArraySize == bArraySize);
		}

		//Texture2DArray를 실제로 만든다.
		
		//기록되었던 Texture2D를 활용.
		D3D11_TEXTURE2D_DESC arrayTextureDesc;
		ZeroMemory(&arrayTextureDesc, sizeof(arrayTextureDesc));
		arrayTextureDesc.Width = tUniformWidth;
		arrayTextureDesc.Height = tUniformHeight;
		arrayTextureDesc.MipLevels = tUniformMipLevels;
		arrayTextureDesc.ArraySize = cnt;
		arrayTextureDesc.Format = tUniformDXGIFormat; 
		arrayTextureDesc.SampleDesc.Count = 1;
		arrayTextureDesc.SampleDesc.Quality = 0;
		arrayTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		arrayTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		arrayTextureDesc.CPUAccessFlags = 0;
		arrayTextureDesc.MiscFlags = 0;

		//Resource가져와서 연동.
		ID3D11Texture2D* arrayTexture2D = nullptr;
		HR(_DXStorage->_device->CreateTexture2D(&arrayTextureDesc, nullptr, &arrayTexture2D));

		//// Loop through each slice of the Texture2DArray and copy data from source textures
		for (UINT sliceIndex = 0; sliceIndex < arrayTextureDesc.ArraySize; ++sliceIndex)
		{
			//저장되어 있는 모든 Vector.
			for (int i = 0; i < tTexture2DStoreVec.size(); i++)
			{
				// Use CopySubresourceRegion to copy data from sourceTex1 to destTexArray
				_DXStorage->_deviceContext->CopySubresourceRegion(
					arrayTexture2D,        // Destination resource (Texture2DArray)
					D3D11CalcSubresource(0, sliceIndex, arrayTextureDesc.MipLevels),
					0, 0, 0,              // DestX, DestY, DestZ
					tTexture2DStoreVec[i],          // Source resource (Texture2D)
					0,                    // SrcSubresource
					nullptr               // pSrcBox (use nullptr to copy the entire resource)
				);
			}
		}

		//for (UINT sliceIndex = 0; sliceIndex < arrayTextureDesc.ArraySize; ++sliceIndex) {
		//	// Loop through each mip level
		//	for (UINT mipLevel = 0; mipLevel < tUniformMipLevels; ++mipLevel) {
		//		// Determine the source SRV based on the slice index and mip level
		//		//ID3D11ShaderResourceView* sourceSRV = sourceSRVs[sliceIndex * NUM_MIPMAPS + mipLevel];
		//
		//		// Use CopySubresourceRegion to copy data from the source SRV to destTexArray
		//		_DXStorage->_deviceContext->CopySubresourceRegion(
		//			arrayTexture2D,                // Destination resource (Texture2DArray)
		//			D3D11CalcSubresource(mipLevel, sliceIndex, arrayTextureDesc.MipLevels),
		//			0, 0, 0,                      // DestX, DestY, DestZ
		//			tTexture2DStoreVec[mipLevel], // Helper function to get the source Texture2D from SRV
		//			mipLevel,                     // SrcSubresource (mipmap level)
		//			nullptr                       // pSrcBox (use nullptr to copy the entire resource)
		//		);
		//	}
		//}

		//for (UINT texElement = 0; texElement < arrayTextureDesc.ArraySize; ++texElement)
		//{
		//	// for each mipmap level...
		//	for (UINT mipLevel = 0; mipLevel < arrayTextureDesc.MipLevels; ++mipLevel)
		//	{
		//		D3D11_MAPPED_SUBRESOURCE mappedTex2D;
		//		//HR(_DXStorage->_deviceContext->Map(tTexture2DStoreVec[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
		//		HR(_DXStorage->_deviceContext->Map(tTexture2DStoreVec[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));
		//
		//		_DXStorage->_deviceContext->UpdateSubresource(arrayTexture2D,
		//			D3D11CalcSubresource(mipLevel, texElement, arrayTextureDesc.MipLevels),
		//			0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);
		//
		//		_DXStorage->_deviceContext->Unmap(tTexture2DStoreVec[texElement], mipLevel);
		//	}
		//}
		
		HR(arrayTexture2D->QueryInterface(__uuidof(ID3D11Resource), (void**)&outTextureData->GetResource()));


		D3D11_SHADER_RESOURCE_VIEW_DESC tSrvArrayDesc;
		ZeroMemory(&tSrvArrayDesc, sizeof(tSrvArrayDesc));
		tSrvArrayDesc.Format = tUniformDXGIFormat;
		tSrvArrayDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		tSrvArrayDesc.Texture2DArray.MipLevels = tUniformMipLevels;
		tSrvArrayDesc.Texture2DArray.ArraySize = cnt;

		ID3D11ShaderResourceView*& textureArraySRV = outTextureData->GetSRV();
		HR(_DXStorage->_device->CreateShaderResourceView(arrayTexture2D, &tSrvArrayDesc, &textureArraySRV));
		
		assert("");
	}

	

}