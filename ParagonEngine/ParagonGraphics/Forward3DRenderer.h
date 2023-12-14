#pragma once
#include <vector>
#include <memory>

#include "SystemVertexShader.h"
#include "SystemPixelShader.h"
#include "WireframeRenderObject.h"

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class LowDX11Logic;

	class RenderCubemap;
	class RenderObjectCubemapList;
}

namespace Pg::Graphics
{
	class Forward3DRenderer
	{
	public:
		Forward3DRenderer();

	public:
		void Initialize();
		void Render(RenderObjectCubemapList* cubeMapList, unsigned int cubeMapIndex, Pg::Data::CameraData* camData);

	private:
		void RenderWireframePrimitives(Pg::Data::CameraData* camData);
		void RenderCubemapWithIndex(Pg::Data::CameraData* camData, RenderObjectCubemapList* cubeMapList, unsigned int cubemapIndex);
	private:
		void CreateSystemVertexShaders();
		void InitializePrimitiveWireframeObjects();
	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

		//SystemVertexShader : Primitive
		std::unique_ptr<SystemVertexShader> _primitiveVS;
		std::unique_ptr<SystemPixelShader> _primitivePS; 

		//SystemPixelShader : Cubemap
		std::unique_ptr<SystemVertexShader> _cubemapVS;
		std::unique_ptr<SystemPixelShader> _cubemapPS;
		
		//Axis / Grid 등을 위함.
		std::vector<std::unique_ptr<WireframeRenderObject>> _primObjectList;
	};
}

