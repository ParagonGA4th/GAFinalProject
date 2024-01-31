#pragma once
#include "RenderTexture.h"

/// <summary>
/// 실제 Texture2D과 1대1 대응되는 클래스. 게임엔진에서 저장된 Texture2D와 연동되어 보관된다.
/// == Image
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	namespace Loader
	{
		class AssetBasic2DLoader;
	}
}

namespace Pg::Graphics
{
	class RenderTexture2D : public RenderTexture
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture2D();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		unsigned int GetFileWidth();
		unsigned int GetFileHeight();

	private:
		unsigned int _fileWidth = 0;
		unsigned int _fileHeight = 0;
	};

}

