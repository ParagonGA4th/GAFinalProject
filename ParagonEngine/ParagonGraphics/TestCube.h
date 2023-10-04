#pragma once

#include "RenderableObject.h"

namespace Pg::Graphics
{
	struct BoxCBData
	{
		float4x4 worldMatrix;
		float4x4 viewMatrix;
		float4x4 projectionMatrix;
		float4x4 viewProjMatrix;
		float3 eyePos;
	};
}

namespace Pg::Graphics
{
	class TestCube : public RenderableObject
	{
	public:
		TestCube();

	public:
		virtual void Draw() override;

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
