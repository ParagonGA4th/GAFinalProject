#include "GraphicsResourceHelper.h"

//RendererComponent들이 더 추가될 수록 여기도 변해야 한다.
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h" 

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/StaticMeshRenderer.h"

#include "../ParagonUtil/Log.h"

#include <cassert>

namespace Pg::Graphics::Helper
{
	void GraphicsResourceHelper::Initialize()
	{
		//템플릿 특수화 초기화의 역할을 할 것이다.
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_2DTEXTURE>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL>();
	}

	short GraphicsResourceHelper::IsRenderer3D(const std::string& rendererTypeName)
	{
		//안되는 경우를 걸러낸다. (Base 클래스들)
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::BaseRenderer).name())) == 0 || 
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase2D).name())) == 0 || 
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase3D).name())) == 0)
		{
			PG_WARN("Wrong Renderer Type Name Entered!");
		}

		//원래는 더 깊숙히 들어가야 하지만, 렌더러 컴포넌트가 아직 나오지 않은 지금은 하드코딩.
		//이렇게 되어도 안된다! 더 자세한 렌더러 타입이 들어가야지.
		// 포인터가 들어와야 한다. typeid(this).name(); 이렇게, 포인터가 들어오기 때문에.
		//2D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0 )
		{
			return 0;
		}

		//3D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0)
		{
			return 1;
		}

		//렌더러가 아닌 것.
		return -1;
	}

}