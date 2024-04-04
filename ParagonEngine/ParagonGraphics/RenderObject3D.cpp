#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonHelper/ResourceHelper.h"
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

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer, unsigned int objID) : RenderObjectBase(baseRenderer), _objectID(objID), _materialIdPointer(nullptr), _isObjMatBufferUsable(false)
	{
		_DXStorage = LowDX11Storage::GetInstance();

		//하단에서 별도로 VB/IB (Static, Skinned Mesh 따로 만들어줘야 함)
	}

	RenderObject3D::~RenderObject3D()
	{


	}

	unsigned int RenderObject3D::GetObjectID() const
	{
		return _objectID;
	}

	unsigned int RenderObject3D::GetMaterialID() const
	{
		return *_materialIdPointer;
	}

	void RenderObject3D::SetMaterialIdPointer(unsigned int* matID)
	{
		this->_materialIdPointer = matID;
	}

}