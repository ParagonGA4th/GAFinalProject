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
		void LoadCustomRenderMaterial(const std::string& path, RenderMaterial* renderMat);

		//Default VS/PS와 Mesh의 정보를 활용해서, DefaultMaterial의 인스턴스를 만든다.
		//해당 Material의 Instance는 DefaultMaterial_MeshName.pgmat으로 기록될 것이다. 
		//역으로 실제 파일 탐색기에 파일이 생성되는 것은 아니다.
		//Mesh가 중복되더라도, 각각의 Material을 만드는것을 막기 위해서이다.
		void LoadDefaultRenderMaterial(const std::string& defInstMatName, RenderMaterial* renderMat);
	
	private:
		std::unique_ptr<Pg::Graphics::MaterialParser> _matParser;

	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};

}
