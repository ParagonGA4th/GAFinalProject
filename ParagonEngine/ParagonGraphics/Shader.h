#pragma once

#include "DX11Headers.h"

#include "ConstantBuffer.h"

#include <string>
#include <vector>
#include <any>

class BoxCBData;
class ConstantBuffer;
class ConstantBufferBase;

namespace Pg::Graphics
{
	class LowDX11Storage;

	class Shader
	{
	public:
		Shader(LowDX11Storage* storage, std::wstring CSOFilePath);

	public:
		HRESULT CompileShader(std::wstring CSOFilePath);

		template <typename T>
		void AddConstantBuffer(T* cbData)
		{
			_constantBuffers.emplace_back(new ConstantBuffer<T>(_DXStorage, cbData));
		}

	public:
		ID3DBlob* _byteCode;

	protected:
		LowDX11Storage* _DXStorage;


	public:
		// 상수 버퍼 관련
		D3D11_BUFFER_DESC _ConstantBufferDesc;
		std::vector< ConstantBufferBase* > _constantBuffers;
	};

}