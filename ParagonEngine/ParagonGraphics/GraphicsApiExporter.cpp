#include "GraphicsApiExporter.h"
#include "GraphicsMain.h"
#include "VisualEffectRenderer.h"
#include "GraphicsResourceManager.h"
#include "ParagonRenderer.h"
#include "RenderMaterial.h"
#include "PPFinalRenderer.h"
#include "Forward3DRenderer.h"

#include "../ParagonUtil/Log.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/CameraData.h"

#include <cassert>

namespace Pg::Graphics
{
	GraphicsApiExporter::GraphicsApiExporter()
	{

	}

	GraphicsApiExporter::~GraphicsApiExporter()
	{

	}

	void GraphicsApiExporter::Initialize(GraphicsMain* graphicsMain)
	{
		_graphicsMain = graphicsMain;
	}

	void SetLoadCubemapIndex(const std::string& cubemapName, unsigned int index)
	{
		PG_WARN("아직 구현되지 않음");
	}

	void GraphicsApiExporter::SetRenderedCubemapIndex(unsigned int index)
	{
		PG_WARN("아직 구현되지 않음");
	}

	void GraphicsApiExporter::RegisterEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		VisualEffectRenderer* tVERenderer = _graphicsMain->GetVisualEffectRenderer();
		assert(tVERenderer != nullptr);
		tVERenderer->RegisterVisualEffectObject(effectName, vfxObj);
	}

	void GraphicsApiExporter::RemoveEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		VisualEffectRenderer* tVERenderer = _graphicsMain->GetVisualEffectRenderer();
		assert(tVERenderer != nullptr);
		tVERenderer->RemoveVisualEffectObject(vfxObj);
	}

	void* GraphicsApiExporter::GetRenderMaterialWithName(const std::string& materialName)
	{
		auto tPtr = Manager::GraphicsResourceManager::Instance()->GetResourceByName(materialName, Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL).get();
		return (void*)tPtr;
	}

	void GraphicsApiExporter::Material_SetBoolVS(void* renderMat, const std::string& varName, bool value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetBoolVS(varName, value);
	}

	void GraphicsApiExporter::Material_SetBoolPS(void* renderMat, const std::string& varName, bool value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetBoolPS(varName, value);
	}

	void GraphicsApiExporter::Material_SetUnsignedIntVS(void* renderMat, const std::string& varName, unsigned int value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetUnsignedIntVS(varName, value);
	}

	void GraphicsApiExporter::Material_SetUnsignedIntPS(void* renderMat, const std::string& varName, unsigned int value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetUnsignedIntPS(varName, value);
	}

	void GraphicsApiExporter::Material_SetIntVS(void* renderMat, const std::string& varName, int value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetIntVS(varName, value);
	}

	void GraphicsApiExporter::Material_SetIntPS(void* renderMat, const std::string& varName, int value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetIntPS(varName, value);
	}

	void GraphicsApiExporter::Material_SetFloatVS(void* renderMat, const std::string& varName, float value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetFloatVS(varName, value);
	}

	void GraphicsApiExporter::Material_SetFloatPS(void* renderMat, const std::string& varName, float value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		tRenderMat->SetFloatPS(varName, value);
	}

	void GraphicsApiExporter::Material_SetFloat2VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT2 tValue = Pg::Math::PG2XM_FLOAT2(value);
		tRenderMat->SetFloat2VS(varName, tValue);
	}

	void GraphicsApiExporter::Material_SetFloat2PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT2 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT2 tValue = Pg::Math::PG2XM_FLOAT2(value);
		tRenderMat->SetFloat2PS(varName, tValue);
	}

	void GraphicsApiExporter::Material_SetFloat3VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT3 tValue = Pg::Math::PG2XM_FLOAT3(value);
		tRenderMat->SetFloat3VS(varName, tValue);
	}

	void GraphicsApiExporter::Material_SetFloat3PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT3 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT3 tValue = Pg::Math::PG2XM_FLOAT3(value);
		tRenderMat->SetFloat3PS(varName, tValue);
	}

	void GraphicsApiExporter::Material_SetFloat4VS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT4 tValue = Pg::Math::PG2XM_FLOAT4(value);
		tRenderMat->SetFloat4VS(varName, tValue);
	}

	void GraphicsApiExporter::Material_SetFloat4PS(void* renderMat, const std::string& varName, Pg::Math::PGFLOAT4 value)
	{
		assert(renderMat != nullptr);
		RenderMaterial* tRenderMat = (RenderMaterial*)renderMat;
		DirectX::XMFLOAT4 tValue = Pg::Math::PG2XM_FLOAT4(value);
		tRenderMat->SetFloat4PS(varName, tValue);
	}

	void GraphicsApiExporter::ScreenSpace_FadeIn()
	{
		_graphicsMain->_renderer->_finalRenderer->Effect_FadeIn();
	}

	void GraphicsApiExporter::ScreenSpace_FadeOut()
	{
		_graphicsMain->_renderer->_finalRenderer->Effect_FadeOut();
	}

	void GraphicsApiExporter::RegisterPlayerPosition(Pg::Math::PGFLOAT3 playerPos)
	{
		Pg::Data::CameraData* tCamData = _graphicsMain->GetCamData();
		//딱 값을 정의해서 쓸 수 있게 만들기.
		tCamData->_playerPosition = playerPos;
	}

	unsigned int* GraphicsApiExporter::GetEffectTextureIndexPointer(const std::string& effectName)
	{
		return _graphicsMain->_renderer->_forward3dRenderer->GetVisualEffectRenderer()->GetEffectTextureIndexPointer(effectName);
	}

	bool GraphicsApiExporter::GetIsFadingIn()
	{
		return _graphicsMain->_renderer->_finalRenderer->GetIsFadingIn();
	}

	bool GraphicsApiExporter::GetIsFadingOut()
	{
		return _graphicsMain->_renderer->_finalRenderer->GetIsFadingOut();
	}

}
//unsigned int* GraphicsApiExporter::GetEffectTextureIndexPointer(const std::string& effectName)
