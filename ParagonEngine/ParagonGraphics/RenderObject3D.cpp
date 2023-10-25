#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "LowDX11Storage.h"
#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer)
	{
		//
		
		//#ForwardTemp : GeometricPrimitive 인스턴스 만들기.
		_tempPrimitive = DirectX::GeometricPrimitive::CreateSphere(LowDX11Storage::GetInstance()->_deviceContext, 1.0f);
	}

	RenderObject3D::~RenderObject3D()
	{

	}

	void RenderObject3D::Render()
	{

	}
	


}