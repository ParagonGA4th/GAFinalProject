#include "RenderObjectStaticMesh3D.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "GraphicsResourceHelper.h"

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;

	RenderObjectStaticMesh3D::RenderObjectStaticMesh3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObject3D(baseRenderer)
	{
		//#ForwardTemp : GeometricPrimitive 인스턴스 만들기.
		_tempPrimitive = DirectX::GeometricPrimitive::CreateSphere(LowDX11Storage::GetInstance()->_deviceContext, 1.0f);
	}

	void RenderObjectStaticMesh3D::Render(Pg::Data::CameraData* camData)
	{
		//렌더.
		DirectX::XMFLOAT4X4 tWorldTM = MathHelper::PG2XM_FLOAT4X4(_baseRenderer->_object->_transform.GetWorldTM());
		DirectX::XMMATRIX tWorldTMMat = DirectX::XMLoadFloat4x4(&tWorldTM);

		DirectX::XMFLOAT4X4 tViewTM = MathHelper::PG2XM_FLOAT4X4(camData->_viewMatrix);
		DirectX::XMMATRIX tViewTMMat = DirectX::XMLoadFloat4x4(&tViewTM);

		DirectX::XMFLOAT4X4 tProjTM = MathHelper::PG2XM_FLOAT4X4(camData->_projMatrix);
		DirectX::XMMATRIX tProjTMMat = DirectX::XMLoadFloat4x4(&tProjTM);

		_tempPrimitive->Draw(tWorldTMMat, tViewTMMat, tProjTMMat, DirectX::Colors::Crimson);
	}
}