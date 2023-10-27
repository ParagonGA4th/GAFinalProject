#include "Forward2DRenderer.h"
#include "RenderObject2DList.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	
	void Forward2DRenderer::Initialize()
	{
		_DXStorage = Pg::Graphics::LowDX11Storage::GetInstance();
		_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);

		
	}

	void Forward2DRenderer::Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData)
	{
		_spriteBatch->Begin();

		for (auto& it : render2DList->_list)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				//·»´õ.
				it.second->Render(_spriteBatch.get(), camData);
			}
		}

		_spriteBatch->End();
	}

	void Forward2DRenderer::Finalize()
	{

	}
}