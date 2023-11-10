#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

/// <summary>
/// 실제 Texture2D과 1대1 대응되는 클래스. 게임엔진에서 저장된 Texture2D와 연동되어 보관된다.
/// == Image
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTexture2D : public Pg::Data::Resources::GraphicsResource
	{
	public:
		RenderTexture2D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture2D();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		ID3D11ShaderResourceView*& GetSRV();
		ID3D11Resource*& GetResource();

		std::wstring GetFilePath();
	
	private:
		//Load하는 로직이 이를 채워야 한다!
		ID3D11ShaderResourceView* _textureSRV = nullptr;
		ID3D11Resource* _textureBuffer = nullptr;
	};

}

