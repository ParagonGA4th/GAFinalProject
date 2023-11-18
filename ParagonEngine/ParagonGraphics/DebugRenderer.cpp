#include "DebugRenderer.h"

#include "DX11Headers.h"
#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"

#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	DebugRenderer::DebugRenderer() : _DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{

	}

	void DebugRenderer::Initialize()
	{

	}

	void DebugRenderer::BeginRender()
	{
		//_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);

	}

	void DebugRenderer::Render(Pg::Data::CameraData* camData)
	{

	}

	void DebugRenderer::CreateGeometry()
	{
		//Box & Sphere 만들기.
		_boxShape = DirectX::GeometricPrimitive::CreateBox(_DXStorage->_deviceContext, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		_sphereShape = DirectX::GeometricPrimitive::CreateSphere(_DXStorage->_deviceContext);
		
		//Capsule 만들기.
		
	
	}
}