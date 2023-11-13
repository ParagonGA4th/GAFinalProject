#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// 게임엔진에서 실제로 Shader로 지칭되는 대상.
/// HLSL 파싱, 연결 등 담당.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderShader : public Pg::Data::Resources::GraphicsResource
	{
	public:
		RenderShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);

	public:
		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	protected:
		LowDX11Storage* _DXStorage;

	public:

		// 쉐이더 바이트코드를 멤버에 저장
		HRESULT LoadShader(const std::string& filePath);
		ID3DBlob* _byteCode;
	};
}

