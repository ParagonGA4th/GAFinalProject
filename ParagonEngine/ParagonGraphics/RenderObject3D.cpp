#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "ConstantBufferDefine.h"

#include "DX11Headers.h"

#include "Asset3DModelData.h"

#include "RenderVertexShader.h"
#include "RenderPixelShader.h"

#include "RenderTexture2D.h"

#include <cassert>
#include <filesystem>

namespace Pg::Graphics
{
	using Pg::Data::Enums::eAssetDefine;
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Util::Helper::ResourceHelper;

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer)
	{
		_DXStorage = LowDX11Storage::GetInstance();
	}

	RenderObject3D::~RenderObject3D()
	{


	}

	

}