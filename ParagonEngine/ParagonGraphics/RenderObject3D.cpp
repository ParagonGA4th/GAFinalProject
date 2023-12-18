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

	RenderObject3D::RenderObject3D(Pg::Data::BaseRenderer* baseRenderer) : RenderObjectBase(baseRenderer),
		_textures()
	{
		_constantBufferStruct = new ConstantBufferDefine::cbPerObjectBase;
		CreateConstantBuffer(_constantBufferStruct);

		_DXStorage = LowDX11Storage::GetInstance();

		//Device / DevCon 받아오기.
		_device = _DXStorage->_device;
		_devCon = _DXStorage->_deviceContext;
	}

	RenderObject3D::~RenderObject3D()
	{
		delete _constantBufferStruct;
	}

	void RenderObject3D::BindShaders()
	{
		_vertexShader->Bind();
		_pixelShader->Bind();
	}

	void RenderObject3D::UnbindShaders()
	{
		_vertexShader->Unbind();
		_pixelShader->Unbind();
	}

	void RenderObject3D::AddTextureToArray(RenderTexture2D* texture)
	{
		_textures.emplace_back(texture);
	}

	void RenderObject3D::BindTextureArray()
	{
		for (int i = 0; i < _textures.size(); ++i)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &(_textures.at(i)->GetSRV()));
		}
	}

	void RenderObject3D::ClearTextureArray()
	{
		_textures.clear();
	}


}