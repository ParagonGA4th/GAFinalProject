#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"

namespace Pg::Graphics
{
	ParagonRenderer::ParagonRenderer() :
		_DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	ParagonRenderer::~ParagonRenderer()
	{

	}

	void ParagonRenderer::BeginRender()
	{
		_DXLogic->PrepareRenderTargets();
		_DXLogic->BindRenderTargets();
	}

	void ParagonRenderer::Render(Pg::Data::GameObject* gameObject)
	{
		
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}
}

