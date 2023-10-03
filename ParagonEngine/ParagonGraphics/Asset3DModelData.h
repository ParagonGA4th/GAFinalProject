#pragma once
#include "../ParagonCore/GraphicsResource.h"

/// <summary>
/// 실제 3DModel과 1대1 대응되는 클래스. 
/// </summary>

namespace Pg::Graphics
{
	class AssetSceneData;
}

namespace Pg::Graphics
{
	class Asset3DModelData : public Pg::Core::Resources::GraphicsResource
	{
	public:
		Asset3DModelData(Pg::Core::Enums::eAssetDefine define, const std::string& filePath);
		~Asset3DModelData();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	private:
		bool _isSkinned = false;
		AssetSceneData* _assetSceneData = nullptr;

		
	};

}




