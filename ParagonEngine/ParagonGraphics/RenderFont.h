#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/AssetDefines.h"

#include <memory>
#include <string>
#include <dxtk/SpriteFont.h>

/// <summary>
/// ���� Font�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ����� Font�� �����Ǿ� �����ȴ�.
/// -> Text Rendering�� ���ȴ�.
/// </summary>

namespace Pg::Graphics
{
	class RenderFont : public Pg::Data::Resources::GraphicsResource
	{
	public:
		RenderFont(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderFont();

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		std::unique_ptr<DirectX::SpriteFont> _font;
	};
}


