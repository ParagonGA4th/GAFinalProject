#pragma once
#include <string>

/// <summary>
/// 2D 리소스를 전담하는 Loader 클래스 (Font / Image)
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderTexture2D;
	class RenderFont;
}

namespace Pg::Graphics::Loader
{
	class AssetBasic2DLoader
	{
	public:
		AssetBasic2DLoader();

		//Texture2D(Image)를 로드한다.
		void LoadTexture2D(const std::string& path, RenderTexture2D* outTextureData);

		//Font(Text)를 로드한다.
		void LoadFont(const std::string& path, RenderFont* outFontData);


	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};
}

