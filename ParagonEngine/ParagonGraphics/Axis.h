#pragma once
#include "WireframeRenderObject.h"

namespace Pg::Graphics
{
	class Axis : public WireframeRenderObject
	{
	public:
		Axis();
		virtual ~Axis();

	private:
		virtual void BuildBuffers() override;
	};
}