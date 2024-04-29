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
	Forward3DRenderer::Forward3DRenderer(D3DCarrier* d3dCarrier, const Pg::Data::Enums::eEditorMode* const editorMode)
		: BaseSpecificRenderer(d3dCarrier), _editorMode(editorMode), _deltaTimeStorage(0.f), _DXStorage(LowDX11Storage::GetInstance())
	{
		

	}

	void Forward3DRenderer::Initialize()
	{
		
	}

	void Forward3DRenderer::SetupRenderPasses()
	{
		_alphaBlendedTotalPass = std::make_unique<AlphaBlendedTotalRenderPass>(_editorMode);
		_alphaBlendedTotalPass->Initialize();
	}

	void Forward3DRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		//얘는 ObjMat을 기록하지 않는다.
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;
		
		//AlphaBlendedTuple을 기반으로 작동으로 동작한다.
		_alphaBlendedTotalPass->ReceiveRequiredElements(*_carrier);
		_alphaBlendedTotalPass->SetDeltaTime(_deltaTimeStorage); 	//Skinning 활용 패스에 DeltaTime 내부적으로 전달.
		_alphaBlendedTotalPass->BindPass();
		_alphaBlendedTotalPass->RenderPass(renderObjectList, camData);
		_alphaBlendedTotalPass->UnbindPass();
		_alphaBlendedTotalPass->ExecuteNextRenderRequirements();
		_alphaBlendedTotalPass->PassNextRequirements(*_carrier);
	}

	void Forward3DRenderer::ConfirmCarrierData()
	{

	}

	void Forward3DRenderer::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	

}