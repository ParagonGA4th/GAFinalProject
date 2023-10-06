// #LINKERRORWATCH 링크 에러 조심! Data에서 피치 못하게 Graphics를 가져온다.

#include "RendererBase3D.h"
#include "GameObject.h"
//#include "../ParagonGraphics/RenderObject3D.h"

namespace Pg::Data
{
	RendererBase3D::RendererBase3D(GameObject* obj) : BaseRenderer(obj)
	{
		//_renderObject = new Pg::Graphics::RenderObject3D();
		//_is3D = true;
	}

	RendererBase3D::~RendererBase3D()
	{
		//
	}

}
