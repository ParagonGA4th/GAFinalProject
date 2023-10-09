#pragma once
#include "D3DBufferInfo.h"

/// <summary>
/// AssetScene을 인풋레이아웃을 기반으로 VB와 IB로 만들어주는 클래스.
/// 후에 리소스를 D3D와 연동하는 역할도 수행할 예정.
/// </summary>

namespace Pg::Graphics
{
	class AssetSceneData;
}

namespace Pg::Graphics::Helper
{
	class BufferParser
	{
	public:
		BufferParser();
		~BufferParser();

		//일단은 1차 그래픽엔진에서 Screen->World Space 1차 Layout에 맞추도록 세팅.
		static void Asset3DModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, bool isSkinned, Pg::Graphics::AssetSceneData* assetSceneData);
	
	private:
		static void AssetStaticModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, Pg::Graphics::AssetSceneData* assetSceneData);
		static void AssetSkinnedModelToD3DBuffer(Pg::Graphics::D3DBufferInfo& outBufferInfo, Pg::Graphics::AssetSceneData* assetSceneData);
	};
}


