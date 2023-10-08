// #LINKERRORWATCH 링크 에러 조심! Data에서 피치 못하게 Graphics를 가져온다.

#include "RendererBase2D.h"
#include "GameObject.h"
#include "IRenderObject.h"

namespace Pg::Data
{
	RendererBase2D::RendererBase2D(GameObject* obj) : BaseRenderer(obj)
	{
		
	}

	RendererBase2D::~RendererBase2D()
	{
		//
	}

	

}