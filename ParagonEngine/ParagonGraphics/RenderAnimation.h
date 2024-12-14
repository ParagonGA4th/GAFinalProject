#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonData/ParagonDefines.h"
#include "AssetAnimationDataDefine.h"
#include "AssetModelDataDefine.h"
#include <vector>
#include <memory>

/// <summary>
/// .fbx ���ϵ��, Animation�� 1��1 �����ϴ� GraphicsResource Ŭ�����̴�.
/// </summary>

namespace Pg::Graphics
{
	namespace Loader
	{
		class AssetCombinedLoader;
	}
	class AnimationParser;
	class RenderObjectSkinnedMesh3D;
}

namespace Pg::Graphics
{
	class RenderAnimation : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetCombinedLoader;
		friend class Pg::Graphics::AnimationParser;
		friend class RenderObjectSkinnedMesh3D;

	public:
		RenderAnimation(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		virtual ~RenderAnimation();

	public:
		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	private:
		std::unique_ptr<Animation_AssetData> _animAssetData;
		
		
	};
}



