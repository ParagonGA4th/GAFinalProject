#include "Forward2DRenderer.h"
#include "RenderObject2DList.h"
#include "LowDX11Storage.h"

#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	Forward2DRenderer::Forward2DRenderer(D3DCarrier* d3dCarrier) : BaseSpecificRenderer(d3dCarrier), _DXStorage(Pg::Graphics::LowDX11Storage::GetInstance())
	{

	}

	void Forward2DRenderer::Initialize()
	{
		_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);
	}

	void Forward2DRenderer::SetupRenderPasses()
	{

	}

	void Forward2DRenderer::RenderContents(void* renderObjectList, Pg::Data::CameraData* camData)
	{
		Render((RenderObject2DList*)renderObjectList, camData);
	}

	void Forward2DRenderer::Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData)
	{
		//Sprite
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_quadMainGDS->GetDSV());

		_spriteBatch->Begin(DirectX::SpriteSortMode_BackToFront);

		for (auto& it : render2DList->_list)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				//·»´õ.
				it.second->Render(_spriteBatch.get(), camData);
			}
		}

		_spriteBatch->End();

		_DXStorage->_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
	}

	void Forward2DRenderer::ConfirmCarrierData()
	{

	}

	

}