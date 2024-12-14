#pragma once
#include "DX11Headers.h"

#include <memory>
#include <string>
#include <vector>

/// <summary>
/// Ŭ���̾�Ʈ���� ������� �ʴ�,
/// ������ ���������� ��ü �Ҽ� ���̴��� �����ϱ� ���� ����.
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

		// ���̴� ����Ʈ�ڵ带 ����� ����
		ID3DBlob* _byteCode;

		//����׿�, ���� FilePath ����.
		std::string _filePath;
	};
}


