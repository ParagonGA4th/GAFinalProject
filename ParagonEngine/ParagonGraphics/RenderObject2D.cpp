#include "RenderObject2D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject2D::RenderObject2D() :
		_renderUsageStruct2D(), _graphicsResourceManager(nullptr)
	{
		this->_graphicsResourceManager = GraphicsResourceManager::Instance();
		//만들 때, 렌더할 때 필요한 정보를 받아와야 한다.
	}

	RenderObject2D::~RenderObject2D()
	{

	}

	void RenderObject2D::UpdateObjectRenderData(const Pg::Data::RenderTextData rTextData)
	{
		//그 자체로 렌더 가능한 리소스인지 검사.
		int tCanRender = ResourceHelper::IsPlainRenderable(rTextData._assetDefine);
		assert((tCanRender == 0) && "막히면 렌더될 수 없는 RenderTextData를 전달했다는 뜻.");

		Update2DObjectRenderData(rTextData);

	}

	void RenderObject2D::Update2DObjectRenderData(const Pg::Data::RenderTextData& recent)
	{
		//2D
		RenderUsageStruct2D tFreshInfo2D(recent);

		if (_renderUsageStruct2D != tFreshInfo2D)
		{
			//World Matrix는 그냥 대입.
			_renderUsageStruct2D._worldMatrix2D = tFreshInfo2D._worldMatrix2D;

			//ResourcePath 다르면, 새로 투입.
			if (!_renderUsageStruct2D.IsResourcePathSame(recent.resourcePath))
			{
				//이미 Load한 리소스만 가져올 수 있게 설계!
				auto it = _graphicsResourceManager->GetResource(recent.resourcePath, recent._assetDefine);
				_renderUsageStruct2D._resourcePath2D = recent.resourcePath;
			}
		}
	}





}