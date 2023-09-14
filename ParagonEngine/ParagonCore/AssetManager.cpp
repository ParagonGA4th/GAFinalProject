#include "AssetManager.h"

namespace Pg::Core::Manager
{
	AssetManager::AssetManager()
	{

	}

	AssetManager::~AssetManager()
	{

	}

	void AssetManager::Unload(const std::string& filepath)
	{
		//Path 표기 방식 일원화.
		std::string path = Pg::Core::Helper::ResourceHelper::ForcePathUniform(filepath);

		//우선적으로 일단 리스트에 값이 있는지를 확인.
		if (this->_resources.contains(path))
		{
			//명시적으로 두 인스턴스에 모두 삭제(및 언로드)를 진행해준다. 
			//템플릿을 쓰지 않기 위해, 둘 다 삭제 코드, 내부적으로 해당하는지 아닌지 반환.
			bool tIsEngineDelete = EngineResourceManager::Instance()->DeleteResource(path);
			bool tIsGraphicsDelete = GraphicsResourceManager::Instance()->DeleteResource(path);
			
			assert((tIsEngineDelete ^ tIsGraphicsDelete) && "하나만 삭제하는 경우가 발생하지 않음, 오류");
		
			//이제 메인 AssetManager에서 값을 삭제.
			_resources.erase(path);
		}
		else
		{
			//리스트에 값이 없다는 뜻이다. 언로드될 수도 없음.
			return;
		}
	}



}
