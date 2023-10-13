#pragma once

#include "DX11Headers.h"
#include "RenderableObject.h"
#include "GeometryGenerator.h"

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

		void SetGridSize(float width, float depth, UINT m, UINT n);

	public:
		CBDataBase _cbData;

	private:
		GeometryGenerator tGeometryGenerator;
		GeometryGenerator::MeshData_PosColor _MeshData;
	};
}