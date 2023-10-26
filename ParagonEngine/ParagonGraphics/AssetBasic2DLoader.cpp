#include "AssetBasic2DLoader.h"
#include "LowDX11Storage.h"

#include "RenderTexture2D.h"
#include "RenderFont.h"

#include "DX11Headers.h"
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

	

}