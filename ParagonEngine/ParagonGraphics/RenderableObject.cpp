#include "RenderableObject.h"

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

	for (auto& cb : _vertexShader->_constantBuffers)
	{
		cb->Update();
	}
	
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
