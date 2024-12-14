#pragma once
#include <vector>
#include <memory>
#include <d3d11.h>

#include "BaseSpecificRenderer.h"
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
	class CubemapRenderer : public BaseSpecificRenderer
	{
	public:
		CubemapRenderer(D3DCarrier* d3dCarrier);

	public:
		virtual void Initialize() override;
		virtual void SetupRenderPasses() override;
		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) override;
		virtual void ConfirmCarrierData() override;

	private:
		void Render(RenderObjectCubemapList* cubeMapList, Pg::Data::CameraData* camData);
		void RenderCubemapWithIndex(Pg::Data::CameraData* camData, RenderObjectCubemapList* cubeMapList, unsigned int cubemapIndex);
	
	private:
		void CreateSystemVertexShaders();

	private:
		LowDX11Storage* _DXStorage;
		LowDX11Logic* _DXLogic;

		unsigned int _internalCubemapList = 0;

		//SystemPixelShader : Cubemap
		std::unique_ptr<SystemVertexShader> _cubemapVS;
		std::unique_ptr<SystemPixelShader> _cubemapPS;
	};
}

