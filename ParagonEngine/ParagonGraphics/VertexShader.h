#pragma once

#include "RenderShader.h"

namespace Pg::Graphics
{
	class VertexShader : public RenderShader
	{
	public:
		VertexShader(std::wstring CSOFilePath, D3D11_INPUT_ELEMENT_DESC* vertexDesc);
		VertexShader(std::wstring CSOFilePath);

	private:
		void CreateShader();

	public:
		void Bind();
		void UnBind();

	private:
		ID3D11VertexShader* _shader;

	public:
		D3D11_INPUT_ELEMENT_DESC* _vertexDesc;
		ID3D11InputLayout* _inputLayout;

	};
}


