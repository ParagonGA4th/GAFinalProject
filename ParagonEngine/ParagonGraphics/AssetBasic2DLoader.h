#pragma once
#include <string>

/// <summary>
/// 2D ���ҽ��� �����ϴ� Loader Ŭ���� (Font / Image)
/// ���� �ϳ��� �ν��Ͻ��� ������ �۵��Ǿ�� �Ѵ�.
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

		//Texture1D�� �ε��Ѵ�.
		void LoadTexture1D(const std::string& path, RenderTexture1D* outTextureData);

		//Texture2D(Image)�� �ε��Ѵ�.
		void LoadTexture2D(const std::string& pathValue, RenderTexture2D* outTextureData);

		//Texture2DArray�� �ε��Ѵ�.
		void LoadTexture2DArray(bool isDDS, const std::string& path, RenderTexture2DArray* outTextureData);

		//TextureCube�� �ε��Ѵ�.
		void LoadTextureCube(const std::string& path, RenderTextureCube* outTextureData);

		//Font(Text)�� �ε��Ѵ�.
		void LoadFont(const std::string& path, RenderFont* outFontData);

		//Cubemap(Cubemap)�� �ε��Ѵ�.
		void LoadCubemap(const std::string& path, RenderCubemap* outCubemapData);

		//RenderTexture�� ����Ʈ�κ��� RenderTexture2DArray�� �����.
		void MultipleRenderTexture2DToTexture2DArray(RenderTexture2D** textureSrc, unsigned int cnt, RenderTexture2DArray* outTextureData);

	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;

	private:
		//�ϴ� ���� RenderTexture ���ü���� ������ Create �Լ��� �ʿ�� ���� ������, 
		//���������� �ϰ������� ȣ��Ǵ� �ڵ��̴�.
		void LoadInternalRenderTexture2D(const std::string& path, RenderTexture* outTextureData);

	private:
		//Cubemap�� ����� ���� ������ �Լ�.
		void PlaceCubemapBuffers(RenderCubemap* outCubemapData);
		void PlaceCubemapSRV(const std::wstring& path, RenderCubemap* outCubemapData);

	};
}

