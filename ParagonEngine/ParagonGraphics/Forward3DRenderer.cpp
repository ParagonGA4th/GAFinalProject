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
#include "AlphaBlendedRenderPass.h"

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
		//_alphaBlendedRenderPass = std::make_unique<AlphaBlendedRenderPass>(_editorMode);
		//_alphaBlendedRenderPass->Initialize();
	}

	void Forward3DRenderer::SetupRenderPasses()
	{

	}

	void Forward3DRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		RenderObject3DList* tRenderObjectList = (RenderObject3DList*)renderObjectList;

		RenderAlphaBlendedTotalPass(tRenderObjectList, camData);
	}

	void Forward3DRenderer::ConfirmCarrierData()
	{

	}

	void Forward3DRenderer::SetDeltaTime(float dt)
	{
		_deltaTimeStorage = dt;
	}

	void Forward3DRenderer::RenderAlphaBlendedTotalPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{

	}

}