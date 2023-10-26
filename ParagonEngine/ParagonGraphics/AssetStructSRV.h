#pragma once
#include <string>
#include <d3d11.h>

/// <summary>
/// TextureCluster에 쓰일 Type-Path-SRV를 묶어놓을 구조체.
/// </summary>

namespace Pg::Graphics
{
	struct AssetTextureSRV
	{
	public:
		AssetTextureSRV() : type(), path(), texture(nullptr) {}
		AssetTextureSRV(const AssetTextureSRV& rhs) : type(rhs.type), path(rhs.path), texture(rhs.texture) {}
		
		std::string type;
		std::string path;
		ID3D11ShaderResourceView* texture;
	};
}
