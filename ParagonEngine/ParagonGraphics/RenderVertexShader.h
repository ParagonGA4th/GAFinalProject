#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// 게임엔진 클라이언트에서 실제로 Vertex Shader로 지칭되는 대상.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderVertexShader : public Pg::Data::Resources::GraphicsResource
	{
	public:
		RenderVertexShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		virtual ~RenderVertexShader();

	public:
		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	private:
		LowDX11Storage* _DXStorage = nullptr;
	};
}

