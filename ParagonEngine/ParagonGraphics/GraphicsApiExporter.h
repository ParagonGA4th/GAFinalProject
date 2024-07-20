#pragma once
#include "GraphicsDLLExporter.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonMath/PgMath.h"
#include <string>
/// <summary>
/// ParagonAPI에서 발동할 Graphics API 함수들 모아서 호출하기 위한 클래스.
/// Singleton-CPP로 ParagonAPI와 함께 공유될 예정.
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

		//Engine의 SceneSystem에서 미리 호출될 수 있느 함수들.
		
		//Cubemap의 순서를 저장하기 전에, 어느 리소스의 이름을 어느 인덱스에 넣을지 세팅하는 함수.
		PARAGON_GRAPHICS_DLL void SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index);

		//어느 인덱스의 큐브맵을 현재 렌더되는 큐브맵으로 설정할지 결정.
		PARAGON_GRAPHICS_DLL void SetRenderedCubemapIndex(unsigned int index);

		PARAGON_GRAPHICS_DLL void RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);

		PARAGON_GRAPHICS_DLL void RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//void* 반환, 나중에 해당 포인터를 집어넣어서 캐스팅 후 함수를 활용할 수 있게 세팅.
		PARAGON_GRAPHICS_DLL void* GetRenderMaterialWithName(const std::string& materialName);

		//Material 세팅 관련 함수들.
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

		//PostProcessing 껐다키는 것도 여기에서 할 수 있어야.
		//Ex. DXTK's DualPostProcessing 같은 걸로 인 & 아웃.
		PARAGON_GRAPHICS_DLL void ScreenSpace_FadeIn();
		PARAGON_GRAPHICS_DLL void ScreenSpace_FadeOut();

		PARAGON_GRAPHICS_DLL bool GetIsFadingIn();
		PARAGON_GRAPHICS_DLL bool GetIsFadingOut();

		//Player의 Position : Graphics한테 전달.
		PARAGON_GRAPHICS_DLL void RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos);
		
		//Manual Switch Mode 일 경우, billboard 상태일 때 Texture Index 스위칭 가능, nullptr 검사 필요
		PARAGON_GRAPHICS_DLL unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);
	private:
		GraphicsMain* _graphicsMain{ nullptr };
		
	};
}


