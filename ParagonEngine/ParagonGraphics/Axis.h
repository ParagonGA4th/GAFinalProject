#pragma once

#include "RenderableObject.h"

namespace Pg::Graphics
{

	class Axis : public RenderableObject
	{
	public:
		Axis();
		virtual ~Axis();

	public:
		virtual void Render() override;

		virtual void BuildBuffers() override;
		virtual void BindBuffers() override;

		virtual void BindInputLayout() override;

	public:
		CBDataBase _cbData;
	};
}