#pragma once

#include "RenderableObject.h"

namespace Pg::Graphics
{
	struct BoxCBData
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMFLOAT4X4 viewProjMatrix;
		DirectX::XMFLOAT3 eyePos;
	};
}

namespace Pg::Data
{
	class Transform;
	class CameraData;
}

namespace Pg::Graphics
{
	class TestCube : public RenderableObject
	{
	public:
		TestCube();

	public:
		virtual void Draw() override;
		virtual void Draw(Pg::Data::Transform& transform, Pg::Data::CameraData& camData) override;

		virtual void BuildBuffers() override;
		virtual void BindBuffers() override;

	private:

		struct TestCubeVertex
		{
			float3 position;
			float3 color;
		};

	public:
		BoxCBData _cbData;
	};
}
