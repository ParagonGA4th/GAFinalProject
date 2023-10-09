#pragma once
#include "D3DBuffers.h"

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

		//일단은 
		//static D3DBuffers Asset3DModelToD3DBuffer(Pg::Graphics::AssetSceneData* assetSceneData);
	};
}


