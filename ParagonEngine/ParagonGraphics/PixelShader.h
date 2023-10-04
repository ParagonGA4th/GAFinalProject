#pragma once

#include "RenderShader.h"

namespace Pg::Graphics
{
	class PixelShader : public RenderShader
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


