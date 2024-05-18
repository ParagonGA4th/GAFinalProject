#include "RenderObjectInstancedMesh3D.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "LayoutDefine.h"
#include "MathHelper.h"
#include "RenderTexture2D.h"
#include "AssetBasic3DLoader.h"
#include "AssetModelDataDefine.h"
#include "AssetTextureType.h"
#include "MaterialCluster.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/ParagonDefines.h"

#include <dxtk/WICTextureLoader.h>
#include <dxtk/SimpleMath.h>
#include <singleton-cpp/singleton.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Data::Enums::eAssetDefine;

	RenderObjectInstancedMesh3D::RenderObjectInstancedMesh3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID) :
		RenderObject3D(baseRenderer, objID)
	{

	}

	RenderObjectInstancedMesh3D::~RenderObjectInstancedMesh3D()
	{

	}

	void RenderObjectInstancedMesh3D::First_UpdateConstantBuffers(Pg::Data::CameraData* camData)
	{

	}

	void RenderObjectInstancedMesh3D::First_BindBuffers()
	{

	}

	void RenderObjectInstancedMesh3D::First_Render(const float* const dt)
	{

	}

	void RenderObjectInstancedMesh3D::First_UnbindBuffers()
	{

	}
}
