#include "RenderObject3D.h"
#include "GraphicsResourceManager.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonUtil/ResourceHelper.h"
#include "LowDX11Storage.h"
#include "MathHelper.h"
#include "ConstantBufferDefine.h"

#include "DX11Headers.h"

#include "dxtk/DDSTextureLoader.h"
#include "dxtk/WICTextureLoader.h"

#include "Asset3DModelData.h"


#include "VertexShader.h"
#include "PixelShader.h"

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

	}

	void RenderObject3D::Render()
	{

	}

	void RenderObject3D::Initialize()
	{
		BindBuffers();
	}

	void RenderObject3D::BindShaders()
	{
		_vertexShader->Bind();
		_pixelShader->Bind();

		// Bind Shader Resources
		//_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &_SRV);

		//
		_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_DXStorage->_defaultSamplerState);
	}

	void RenderObject3D::UnbindShaders()
	{
		_vertexShader->UnBind();
		_pixelShader->UnBind();
	}

	void RenderObject3D::BindInputLayout()
	{
		_DXStorage->_deviceContext->IASetInputLayout(_inputLayout);
		_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void RenderObject3D::UnbindInputLayout()
	{
		_DXStorage->_deviceContext->IASetInputLayout(nullptr);
	}

	void RenderObject3D::SetVertexShader(VertexShader* shader)
	{
		_vertexShader = shader;
	}

	void RenderObject3D::SetPixelShader(PixelShader* shader)
	{
		_pixelShader = shader;
	}

	Pg::Graphics::VertexShader* RenderObject3D::GetVertexShader()
	{
		return _vertexShader;
	}

	Pg::Graphics::PixelShader* RenderObject3D::GetPixelShader()
	{
		return _pixelShader;
	}

	void RenderObject3D::BindTextures()
	{
		for (int i = 0; i < _textures.size(); ++i)
		{
			_DXStorage->_deviceContext->PSSetShaderResources(i, 1, &(_textures.at(i)->GetSRV()));
		}
	}

}