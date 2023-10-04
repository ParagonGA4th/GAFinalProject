#pragma once

#include "RenderableObject.h"

namespace Pg::Graphics
{
	struct CubeMapCB : CBDataBase
	{
		float4x4 worldViewProjMatrix;
	};

	struct CubemapVertex
	{
		float3 Pos;
		float2 UV;
	};
}

namespace Pg::Graphics
{
	class Cubemap : public RenderableObject
	{
	public:
		virtual void Initialize() override;
		virtual void Draw() override;

	protected:
		virtual void BuildBuffers() override;
		virtual void BindBuffers() override;

	public:
		CubeMapCB _cbData;
	};
}
