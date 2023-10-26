#include "Forward2DRenderer.h"
#include "RenderObject2DList.h"
#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	void Forward2DRenderer::Initialize()
	{

	}

	void Forward2DRenderer::Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData)
	{
		for (auto& it : render2DList->_list)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				//·»´õ.

			}
		}
	}

	void Forward2DRenderer::Finalize()
	{

	}
}