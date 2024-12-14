#pragma once
#include "SystemShader.h"

/// <summary>
/// SystemShader ��, Pixel Shader�� ������ ����.
/// </summary>

namespace Pg::Graphics
{
	class SystemPixelShader : public SystemShader
	{
	public:
		SystemPixelShader(const std::wstring& wFilePath);
		virtual ~SystemPixelShader();

		ID3D11PixelShader* GetPixelShader();
		void Bind();
		void Unbind();
	private:
		ID3D11PixelShader* _shader{ nullptr };
	};
}




