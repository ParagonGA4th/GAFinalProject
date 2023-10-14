#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <type_traits>
#include <map>
#include <cassert>

#include "../ParagonData/GraphicsResource.h"
#include "../ParagonProcess/CoreSingleton.h"

/// <summary>
/// AssetManager에 의해 제어되는 그래픽스 리소스 관리 전담 매니저. 독단적 사용 불가.
/// 관리 대신, 생성/삭제만 전담하게 된다.
/// 
/// 리소스가 추가될 수록, LoadResource와 GetResource의 If문이 확장되어야 한다!
/// </summary>

//전방 선언, Namespace 다름에도 Friend Class 활용하기 위해.
namespace Pg::Core
{
	class ProcessMain;
	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Graphics
{
	class GraphicsMain;
	namespace Loader
	{
		class AssetBasic3DLoader;
		class AssetBasic2DLoader;
	}
}

namespace Pg::Graphics::Manager
{
	using Pg::Data::Resources::GraphicsResource;

	class GraphicsResourceManager : public Pg::Core::Singleton<GraphicsResourceManager>
	{
		friend class Pg::Core::Manager::AssetManager;
		friend class Pg::Graphics::GraphicsMain;
	public:
		GraphicsResourceManager();
		~GraphicsResourceManager();

		//메모리 관리와 관계없이, 포인터를 리턴한다.
		Pg::Graphics::Loader::AssetBasic3DLoader* GetBasic3DLoader();
		Pg::Graphics::Loader::AssetBasic2DLoader* GetBasic2DLoader();

		//특정 리소스가 이미 로딩되었는지를 확인한다.
		bool IsExistResource(const std::string& path);

		//리소스가 있는 경우가 강제될 때, 리소스를 반환한다. (eAssetDefine으로)
		std::shared_ptr<GraphicsResource> GetResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		//2차적으로 생성된 리스트들 반환, 목록 없으면 nullptr 반환.
		std::map<std::string, Pg::Data::Enums::eAssetDefine>* GetSecondaryResources();

		//한 Iteration마다 활용되는 2차 발생 리소스들을 지운다.
		void ClearSecondaryResourcesList();
	private:
		//GraphicsMain에서, 리소스 로드할 때 활용된다.
		void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define);
		void UnloadResource(const std::string& filePath);

		//리소스가 있는 경우가 강제될 때, 리소스를 반환한다. (템플릿 타입으로)
		template<typename T>
		std::shared_ptr<T> GetResourceTemplated(const std::string& path);

		//리소스가 없는 경우가 강제될 때, 리소스를 생성한다. 
		template<typename T>
		void CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		//리소스를 언로드하는 함수. AssetManager에서 동시에 발동. 삭제 성공하면 True 반환.
		inline bool DeleteResource(const std::string& path);

	private:
		//실제로 리소스 관리를 위해 사용되는 맵. (MAIN)
		std::unordered_map<std::string, std::weak_ptr<Pg::Data::Resources::GraphicsResource>> _resources;
		//weak_ptr가 만기(expire)되는 것을 막기 위해서, 별도로 Shared_ptr 보관. (스코프만 유지 용도이기에, 효율적일 필요X)
		std::map<std::string, std::shared_ptr<Pg::Data::Resources::GraphicsResource>> _scopeResourceMap;

	private:
		std::unique_ptr<Pg::Graphics::Loader::AssetBasic3DLoader> _asset3DLoader;
		std::unique_ptr<Pg::Graphics::Loader::AssetBasic2DLoader> _asset2DLoader;
	private:
		//2차적인 애셋이 발생되어 (Ex. Embedded Texture), AssetManaeger랑 연동을 할 수 있게 하는 Map.
		std::map<std::string, Pg::Data::Enums::eAssetDefine> _toAddSecondaryResourcesMap;

	};

	template<typename T>
	void Pg::Graphics::Manager::GraphicsResourceManager::CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		//이미 AssetManager의 시점에서는 static하게 체크 완료.
		//AssetManager의 목록과 연동이 되어야 한다.

		auto res = _resources[path].lock();

		assert((!res) && "막히면 이미 만들어진 리소스를 로직 상으로 다시 만드려고 했다는 뜻이다. 로직을 고쳐야.");

		// 없으면, 템플릿으로 들어온 값으로 생성 및 Load.
		_resources[path] = res = std::make_shared<T>(define, path);
		res->InternalLoad();

		//원 형태로 반환해줘야 한다.
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[Graphics] 리소스 '") + path + "'를 해당 타입으로 변환하는 것이 불가능!!");
			assert(false);
		}

		//Scope 유지를 위해 shared_ptr를 별도로 보관해야 한다.
		_scopeResourceMap.insert(std::make_pair(path, res));

		return;
	}

	template<typename T>
	std::shared_ptr<T>
		Pg::Graphics::Manager::GraphicsResourceManager::GetResourceTemplated(const std::string& path)
	{
		//shared_ptr로 변환.
		auto res = _resources[path].lock();

		assert(res && "막히면 없는 리소스를 로직 상으로 가져오라고 했다는 뜻. 로직을 고쳐야.");

		//원형태로 변환
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[Graphics] 리소스 '") + path + "'를 해당 타입으로 변환하는 것이 불가능!!");
			assert(false);
		}
		return return_value;

	}

	bool GraphicsResourceManager::DeleteResource(const std::string& path)
	{
		//리소스 활용을 위해 weak_ptr.lock()으로 체크.
		auto res = _resources[path].lock();

		//만약 지울 수 있는 Resource가 있으면?
		if (res)
		{
			res->InternalUnload();
			_resources.erase(path);

			//이제 std::shared_ptr 스코프 유지도 하면 안된다.
			_scopeResourceMap.erase(path);

			return true;
		}
		else
		{
			return false;
		}
	}
}


