#pragma once
#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// 클라이언트에게 노출되지 않는,
/// 렌더링 파이프라인 자체 소속 셰이더를 관리하기 위해 구성.
/// </summary>

namespace Pg::Graphics
{
	class LowDX11Storage;

	class SystemShader
	{
	public:
		SystemShader(const std::wstring& wFilePath);
		virtual ~SystemShader();

	protected:
		LowDX11Storage* _DXStorage;

		// 쉐이더 바이트코드를 멤버에 저장
		ID3DBlob* _byteCode;

		//디버그용, 따로 FilePath 저장.
		std::string _filePath;
	};
}


