#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer)
	{
		//
	}

	RenderObject3D::~RenderObject3D()
	{

	}

	void RenderObject3D::UpdateObjectRenderData(const Pg::Data::RenderTextData rTextData)
	{
		//그 자체로 렌더 가능한 리소스인지 검사.
		int tCanRender = ResourceHelper::IsPlainRenderable(rTextData._assetDefine);
		assert((tCanRender == 1) && "막히면 3D로 렌더될 수 없는 RenderTextData를 전달했다는 뜻.");

		Update3DObjectRenderData(rTextData);

	}

	void RenderObject3D::Update3DObjectRenderData(const Pg::Data::RenderTextData& recent)
	{
		//3D
		RenderUsageStruct3D tFreshInfo3D(recent);

		if (_renderUsageStruct3D != tFreshInfo3D)
		{
			//World Matrix는 그냥 대입.
			_renderUsageStruct3D._worldMatrix3D = tFreshInfo3D._worldMatrix3D;

			//ResourcePath 다르면, 새로 투입.
			if (!_renderUsageStruct3D.IsResourcePathSame(recent.resourcePath))
			{
				//이미 Load한 리소스만 가져올 수 있게 설계!
				auto it = _graphicsResourceManager->GetResource(recent.resourcePath, recent._assetDefine);
				_renderUsageStruct3D._resourcePath3D = recent.resourcePath;
			}
		}
	}



}