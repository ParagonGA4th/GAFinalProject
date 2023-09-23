#pragma once

#include "Shader.h"

namespace Pg::Graphics
{
	class PixelShader : public Shader
	{
	public:
		PixelShader(LowDX11Storage* storage, std::wstring CSOFilePath);

	private:
		void CreateShader();

	public:
		void Bind();
		void UnBind();

	private:
		ID3D11PixelShader* _shader;
	};
}


