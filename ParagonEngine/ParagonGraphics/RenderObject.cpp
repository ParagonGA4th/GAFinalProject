#include "RenderObject.h"

namespace Pg::Graphics
{
	RenderObject::RenderObject() : _is3D(true)
	{
		//만들 때, 렌더할 때 필요한 정보를 받아와야 한다.

	}

	RenderObject::~RenderObject()
	{

	}

	void RenderObject::UpdateObjectRenderData(Pg::Core::RenderTextData rTextData)
	{
		this->_is3D = rTextData.is3D;

		if (_is3D)
		{
			//3D

		}
		else
		{
			//2D

		}


	}



	

}