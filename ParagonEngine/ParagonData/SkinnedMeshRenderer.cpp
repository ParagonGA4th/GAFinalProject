#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"
#include <cassert>

namespace Pg::Data
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void SkinnedMeshRenderer::ConvertPotentialUnrealValues()
	{
		//
	}

	void SkinnedMeshRenderer::CheckForPathNameErrors()
	{

	}

	void SkinnedMeshRenderer::SetAnimation(const std::string& animName)
	{
		//단순한 존재여부를 파악
		if (!_setAnimationFunction)
		{
			std::string tPrint = "SetAnimation Impl not yet bound in : ";
			tPrint.append(animName);
			PG_WARN(tPrint.c_str());
		}
		//제대로 된 실행 여부를 확인.
		_setAnimationFunction(animName);
	}

}