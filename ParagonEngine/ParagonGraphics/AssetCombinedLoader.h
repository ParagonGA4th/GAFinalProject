#pragma once
#include <memory>
#include <string>

/// <summary>
/// Shader / Material 등 리소스를 전담하는 Loader 클래스 (Shader / Material) +a
/// 전부 하나의 인스턴스를 가지고 작동되어야 한다.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderVertexShader;
	class RenderPixelShader;
	class RenderMaterial;

	class MaterialParser;
}

namespace Pg::Graphics::Loader
{
	class AssetCombinedLoader
	{
	public:
		AssetCombinedLoader();
		~AssetCombinedLoader();

		//RenderVertexShader 로드.
		void LoadRenderVertexShader(const std::string& path, RenderVertexShader* renderVS);
		void LoadRenderPixelShader(const std::string& path, RenderPixelShader* renderPS);

		//미리 VS/PS이 로드된 상태에서만, Material이 로드되어야 한다.
		void LoadRenderMaterial(const std::string& path, RenderMaterial* renderMat);
	
	private:
		std::unique_ptr<Pg::Graphics::MaterialParser> _matParser;

	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};

}
