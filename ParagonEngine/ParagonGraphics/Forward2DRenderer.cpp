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
		_dxtkCommonStates = std::make_unique<DirectX::CommonStates>(_DXStorage->_device);
	}

	void Forward2DRenderer::SetupRenderPasses()
	{

	}

	void Forward2DRenderer::RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData)
	{
		Render((RenderObject2DList*)renderObjectList, camData);
	}

	void Forward2DRenderer::Render(RenderObject2DList* render2DList, Pg::Data::CameraData* camData)
	{
		//Sprite
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_quadMainRT->GetRTV()), _carrier->_quadMainGDS->GetDSV());
		//PostProcessing이 끝난 후.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_carrier->_PPSwitch2->GetRTV()), _carrier->_quadMainGDS->GetDSV());

		_spriteBatch->Begin(DirectX::SpriteSortMode_BackToFront, _dxtkCommonStates->NonPremultiplied());

		for (auto& it : render2DList->_list)
		{
			if (it.second->GetBaseRenderer()->GetActive())
			{
				//렌더.
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