#pragma once

#include "DX11Headers.h"
#include "RenderableObject.h"

namespace Pg::Graphics
{

	class Grid : public RenderableObject
	{
	public:
		Grid();
		virtual ~Grid();

	public:
		virtual void Draw() override;

		virtual void BuildBuffers() override;
		virtual void BindBuffers() override;

		virtual void BindInputLayout() override;

	public:
		CBDataBase _cbData;
	};
}