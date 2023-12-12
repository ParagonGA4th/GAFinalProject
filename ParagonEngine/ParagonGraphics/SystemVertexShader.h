#pragma once
#include "SystemShader.h"

/// <summary>
/// SystemShader 중, Vertex Shader를 별도로 관리.
/// </summary>

namespace Pg::Graphics
{
	class SystemVertexShader : public SystemShader
	{
	public:
		SystemVertexShader(const std::wstring& wFilePath, ID3D11InputLayout* tInputLayout);
		virtual ~SystemVertexShader();

		ID3D11VertexShader* GetVertexShader();
		void Bind();
		void Unbind();
	private:
		ID3D11VertexShader* _shader;
		ID3D11InputLayout* _inputLayout;
	};
}


