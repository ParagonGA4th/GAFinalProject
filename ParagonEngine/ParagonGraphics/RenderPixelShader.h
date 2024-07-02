#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// 클라이언트에게 노출되는,
/// 렌더링 파이프라인에 후에 바인딩되는 셰이더를 관리하기 위해 구성.
/// 게임엔진 클라이언트에서 실제로 Pixel Shader로 지칭되는 대상.
/// </summary>

namespace Pg::Graphics
{
	namespace Loader
	{
		class AssetCombinedLoader;
	}
}

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderPixelShader : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetCombinedLoader;
	public:
		RenderPixelShader(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		virtual ~RenderPixelShader();

	public:
		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	public:
		void Bind();
		void Unbind();
		ID3D11PixelShader* GetPixelShader();

	private:
		LowDX11Storage* _DXStorage = nullptr;

		// 쉐이더 바이트코드를 멤버에 저장
		ID3DBlob* _byteCode = nullptr;
		ID3D11PixelShader* _shader = nullptr;
	};
}
