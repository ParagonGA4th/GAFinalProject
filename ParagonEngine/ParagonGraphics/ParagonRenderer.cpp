#include "ParagonRenderer.h"

#include "LowDX11Logic.h"

namespace Pg::Graphics
{
	void ParagonRenderer::BeginRender()
	{
		Pg::Graphics::LowDX11Logic::GetInstance()->PrepareRenderTargets();
	}

	void ParagonRenderer::Render(Pg::Core::GameObject* gameObject)
	{
		_DXLogic->BindRenderTargets();
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

}

