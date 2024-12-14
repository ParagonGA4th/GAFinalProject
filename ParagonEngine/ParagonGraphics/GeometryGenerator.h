///[TW] 디버그를 위한 수정을 거침.

//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
//***************************************************************************************

#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include "DirectXPackedVector.h"
#include "MathHelper.h"
#include <d3d11.h>

typedef unsigned int UINT;

namespace Pg::Graphics
{
	using namespace DirectX;

	class GeometryGenerator
	{
	public:
		static void Initialize();

		struct GeomVertex_PosColor
		{
			GeomVertex_PosColor() {}
			GeomVertex_PosColor(XMFLOAT3 pos, XMFLOAT4 col)
				: _position(pos), _color(col) {}
			XMFLOAT3 _position;
			XMFLOAT4 _color;
		};

		struct GeomVertex_PosNormalTex
		{
			GeomVertex_PosNormalTex() {}
			GeomVertex_PosNormalTex(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT2 tex )
				: _position(pos), _normal(norm), _tex(tex) {}
			XMFLOAT3 _position;
			XMFLOAT3 _normal;
			XMFLOAT2 _tex;
		};

		struct MeshData_PosColor
		{
			std::vector<GeomVertex_PosColor> Vertices;
			std::vector<UINT> Indices;
		};

		//ScreenQuad를 위해 개조.
		struct MeshData_PosNormalTex
		{
			std::vector<GeomVertex_PosNormalTex> Vertices;
			std::vector<UINT> Indices;
		};

		///<summary>
		/// Creates a box centered at the origin with the given dimensions.
		///</summary>
		static void CreateBox(float width, float height, float depth, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a sphere centered at the origin with the given radius.  The
		/// slices and stacks parameters control the degree of tessellation.
		///</summary>
		static void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
		/// The bottom and top radius can vary to form various cone shapes rather than true
		// cylinders.  The slices and stacks parameters control the degree of tessellation.
		///</summary>
		static void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);

		///<summary>
		/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
		/// at the origin with the specified width and depth.
		///</summary>
		static void CreateGrid(float width, float depth, UINT m, UINT n, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a quad covering the screen in NDC coordinates.  This is useful for
		/// postprocessing effects.
		///</summary>
		static void GenerateFullscreenQuad(MeshData_PosNormalTex& a_meshData); //[TW] Deferred Rendering 위해 개조.

		static void GenerateFullscreenQuad_LyingDown(MeshData_PosNormalTex& a_meshData);

	public:
		static unsigned int QUAD_VERTEX_COUNT;
		static unsigned int QUAD_INDICE_COUNT;

		static ID3D11Buffer* _QUAD_VB;
		static ID3D11Buffer* _QUAD_IB;

		static ID3D11Buffer* LYING_DOWN_QUAD_VB;
		static ID3D11Buffer* LYING_DOWN_QUAD_IB;

	private:
		static void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);
		static void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);
		
	};
}


#endif // GEOMETRYGENERATOR_H