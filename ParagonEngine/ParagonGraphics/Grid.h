#pragma once
#include "WireframeRenderObject.h"

namespace Pg::Graphics
{
	class Grid : public WireframeRenderObject
	{
	public:
		Grid();
		virtual ~Grid();

	private:
		virtual void BuildBuffers() override;
		
	};
}