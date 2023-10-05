#include "GraphicsResourceHelper.h"

namespace Pg::Graphics::Helper
{
	void GraphicsResourceHelper::Initialize()
	{
		//템플릿 특수화 초기화의 역할을 할 것이다.
		AssetDefineType<Pg::Core::Enums::eAssetDefine::_2DTEXTURE>();
		AssetDefineType<Pg::Core::Enums::eAssetDefine::_3DMODEL>();
		AssetDefineType<Pg::Core::Enums::eAssetDefine::_RENDERMATERIAL>();
	}

}