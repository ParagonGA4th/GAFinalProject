#pragma once
#include "RenderTexture.h"

/// <summary>
/// 실제 TextureCube와 1대1 대응되는 클래스. 게임엔진에서 저장된 TextureCube와 연동되어 보관된다.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTextureCube : public RenderTexture
	{
	public:
		RenderTextureCube(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTextureCube();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;
	};
}

