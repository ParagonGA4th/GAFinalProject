#pragma once
#include "D3DCarrier.h"
#include "SceneInformationList.h"
#include "../ParagonData/CameraData.h"

namespace Pg::Graphics
{
	class BaseSpecificRenderer
	{
	public:
		BaseSpecificRenderer(D3DCarrier* d3dCarrier);
		virtual void Initialize() abstract;
		virtual void SetupRenderPasses() abstract;
		virtual void RenderContents(void* renderObjectList, void* optionalRequirement, Pg::Data::CameraData* camData) abstract;
		virtual void ConfirmCarrierData() abstract;

	protected:
		D3DCarrier* _carrier = nullptr;
	};
}


