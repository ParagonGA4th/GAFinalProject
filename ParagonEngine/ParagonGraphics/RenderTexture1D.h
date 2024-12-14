#pragma once
#include "RenderTexture.h"

/// <summary>
/// ���� Texture1D�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ����� Texture1D�� �����Ǿ� �����ȴ�.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTexture1D : public RenderTexture
	{
	public:
		RenderTexture1D(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture1D();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;
	};
}



