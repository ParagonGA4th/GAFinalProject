#pragma once
#include <string>
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectData.h"
#include "../ParagonData/VisualEffectRenderObject.h"

/// <summary>
/// GraphicsAPI 전반을 제어할 수 있게 열어놓은 API용 클래스.
/// </summary>

namespace Pg::Graphics
{
	class GraphicsApiExporter;
}

namespace Pg::API::Graphics
{
	class PgGraphics
	{
	public:
		PgGraphics();
		~PgGraphics();

		void Initialize();

		void SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index);

		//어느 인덱스의 큐브맵을 현재 렌더되는 큐브맵으로 설정할지 결정.
		void SetRenderedCubemapIndex(unsigned int index);

		void RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);

		void RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//void* 반환, 나중에 해당 포인터를 집어넣어서 캐스팅 후 함수를 활용할 수 있게 세팅.
		void* GetRenderMaterialWithName(const std::string& materialName);

		//Material 세팅 관련 함수들.
		void Material_SetBoolVS(void* renderMat, const std::string& varName, bool value);
		void Material_SetBoolPS(void* renderMat, const std::string& varName, bool value);
		void Material_SetUnsignedIntVS(void* renderMat, const std::string& varName, unsigned int value);
		void Material_SetUnsignedIntPS(void* renderMat, const std::string& varName, unsigned int value);
		void Material_SetIntVS(void* renderMat, const std::string& varName, int value);
		void Material_SetIntPS(void* renderMat, const std::string& varName, int value);
		void Material_SetFloatVS(void* renderMat, const std::string& varName, float value);
		void Material_SetFloatPS(void* renderMat, const std::string& varName, float value);
		void Material_SetFloat2VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value);
		void Material_SetFloat2PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value);
		void Material_SetFloat3VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value);
		void Material_SetFloat3PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value);
		void Material_SetFloat4VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value);
		void Material_SetFloat4PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value);

		//ScreenSpace Effect.
		void ScreenSpace_FadeIn();
		void ScreenSpace_FadeOut();

		bool GetIsFadingIn();
		bool GetIsFadingOut();

		//Player의 위치, 그래픽스 엔진도 알게 하자.
		void RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos);

		unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);

	private:
		Pg::Graphics::GraphicsApiExporter* _graphicsApiExporter{ nullptr };
	};
}
