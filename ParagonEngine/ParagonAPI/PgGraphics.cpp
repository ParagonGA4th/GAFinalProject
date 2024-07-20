#include "PgGraphics.h"
#include "../ParagonGraphics/GraphicsApiExporter.h"

#include <singleton-cpp/singleton.h>

namespace Pg::API::Graphics
{
	PgGraphics::PgGraphics()
	{

	}

	PgGraphics::~PgGraphics()
	{

	}

	void PgGraphics::Initialize()
	{
		auto& tSystem = singleton<Pg::Graphics::GraphicsApiExporter>();
		_graphicsApiExporter = &tSystem;
	}

	void PgGraphics::SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index)
	{

	}

	void PgGraphics::SetRenderedCubemapIndex(unsigned int index)
	{

	}

	void PgGraphics::RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		_graphicsApiExporter->RegisterEffectObject(effectName, vfxObj);
	}

	void PgGraphics::RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		_graphicsApiExporter->RemoveEffectObject(vfxObj);
	}

	void* PgGraphics::GetRenderMaterialWithName(const std::string& materialName)
	{
		return _graphicsApiExporter->GetRenderMaterialWithName(materialName);
	}

	void PgGraphics::Material_SetBoolVS(void* renderMat, const std::string& varName, bool value)
	{
		_graphicsApiExporter->Material_SetBoolVS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetBoolPS(void* renderMat, const std::string& varName, bool value)
	{
		_graphicsApiExporter->Material_SetBoolPS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetUnsignedIntVS(void* renderMat, const std::string& varName, unsigned int value)
	{
		_graphicsApiExporter->Material_SetUnsignedIntVS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetUnsignedIntPS(void* renderMat, const std::string& varName, unsigned int value)
	{
		_graphicsApiExporter->Material_SetUnsignedIntPS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetIntVS(void* renderMat, const std::string& varName, int value)
	{
		_graphicsApiExporter->Material_SetIntVS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetIntPS(void* renderMat, const std::string& varName, int value)
	{
		_graphicsApiExporter->Material_SetIntPS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloatVS(void* renderMat, const std::string& varName, float value)
	{
		_graphicsApiExporter->Material_SetFloatVS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloatPS(void* renderMat, const std::string& varName, float value)
	{
		_graphicsApiExporter->Material_SetFloatPS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat2VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value)
	{
		_graphicsApiExporter->Material_SetFloat2VS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat2PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value)
	{
		_graphicsApiExporter->Material_SetFloat2PS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat3VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value)
	{
		_graphicsApiExporter->Material_SetFloat3VS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat3PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value)
	{
		_graphicsApiExporter->Material_SetFloat3PS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat4VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value)
	{
		_graphicsApiExporter->Material_SetFloat4VS(renderMat, varName, value);
	}

	void PgGraphics::Material_SetFloat4PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value)
	{
		_graphicsApiExporter->Material_SetFloat4PS(renderMat, varName, value);
	}

	void PgGraphics::ScreenSpace_FadeIn()
	{
		_graphicsApiExporter->ScreenSpace_FadeIn();
	}

	void PgGraphics::ScreenSpace_FadeOut()
	{
		_graphicsApiExporter->ScreenSpace_FadeOut();
	}

	void PgGraphics::RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos)
	{
		_graphicsApiExporter->RegisterPlayerPosition(playerPos);
	}

	unsigned int* PgGraphics::GetEffectTextureIndexPointer(const std::string& effectName)
	{
		return _graphicsApiExporter->GetEffectTextureIndexPointer(effectName);
	}

	bool PgGraphics::GetIsFadingIn()
	{
		return _graphicsApiExporter->GetIsFadingIn();
	}

	bool PgGraphics::GetIsFadingOut()
	{
		return _graphicsApiExporter->GetIsFadingOut();
	}

}