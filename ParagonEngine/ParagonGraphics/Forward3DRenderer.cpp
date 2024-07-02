#include "Forward3DRenderer.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "GraphicsResourceManager.h"
#include "RenderMaterial.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"
#include "SceneInformationList.h"

//RenderPasses
#include "IRenderSinglePass.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include <cassert>
#include <algorithm>

namespace Pg::Graphics
{
	Forward3DRenderer::Forward3DRenderer(D3DCarrier* d3dCarrier, const Pg::Data::Enums::eEditorMode* const editorMode, const std::string& resourceListPath)
		: BaseSpecificRenderer(d3dCarrier), _editorMode(editorMode), _deltaTimeStorage(0.f), _DXStorage(LowDX11Storage::GetInstance()), _resourceListPath(resourceListPath)
	{
		

	}

	void Forward3DRenderer::Initialize()
	{
		
	}

	void Forward3DRenderer::SetupRenderPasses()
	{
		_alphaBlendedTotalPass = std::make_unique<AlphaBlendedTotalRenderPass>(_editorMode);
		_alphaBlendedTotalPass->Initialize();

		_billboardRenderPass = std::make_unique<VisualEffectRenderPass>(_resourceListPath);
		_billboardRenderPass->Initialize();
	}

	void Forward3DRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		//얘는 ObjMat을 기록하지 않는다.
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;
		
		RenderAlphaBlended3DPass(tRenderObjectList, camData);
		RenderBillboardPass(tRenderObjectList, camData);
	}

	void Forward3DRenderer::ConfirmCarrierData()
	{

	}

	void Forward3DRenderer::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void Forward3DRenderer::RenderAlphaBlended3DPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//AlphaBlendedTuple을 기반으로 작동으로 동작한다.
		_alphaBlendedTotalPass->ReceiveRequiredElements(*_carrier);
		_alphaBlendedTotalPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning 활용 패스에 DeltaTime 내부적으로 전달.
		_alphaBlendedTotalPass->BindPass();
		_alphaBlendedTotalPass->RenderPass(renderObjectList, camData);
		_alphaBlendedTotalPass->UnbindPass();
		_alphaBlendedTotalPass->ExecuteNextRenderRequirements();
		_alphaBlendedTotalPass->PassNextRequirements(*_carrier);
	}

	void Forward3DRenderer::RenderBillboardPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{
		//AlphaBlendedTuple을 기반으로 작동으로 동작한다.
		_billboardRenderPass->ReceiveRequiredElements(*_carrier);
		_billboardRenderPass->BindPass();
		_billboardRenderPass->RenderPass(renderObjectList, camData);
		_billboardRenderPass->UnbindPass();
		_billboardRenderPass->ExecuteNextRenderRequirements();
		_billboardRenderPass->PassNextRequirements(*_carrier);
	}

	Pg::Graphics::VisualEffectRenderer* Forward3DRenderer::GetVisualEffectRenderer()
	{
		return _billboardRenderPass->GetVisualEffectRenderer();
	}

}