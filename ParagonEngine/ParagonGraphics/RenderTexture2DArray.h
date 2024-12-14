#pragma once
#include "RenderTexture.h"
#include <vector>
/// <summary>
/// ���� Texture2DArray�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ����� Texture2DArray�� �����Ǿ� �����ȴ�.
/// </summary>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Pg::Graphics
{
	class RenderTexture2D;
	namespace Loader { AssetBasic2DLoader; }

	class RenderTexture2DArray : public RenderTexture
	{
		friend class Pg::Graphics::Loader::AssetBasic2DLoader;
	public:
		RenderTexture2DArray(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderTexture2DArray();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		std::vector<RenderTexture2D*> GetSingleRenderTexture2DArray() { return _singleRenderTexture2DArray; }
	private:
		//���������� �ε�� ����.
		std::vector<RenderTexture2D*> _singleRenderTexture2DArray;
	};
}
