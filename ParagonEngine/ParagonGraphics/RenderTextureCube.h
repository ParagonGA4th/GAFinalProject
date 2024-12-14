#pragma once
#include "RenderTexture.h"

/// <summary>
/// ���� TextureCube�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ����� TextureCube�� �����Ǿ� �����ȴ�.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTextureCube : public RenderTexture
	{
	public:
		RenderTextureCube(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTextureCube();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;
	};
}

