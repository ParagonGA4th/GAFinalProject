#include "RenderObject.h"
#include "GraphicsResourceManager.h"
#include "../ParagonUtil/ResourceHelper.h"

#include <cassert>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject::RenderObject() : 
		_is3D(true), _renderUsageStruct2D(), _renderUsageStruct3D(), _graphicsResourceManager(nullptr)
	{
		this->_graphicsResourceManager = GraphicsResourceManager::Instance();
		//만들 때, 렌더할 때 필요한 정보를 받아와야 한다.
	}

	RenderObject::~RenderObject()
	{

	}

	void RenderObject::UpdateObjectRenderData(const Pg::Core::RenderTextData rTextData)
	{
		//그 자체로 렌더 가능한 리소스인지 검사.
		int tCanRender = ResourceHelper::IsPlainRenderable(rTextData._assetDefine);
		assert((tCanRender != -1) && "막히면 렌더될 수 없는 RenderTextData를 전달했다는 뜻.");

		//함수 자체 로직으로 2D / 3D 구분.
		bool tNewIs3D = (bool)tCanRender;

		//렌더러의 종류가 바뀌었는지 확인.
		if (this->_is3D != tNewIs3D)
		{
			_renderUsageStruct2D.Clear();
			_renderUsageStruct3D.Clear();
		}

		//3D 렌더러인지 여부 업데이트.
		this->_is3D = tNewIs3D;

		if (_is3D) 
		{
			Update3DObjectRenderData(rTextData);
		}
		else
		{
			Update2DObjectRenderData(rTextData);
		}






	}

	void RenderObject::Update2DObjectRenderData(const Pg::Core::RenderTextData& recent)
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
				//이미 만들어진 리소스만 갖고 올 수 있게 설계!
				//_graphicsResourceManager->GetResource(recent.resourcePath);
			}
		}
	}

	void RenderObject::Update3DObjectRenderData(const Pg::Core::RenderTextData& recent)
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

			}
		}
	}

	

}