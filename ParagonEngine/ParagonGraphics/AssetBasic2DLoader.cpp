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
#include "../ParagonHelper/ResourceHelper.h"

#include <dxtk/DDSTextureLoader.h>
#include <dxtk/WICTextureLoader.h>
#include <dxtex/DirectXTex.h>
#include <fstream>
#include <tuple>

namespace Pg::Graphics::Loader
{
	using Pg::Util::Helper::ResourceHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Graphics::Helper::GraphicsResourceHelper;

	AssetBasic2DLoader::AssetBasic2DLoader() : _DXStorage(LowDX11Storage::GetInstance())
	{

	}

	void AssetBasic2DLoader::LoadTexture1D(const std::string& path, RenderTexture1D* outTextureData)
	{
		LoadInternalRenderTexture2D(path, outTextureData);

		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);
	}

	void AssetBasic2DLoader::LoadTexture2D(const std::string& path, RenderTexture2D* outTextureData)
	{
		LoadInternalRenderTexture2D(path, outTextureData);
		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);

		//Width / Height 기록.
		ID3D11Texture2D* tTexture2D = nullptr;
		HR(outTextureData->GetResource()->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tTexture2D));

		D3D11_TEXTURE2D_DESC tTextureDesc;
		tTexture2D->GetDesc(&tTextureDesc);

		//Width / Height 옮기기.
		outTextureData->_fileWidth = tTextureDesc.Width;
		outTextureData->_fileHeight = tTextureDesc.Height;
	}

	void AssetBasic2DLoader::LoadTexture2DArray(bool isDDS, const std::string& path, RenderTexture2DArray* outTextureData)
	{
		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);

		if (isDDS)
		{
			LoadInternalRenderTexture2D(path, outTextureData);
		}
		else
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
	}

	void AssetBasic2DLoader::LoadTextureCube(const std::string& path, RenderTextureCube* outTextureData)
	{
		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);
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
			//HR(DirectX::CreateDDSTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
			//HR(DirectX::CreateDDSTextureFromFile(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

			////이게 되는게 목표.
			//UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //밉맵 생성 허용 위해.
			//UINT tCPUAccessFlags = 0;
			//UINT tMiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			//이게 되는게 목표.
			UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE; //밉맵 생성 허용 위해.
			UINT tCPUAccessFlags = 0;
			UINT tMiscFlags = 0;

			HR(DirectX::CreateDDSTextureFromFileEx(_DXStorage->_device, _DXStorage->_deviceContext, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
				DirectX::DDS_LOADER_DEFAULT, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
		
		}
		else if (ResourceHelper::IsResourceTGA(path))
		{
			auto image = std::make_unique<DirectX::ScratchImage>();
			auto mipChain = std::make_unique<DirectX::ScratchImage>();

			//TGA 파일 자체 로딩.
			HR(DirectX::LoadFromTGAFile(tWStrPath.c_str(), DirectX::TGA_FLAGS_NONE, nullptr, *image));
			
			//TGA 기준 MipMap 생성.
			HR(DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, (size_t)NULL, *(mipChain.get())));

			//내부적으로 처리됨.
			HR(DirectX::CreateShaderResourceView(
				_DXStorage->_device, mipChain->GetImages(), mipChain->GetImageCount(), mipChain->GetMetadata(), &(outTextureData->GetSRV())));

			//ID3D11Resource 자체 역시 포함해야 일괄적으로 쓰일 수 있다.
			ID3D11Resource* res = nullptr;
			outTextureData->GetSRV()->GetResource(&res);
			outTextureData->GetResource() = res;
			
			//GenerateMips 테스트.
			_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());
		}
		else
		{
			UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //밉맵 생성 허용 위해.
			UINT tCPUAccessFlags = 0;
			UINT tMiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			//HR(DirectX::CreateWICTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
			//	DirectX::WIC_LOADER_DEFAULT, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

			//FORCE -> RGBA32
			HR(DirectX::CreateWICTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
				DirectX::WIC_LOADER_FORCE_RGBA32, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
		
			//GenerateMips 테스트.
			_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());
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

		for (UINT sliceIndex = 0; sliceIndex < arrayTextureDesc.ArraySize; ++sliceIndex)
		{
			// Loop through each mip level
			for (UINT mipLevel = 0; mipLevel < arrayTextureDesc.MipLevels; ++mipLevel)
			{
				// Copy data from the corresponding source texture and mipmap level to the current slice and mipmap level of the Texture2DArray
				_DXStorage->_deviceContext->CopySubresourceRegion(
					arrayTexture2D,                      // Destination (Texture2DArray)
					D3D11CalcSubresource(mipLevel, sliceIndex, arrayTextureDesc.MipLevels),
					0, 0, 0,                             // DestX, DestY, DestZ
					tTexture2DStoreVec[sliceIndex],      // Source resource (Texture2D)
					D3D11CalcSubresource(mipLevel, 0, tUniformMipLevels), // SrcSubresource (source mipmap level)
					nullptr                              // pSrcBox (use nullptr to copy the entire resource)
				);
			}
		}

		HR(arrayTexture2D->QueryInterface(__uuidof(ID3D11Resource), (void**)&outTextureData->GetResource()));


		D3D11_SHADER_RESOURCE_VIEW_DESC tSrvArrayDesc;
		ZeroMemory(&tSrvArrayDesc, sizeof(tSrvArrayDesc));
		tSrvArrayDesc.Format = tUniformDXGIFormat;
		tSrvArrayDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		tSrvArrayDesc.Texture2DArray.MipLevels = tUniformMipLevels;
		tSrvArrayDesc.Texture2DArray.ArraySize = cnt;

		ID3D11ShaderResourceView*& textureArraySRV = outTextureData->GetSRV();
		HR(_DXStorage->_device->CreateShaderResourceView(arrayTexture2D, &tSrvArrayDesc, &textureArraySRV));

		//GenerateMips 테스트.
		//_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());

		assert("");
	}



}