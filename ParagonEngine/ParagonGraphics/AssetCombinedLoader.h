#pragma once
#include <memory>
#include <string>

/// <summary>
/// Shader / Material �� ���ҽ��� �����ϴ� Loader Ŭ���� (Shader / Material) +a
/// ���� �ϳ��� �ν��Ͻ��� ������ �۵��Ǿ�� �Ѵ�.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class RenderVertexShader;
	class RenderPixelShader;
	class RenderMaterial;
	
	class MaterialParser;
	class AnimationParser;

	class RenderAnimation;
}

namespace Pg::Graphics::Loader
{
	class AssetCombinedLoader
	{
	public:
		AssetCombinedLoader();
		~AssetCombinedLoader();

		//RenderVertexShader �ε�.
		void LoadRenderVertexShader(const std::string& path, RenderVertexShader* renderVS);
		void LoadRenderPixelShader(const std::string& path, RenderPixelShader* renderPS);

		//�̸� VS/PS�� �ε�� ���¿�����, Material�� �ε�Ǿ�� �Ѵ�.
		void LoadCustomRenderMaterial(const std::string& path, RenderMaterial* renderMat);

		//Default VS/PS�� Mesh�� ������ Ȱ���ؼ�, DefaultMaterial�� �ν��Ͻ��� �����.
		//�ش� Material�� Instance�� DefaultMaterial_MeshName.pgmat���� ��ϵ� ���̴�. 
		//������ ���� ���� Ž���⿡ ������ �����Ǵ� ���� �ƴϴ�.
		//Mesh�� �ߺ��Ǵ���, ������ Material�� ����°��� ���� ���ؼ��̴�.
		void LoadDefaultRenderMaterial(const std::string& defInstMatName, RenderMaterial* renderMat);
	
		//Material Parser ������ Material ID Remapping (���� �ٲ� ����) ��ü!
		void RemapMaterialIdForUninitMaterials();

		//�߰��� �߰��Ǵ� Material Parser ������ Material ID �Ϻ� ����, �߰���!
		void RemapAppendedMatID();

		//Scene Parsing ������ �� ��� Material Init State ����.
		void ResetAllKnownMatInitStates();

		//Animation Load.
		void LoadAnimation(const std::string& path, RenderAnimation* anim);
	private:
		std::unique_ptr<Pg::Graphics::MaterialParser> _matParser;
		std::unique_ptr<Pg::Graphics::AnimationParser> _animParser;

	private:
		Pg::Graphics::LowDX11Storage* _DXStorage;
	};

}
