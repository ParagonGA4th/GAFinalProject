#include "RenderTexture2DArray.h"
#include "AssetBasic2DLoader.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "RenderTexture2D.h"
#include <typeinfo>
#include <filesystem>

namespace Pg::Graphics
{
	RenderTexture2DArray::RenderTexture2DArray(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		RenderTexture(define, typeid(this).name(), filePath)
	{
		//생성자가 호출된 시점에서 이미 새로운 모델 데이터가 중복없이 만들어진다고 확신됨.
	}

	RenderTexture2DArray::~RenderTexture2DArray()
	{

	}

	void RenderTexture2DArray::InternalLoad()
	{
		//중복되었는지 찾는 것은 외부에서 이루어질 일!
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;
		using Pg::Graphics::Loader::AssetBasic2DLoader;

		AssetBasic2DLoader* t2DLoader = GraphicsResourceManager::Instance()->GetBasic2DLoader();

		std::filesystem::path tPath = _filePath;
		std::string tExt = tPath.extension().string();

		if (_filePath.find(Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX) != std::string::npos)
		{
			//디폴트 일시, Default 전용 Texture2DArray를 로드.
			std::vector<std::string> tSingleTextureNameVector;
			GraphicsResourceHelper::GetTextureNamesFromDefaultTex2DArrayName(_filePath, tSingleTextureNameVector);

			std::vector<RenderTexture2D*> tSingleRenderTexture2DArray;
			for (int i = 0; i < tSingleTextureNameVector.size(); i++)
			{
				auto tRes2D = GraphicsResourceManager::Instance()->GetResourceByName(tSingleTextureNameVector.at(i), Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
				tSingleRenderTexture2DArray.push_back(static_cast<RenderTexture2D*>(tRes2D.get()));
			}
			
			t2DLoader->MultipleRenderTexture2DToTexture2DArray(tSingleRenderTexture2DArray.data(), tSingleRenderTexture2DArray.size(), this);
		}
		else
		{
			if (tExt == ".dds" || tExt == ".DDS")
			{
				t2DLoader->LoadTexture2DArray(true, _filePath, this);
			}

			//디폴트가 아니다! 커스텀 파일 로드.
			assert(tExt == ".pgt2arr" || tExt == ".PGT2ARR");
			t2DLoader->LoadTexture2DArray(false, _filePath, this);
		}
	}

	void RenderTexture2DArray::InternalUnload()
	{

	}
}