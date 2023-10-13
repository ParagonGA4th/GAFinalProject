#include "RenderableObject.h"

#include "DX11Headers.h"
#include "dxtk/DDSTextureLoader.h"
#include "LowDX11Storage.h"
#include "VertexShader.h"
#include "PixelShader.h"

Pg::Graphics::RenderableObject::RenderableObject()
{
	_DXStorage = Pg::Graphics::LowDX11Storage::GetInstance();

}

Pg::Graphics::RenderableObject::~RenderableObject()
{

}

void Pg::Graphics::RenderableObject::Initialize()
{
	BuildBuffers();
	BindBuffers();
}

void Pg::Graphics::RenderableObject::Update(float time)
{

}

void Pg::Graphics::RenderableObject::Draw(Pg::Data::Transform& transform, Pg::Data::CameraData& camData)
{

}

void Pg::Graphics::RenderableObject::Draw()
{

}

void Pg::Graphics::RenderableObject::BuildBuffers()
{

}

void Pg::Graphics::RenderableObject::BindBuffers()
{

}

void Pg::Graphics::RenderableObject::BindShaders()
{
	_vertexShader->Bind();
	_pixelShader->Bind();

	// Bind Constant Buffers
	for (auto& cb : _vertexShader->_constantBuffers)
	{
		cb->UpdateAndBind();
	}
	
	// Bind Shader Resources
	_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &SRV);

	//
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_samplerState);
}

void Pg::Graphics::RenderableObject::UnbindShaders()
{
	_vertexShader->UnBind();
	_pixelShader->UnBind();
}

void Pg::Graphics::RenderableObject::BindInputLayout()
{
	_DXStorage->_deviceContext->IASetInputLayout(_vertexShader->_inputLayout);
	_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Pg::Graphics::RenderableObject::UnbindInputLayout()
{
	_DXStorage->_deviceContext->IASetInputLayout(nullptr);
}

void Pg::Graphics::RenderableObject::AssignVertexShader(VertexShader* shader)
{
	_vertexShader = shader;
}

void Pg::Graphics::RenderableObject::AssignPixelShader(PixelShader* shader)
{
	_pixelShader = shader;
}

Pg::Graphics::VertexShader* Pg::Graphics::RenderableObject::GetVertexShader()
{
	return _vertexShader;
}

Pg::Graphics::PixelShader* Pg::Graphics::RenderableObject::GetPixelShader()
{
	return _pixelShader;
}

void Pg::Graphics::RenderableObject::SetTexture(std::wstring filepath)
{
	ID3D11Resource* Texture = nullptr;

	HRESULT hr = DirectX::CreateDDSTextureFromFile(_DXStorage->_device, filepath.c_str(), &Texture, &SRV);

	_DXStorage->_deviceContext->PSSetShaderResources(0, 1, &SRV);

	D3D11_SAMPLER_DESC sd;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	sd.MipLODBias = 0.0f;
	sd.MaxAnisotropy = 1;
		
	_DXStorage->_device->CreateSamplerState(&sd, &_samplerState);
	_DXStorage->_deviceContext->PSSetSamplers(0, 1, &_samplerState);
}
