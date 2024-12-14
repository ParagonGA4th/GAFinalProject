#pragma once
#include <string>
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/VisualEffectData.h"
#include "../ParagonData/VisualEffectRenderObject.h"

/// <summary>
/// GraphicsAPI ������ ������ �� �ְ� ������� API�� Ŭ����.
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

		//��� �ε����� ť����� ���� �����Ǵ� ť������� �������� ����.
		void SetRenderedCubemapIndex(unsigned int index);

		void RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);

		void RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//void* ��ȯ, ���߿� �ش� �����͸� ����־ ĳ���� �� �Լ��� Ȱ���� �� �ְ� ����.
		void* GetRenderMaterialWithName(const std::string& materialName);

		//Material ���� ���� �Լ���.
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

		//Player�� ��ġ, �׷��Ƚ� ������ �˰� ����.
		void RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos);

		unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);

	private:
		Pg::Graphics::GraphicsApiExporter* _graphicsApiExporter{ nullptr };
	};
}
