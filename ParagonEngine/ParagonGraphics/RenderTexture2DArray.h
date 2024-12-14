#pragma once
#include "RenderTexture.h"
#include <vector>
/// <summary>
/// 실제 Texture2DArray과 1대1 대응되는 클래스. 게임엔진에서 저장된 Texture2DArray와 연동되어 보관된다.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTexture2D;
	namespace Loader { AssetBasic2DLoader; }

	class RenderTexture2DArray : public RenderTexture
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderTexture2DArray(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture2DArray();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		std::vector<RenderTexture2D*> GetSingleRenderTexture2DArray() { return _singleRenderTexture2DArray; }
	private:
		//내부적으로 로드된 값들.
		std::vector<RenderTexture2D*> _singleRenderTexture2DArray;
	};
}
