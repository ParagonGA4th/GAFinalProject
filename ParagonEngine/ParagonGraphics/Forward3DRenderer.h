#pragma once
#include <vector>
#include <memory>
#include <d3d11.h>

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
		void RenderCubemapWithIndex(Pg::Data::CameraData* camData, RenderObjectCubemapList* cubeMapList, unsigned int cubemapIndex);
	
	private:
		void CreateSystemVertexShaders();
	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

		//SystemPixelShader : Cubemap
		std::unique_ptr<SystemVertexShader> _cubemapVS;
		std::unique_ptr<SystemPixelShader> _cubemapPS;
	};
}

