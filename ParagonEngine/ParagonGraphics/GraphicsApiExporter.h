#pragma once
#include "GraphicsDLLExporter.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonMath/PgMath.h"
#include <string>
/// <summary>
/// ParagonAPI���� �ߵ��� Graphics API �Լ��� ��Ƽ� ȣ���ϱ� ���� Ŭ����.
/// Singleton-CPP�� ParagonAPI�� �Բ� ������ ����.
/// </summary>

namespace Pg::Graphics
{
	class GraphicsMain;
}

namespace Pg::Graphics
{
	class GraphicsApiExporter
	{
	public:
		PARAGON_GRAPHICS_DLL GraphicsApiExporter();
		PARAGON_GRAPHICS_DLL ~GraphicsApiExporter();
		
		void Initialize(GraphicsMain* graphicsMain);

		//Engine�� SceneSystem���� �̸� ȣ��� �� �ִ� �Լ���.
		
		//Cubemap�� ������ �����ϱ� ����, ��� ���ҽ��� �̸��� ��� �ε����� ������ �����ϴ� �Լ�.
		PARAGON_GRAPHICS_DLL void SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index);

		//��� �ε����� ť����� ���� �����Ǵ� ť������� �������� ����.
		PARAGON_GRAPHICS_DLL void SetRenderedCubemapIndex(unsigned int index);

		PARAGON_GRAPHICS_DLL void RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);

		PARAGON_GRAPHICS_DLL void RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//void* ��ȯ, ���߿� �ش� �����͸� ����־ ĳ���� �� �Լ��� Ȱ���� �� �ְ� ����.
		PARAGON_GRAPHICS_DLL void* GetRenderMaterialWithName(const std::string& materialName);

		//Material ���� ���� �Լ���.
		PARAGON_GRAPHICS_DLL void Material_SetBoolVS(void* renderMat, const std::string& varName, bool value);
		PARAGON_GRAPHICS_DLL void Material_SetBoolPS(void* renderMat, const std::string& varName, bool value);
		PARAGON_GRAPHICS_DLL void Material_SetUnsignedIntVS(void* renderMat, const std::string& varName, unsigned int value);
		PARAGON_GRAPHICS_DLL void Material_SetUnsignedIntPS(void* renderMat, const std::string& varName, unsigned int value);
		PARAGON_GRAPHICS_DLL void Material_SetIntVS(void* renderMat, const std::string& varName, int value);
		PARAGON_GRAPHICS_DLL void Material_SetIntPS(void* renderMat, const std::string& varName, int value);
		PARAGON_GRAPHICS_DLL void Material_SetFloatVS(void* renderMat, const std::string& varName, float value);
		PARAGON_GRAPHICS_DLL void Material_SetFloatPS(void* renderMat, const std::string& varName, float value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat2VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat2PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat3VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat3PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat4VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value);
		PARAGON_GRAPHICS_DLL void Material_SetFloat4PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value);

		//PostProcessing ����Ű�� �͵� ���⿡�� �� �� �־��.
		//Ex. DXTK's DualPostProcessing ���� �ɷ� �� & �ƿ�.
		PARAGON_GRAPHICS_DLL void ScreenSpace_FadeIn();
		PARAGON_GRAPHICS_DLL void ScreenSpace_FadeOut();

		PARAGON_GRAPHICS_DLL bool GetIsFadingIn();
		PARAGON_GRAPHICS_DLL bool GetIsFadingOut();

		//Player�� Position : Graphics���� ����.
		PARAGON_GRAPHICS_DLL void RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos);
		
		//Manual Switch Mode �� ���, billboard ������ �� Texture Index ����Ī ����, nullptr �˻� �ʿ�
		PARAGON_GRAPHICS_DLL unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);
	private:
		GraphicsMain* _graphicsMain{ nullptr };
		
	};
}


