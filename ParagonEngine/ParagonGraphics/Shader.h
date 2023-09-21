#pragma once

#include "DX11Headers.h"

#include "ConstantBuffer.h"

#include <string>
#include <vector>

class ConstantBuffer;
class ConstantBufferBase;

namespace Pg::Graphics
{
	class LowDX11Storage;

	class Shader
	{
	public:
		Shader(LowDX11Storage* storage, std::wstring CSOFilePath);

	protected:
		LowDX11Storage* _DXStorage;

	public:

		// 쉐이더를 컴파일하여 바이트코드를 멤버에 저장
		HRESULT CompileShader(std::wstring CSOFilePath);
		ID3DBlob* _byteCode;

	public:
		// 상수 버퍼들을 저장하는 벡터
		std::vector< ConstantBufferBase* > _constantBuffers;
	
		// 상수 버퍼 데이터를 추가하는 함수
		template <typename T>
		void AddConstantBuffer(T* cbData)
		{
			_constantBuffers.emplace_back(new ConstantBuffer<T>(_DXStorage, cbData));
		}

	};
}