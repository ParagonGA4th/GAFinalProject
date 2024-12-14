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
#include "../ParagonUtil/CustomAssert.h"

#include "DirectXTexEXR.h"
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

	void AssetBasic2DLoader::LoadTexture2D(const std::string& pathValue, RenderTexture2D* outTextureData)
	{
		std::string path;
		//if (pathValue.find("#") != std::string::npos)
		//{
		//	//Texture2D ����, String ������ �ɰ��� #���� ������.
		//	std::stringstream tPathSS(pathValue);
		//	std::string segment;
		//	std::vector<std::string> seglist;
		//
		//	while (std::getline(tPathSS, segment, '#'))
		//	{
		//		seglist.push_back(segment);
		//	}
		//
		//	path = seglist.at(0);
		//
		//	if (seglist.size() > 1)
		//	{
		//		outTextureData->_isIgnoreSRGB = std::stoi(seglist.at(1));
		//	}
		//}
		//else
		//{
			//�� ã������ �� �Ҵ�.
			path = pathValue;
		//}

		LoadInternalRenderTexture2D(path, outTextureData);
		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);

		//Width / Height ���.
		ID3D11Texture2D* tTexture2D = nullptr;
		HR(outTextureData->GetResource()->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tTexture2D));

		D3D11_TEXTURE2D_DESC tTextureDesc;
		tTexture2D->GetDesc(&tTextureDesc);

		//Width / Height �ű��.
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

			//����, ���� PGT2ARR���� SingleRenderTexture2DArray�� ������ �̵�. ������ RenderTexture2D���� ���͵� ���·ε� �����ϴ� ��.
			std::copy(tSingleRenderTexture2DArray.begin(), tSingleRenderTexture2DArray.end(),
				std::back_inserter(outTextureData->_singleRenderTexture2DArray));
		}
	}

	void AssetBasic2DLoader::LoadTextureCube(const std::string& path, RenderTextureCube* outTextureData)
	{
		outTextureData->_textureExt = GraphicsResourceHelper::GetTexExtFromPath(path);
		LoadInternalRenderTexture2D(path, outTextureData);
	}

	void AssetBasic2DLoader::LoadFont(const std::string& path, RenderFont* outFontData)
	{
		//�ѱ� Path�� �Ѿ���� ���ϰ�����, �ϴ���.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		outFontData->_font = std::make_unique<DirectX::SpriteFont>(_DXStorage->_device, tWStrPath.c_str());
	}

	void AssetBasic2DLoader::LoadCubemap(const std::string& path, RenderCubemap* outCubemapData)
	{
		//�ѱ� Path�� �Ѿ���� ���ϰ�����, �ϴ���.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		PlaceCubemapBuffers(outCubemapData);
		PlaceCubemapSRV(tWStrPath, outCubemapData);
		//Sampler�� ���������� ������ �۵��� ���̴�.
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
		CustomAssert(ResourceHelper::IsResourceDDS(path));
		HR(DirectX::CreateDDSTextureFromFile(_DXStorage->_device, path.c_str(), &tTexture, &(outCubemapData->_srv)));
	}

	void AssetBasic2DLoader::LoadInternalRenderTexture2D(const std::string& path, RenderTexture* outTextureData)
	{
		//�ѱ� Path�� �Ѿ���� ���ϰ�����, �ϴ���.
		std::wstring tWStrPath;
		tWStrPath.assign(path.begin(), path.end());

		if (ResourceHelper::IsResourceDDS(path))
		{
			//HR(DirectX::CreateDDSTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
			//HR(DirectX::CreateDDSTextureFromFile(_DXStorage->_device, tWStrPath.c_str(), &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

			////�̰� �Ǵ°� ��ǥ.
			//UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //�Ӹ� ���� ��� ����.
			//UINT tCPUAccessFlags = 0;
			//UINT tMiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			//�̰� �Ǵ°� ��ǥ.
			UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE; //�Ӹ� ���� ��� ����.
			UINT tCPUAccessFlags = 0;
			UINT tMiscFlags = 0;

			HR(DirectX::CreateDDSTextureFromFileEx(_DXStorage->_device, _DXStorage->_deviceContext, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
				DirectX::DDS_LOADER_DEFAULT, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

		}
		else if (ResourceHelper::IsResourceTGA(path))
		{
			auto image = std::make_unique<DirectX::ScratchImage>();
			auto mipChain = std::make_unique<DirectX::ScratchImage>();

			//TGA ���� ��ü �ε�.
			HR(DirectX::LoadFromTGAFile(tWStrPath.c_str(), DirectX::TGA_FLAGS_NONE, nullptr, *image));

			//TGA ���� MipMap ����.
			HR(DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, (size_t)NULL, *(mipChain.get())));


			//���������� ó����.
			HR(DirectX::CreateShaderResourceView(
				_DXStorage->_device, mipChain->GetImages(), mipChain->GetImageCount(), mipChain->GetMetadata(), &(outTextureData->GetSRV())));

			//ID3D11Resource ��ü ���� �����ؾ� �ϰ������� ���� �� �ִ�.
			ID3D11Resource* res = nullptr;
			outTextureData->GetSRV()->GetResource(&res);
			outTextureData->GetResource() = res;

			//GenerateMips �׽�Ʈ.
			//_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());
		}
		else if (ResourceHelper::IsResourceEXR(path))
		{
			//����Ʈ���� ���� ����, �Ӹ��� ���� ���� ����. �Ӹ��� ������ ����.
			auto image = std::make_unique<DirectX::ScratchImage>();

			//EXR ���� ��ü �ε�.
			HR(DirectX::LoadFromEXRFile(tWStrPath.c_str(), nullptr, *image));
			HR(DirectX::CreateShaderResourceView(_DXStorage->_device, image->GetImages(), image->GetImageCount(), image->GetMetadata(), &(outTextureData->GetSRV())));

			//ID3D11Resource ��ü ���� �����ؾ� �ϰ������� ���� �� �ִ�.
			ID3D11Resource* res = nullptr;
			outTextureData->GetSRV()->GetResource(&res);
			outTextureData->GetResource() = res;

			//Mipmap�� �ƿ� ������ �ʴ´�.
		}
		else
		{
			UINT tBindingFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; //�Ӹ� ���� ��� ����.
			UINT tCPUAccessFlags = 0;
			UINT tMiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			//HR(DirectX::CreateWICTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
			//	DirectX::WIC_LOADER_DEFAULT, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

			//FORCE -> RGBA32
			//HR(DirectX::CreateWICTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
			//	DirectX::WIC_LOADER_FORCE_RGBA32, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));
			//RGBA32?

			//Normal : Default
			//Ignore ���� : �޶���.
			// ���� Asset���������� ���� ���Ӱ� Ignore_SRGB Force.
			//DirectX::WIC_LOADER_FLAGS tWICColFlag = (outTextureData->_isIgnoreSRGB) ? DirectX::WIC_LOADER_IGNORE_SRGB : DirectX::WIC_LOADER_DEFAULT;
			DirectX::WIC_LOADER_FLAGS tWICColFlag = DirectX::WIC_LOADER_IGNORE_SRGB;

			HR(DirectX::CreateWICTextureFromFileEx(_DXStorage->_device, tWStrPath.c_str(), NULL, D3D11_USAGE_DEFAULT, tBindingFlags, tCPUAccessFlags, tMiscFlags,
				tWICColFlag, &(outTextureData->GetResource()), &(outTextureData->GetSRV())));

			//GenerateMips �׽�Ʈ.
			_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());
		}
	}

	void AssetBasic2DLoader::MultipleRenderTexture2DToTexture2DArray(RenderTexture2D** textureSrc, unsigned int cnt, RenderTexture2DArray* outTextureData)
	{
		//�������� Texture2D�� Width / Height / MipLevels / Format / ArraySize.
		//�ϳ��� ���ϵǾ�� �ϴµ�, �̸� ����üũ�ϴ� �ڵ�.
		std::vector<std::tuple<UINT, UINT, UINT, DXGI_FORMAT, UINT>> tSingleTextureStoreVec;
		std::vector<ID3D11Texture2D*> tTexture2DStoreVec;

		for (int i = 0; i < cnt; i++)
		{
			ID3D11Texture2D* tTexture2D = nullptr;
			HR(textureSrc[i]->GetResource()->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tTexture2D));
			tTexture2DStoreVec.push_back(tTexture2D);

			D3D11_TEXTURE2D_DESC tTextureDesc;
			tTexture2D->GetDesc(&tTextureDesc);

			//�̷μ� ���ҽ��� �⺻���� Width, Height, Mipmap ���� ���� �޾ƿ� �� �ְ� �Ǿ���.
			tSingleTextureStoreVec.push_back(std::make_tuple(tTextureDesc.Width, tTextureDesc.Height, tTextureDesc.MipLevels,
				tTextureDesc.Format, tTextureDesc.ArraySize));
		}

		//���� Texture2D���� Texture2DArray�� ����� ���� ���������� �Ǵ�.
		UINT tUniformWidth = std::get<0>(tSingleTextureStoreVec.at(0));
		UINT tUniformHeight = std::get<1>(tSingleTextureStoreVec.at(0));
		UINT tUniformMipLevels = std::get<2>(tSingleTextureStoreVec.at(0));
		DXGI_FORMAT tUniformDXGIFormat = std::get<3>(tSingleTextureStoreVec.at(0));
		UINT tUniformArraySize = std::get<4>(tSingleTextureStoreVec.at(0));

		for (auto& [bWidth, bHeight, bMipLevels, bFormat, bArraySize] : tSingleTextureStoreVec)
		{
			//��� Texture2D���� ���� ������Ƽ�� �����ؾ� Texture2DArray�� ������� �� �ִ�.
			assert(tUniformWidth == bWidth);
			assert(tUniformHeight == bHeight);
			assert(tUniformMipLevels == bMipLevels);
			assert(tUniformDXGIFormat == bFormat);
			assert(tUniformArraySize == bArraySize);
		}

		//Texture2DArray�� ������ �����.

		//��ϵǾ��� Texture2D�� Ȱ��.
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

		//Resource�����ͼ� ����.
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

		//GenerateMips �׽�Ʈ.
		//_DXStorage->_deviceContext->GenerateMips(outTextureData->GetSRV());

		assert("");
	}



}