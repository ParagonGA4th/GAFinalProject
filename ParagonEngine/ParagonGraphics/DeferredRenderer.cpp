#include "DeferredRenderer.h"
#include "SystemVertexShader.h"
#include "SystemPixelShader.h"

#include "GBufferRender.h"
#include "GBufferDepthStencil.h"
#include "LowDX11Storage.h"

#include "LayoutDefine.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/LightType.h"

#include "RenderObjectBase.h"

#include "RenderObject3D.h"
#include "RenderObject3DList.h"
#include "RenderObjectLightList.h"

#include "ConstantBufferDefine.h"

#include "GeometryGenerator.h"

namespace Pg::Graphics
{
	DeferredRenderer::DeferredRenderer()
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	void DeferredRenderer::Initialize()
	{

	}

	void DeferredRenderer::Render(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData)
	{

	}
}


