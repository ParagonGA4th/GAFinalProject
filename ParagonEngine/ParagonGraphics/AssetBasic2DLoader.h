#pragma once
#include <string>

/// <summary>
/// 2D 리소스를 전담하는 Loader 클래스 (Font / Image)
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderTexture;
	class RenderTexture1D;
	class RenderTexture2D;
	class RenderTexture2DArray;
	class RenderTextureCube;
	class RenderFont;
	class RenderCubemap;
}

namespace Pg::Graphics::Loader
{
	class AssetBasic2DLoader
	{
	public:
		AssetBasic2DLoader();

		//Texture1D를 로드한다.
		void LoadTexture1D(const std::string& path, RenderTexture1D* outTextureData);

		//Texture2D(Image)를 로드한다.
		void LoadTexture2D(const std::string& pathValue, RenderTexture2D* outTextureData);

		//Texture2DArray를 로드한다.
		void LoadTexture2DArray(bool isDDS, const std::string& path, RenderTexture2DArray* outTextureData);

		//TextureCube를 로드한다.
		void LoadTextureCube(const std::string& path, RenderTextureCube* outTextureData);

		//Font(Text)를 로드한다.
		void LoadFont(const std::string& path, RenderFont* outFontData);

		//Cubemap(Cubemap)를 로드한다.
		void LoadCubemap(const std::string& path, RenderCubemap* outCubemapData);

		//RenderTexture의 리스트로부터 RenderTexture2DArray를 만든다.
		void MultipleRenderTexture2DToTexture2DArray(RenderTexture2D** textureSrc, unsigned int cnt, RenderTexture2DArray* outTextureData);

	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;

	private:
		//일단 현재 RenderTexture 상속체들이 별개의 Create 함수를 필요로 하지 않으니, 
		//내부적으로 일괄적으로 호출되는 코드이다.
		void LoadInternalRenderTexture2D(const std::string& path, RenderTexture* outTextureData);

	private:
		//Cubemap을 만들기 위한 별도의 함수.
		void PlaceCubemapBuffers(RenderCubemap* outCubemapData);
		void PlaceCubemapSRV(const std::wstring& path, RenderCubemap* outCubemapData);

	};
}

