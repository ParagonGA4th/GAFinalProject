#pragma once

#include "Shader.h"

namespace Pg::Graphics
{
	class VertexShader : public Shader
	{
	public:
		VertexShader(LowDX11Storage* storage, std::wstring CSOFilePath, D3D11_INPUT_ELEMENT_DESC* vertexDesc);

	private:
		void CreateShader();

	public:
		void Bind();

	private:
		ID3D11VertexShader* _shader;

	public:
		D3D11_INPUT_ELEMENT_DESC* _vertexDesc;
		ID3D11InputLayout* _inputLayout;

	};
}


