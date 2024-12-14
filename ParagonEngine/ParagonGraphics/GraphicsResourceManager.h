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
#include "../ParagonUtil/Log.h"

#include "AssetTextureType.h"
#include "SizeTexture.h"

/// <summary>
/// AssetManager�� ���� ����Ǵ� �׷��Ƚ� ���ҽ� ���� ���� �Ŵ���. ������ ��� �Ұ�.
/// ���� ���, ����/������ �����ϰ� �ȴ�.
/// 
/// ���ҽ��� �߰��� ����, LoadResource�� GetResource�� If���� Ȯ��Ǿ�� �Ѵ�!
/// </summary>

//���� ����, Namespace �ٸ����� Friend Class Ȱ���ϱ� ����.
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
	class GraphicsSceneParser;
	class MaterialParser;

	namespace Loader
	{
		class AssetBasic3DLoader;
		class AssetBasic2DLoader;
		class AssetCombinedLoader;
	}
	namespace Helper
	{
		class AssimpBufferParser;
	}
}

namespace Pg::Graphics
{
	class RenderTexture2D;
}

namespace Pg::Graphics::Manager
{
	using Pg::Data::Resources::GraphicsResource;

	class GraphicsResourceManager : public Pg::Core::Singleton<GraphicsResourceManager>
	{
		friend class Pg::Core::Manager::AssetManager;
		friend class Pg::Graphics::Helper::AssimpBufferParser;
		friend class Pg::Graphics::GraphicsMain;
		friend class Pg::Graphics::GraphicsSceneParser;
		friend class Pg::Graphics::MaterialParser;
	public:
		GraphicsResourceManager(); 
		~GraphicsResourceManager();

		//�޸� ������ �������, �����͸� �����Ѵ�.
		Pg::Graphics::Loader::AssetBasic3DLoader* GetBasic3DLoader();
		Pg::Graphics::Loader::AssetBasic2DLoader* GetBasic2DLoader();
		Pg::Graphics::Loader::AssetCombinedLoader* GetCombinedLoader();

		//Ư�� ���ҽ��� �̹� �ε��Ǿ������� Ȯ���Ѵ�.
		bool IsExistResource(const std::string& path);

		//Ư�� ���ҽ��� ���� �̸��� ������ ���� �����̸��� ���ҽ��� �̹� �ε�Ǿ����� ����.
		bool IsExistResourceByName(const std::string& name);

		//Ư�� ���ҽ��� ���� �̸��� Ÿ���� ������ ���� �����̸��� ���ҽ��� �̹� �ε�Ǿ����� ����.
		bool IsExistResourceByNameType(const std::string& name, Pg::Data::Enums::eAssetDefine define);

		//�ش� Mesh�� �̸����� ����Ʈ ���͸����� �����ϴ����� �����Ѵ�.
		bool IsExistDefaultMaterialByMeshName(const std::string& name);

		//���ҽ��� �ִ� ��찡 ������ ��, ���ҽ��� ��ȯ�Ѵ�. (eAssetDefine����)
		std::shared_ptr<GraphicsResource> GetResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		//�̸����� ������ ���ҽ��� ��ȯ���� �� �ִ� �Լ�.
		std::shared_ptr<GraphicsResource> GetResourceByName(const std::string& name, Pg::Data::Enums::eAssetDefine define);

		//�̸����� ������ ���ҽ��� ��ü ���� ��θ� ��ȯ���� �� �ִ� �Լ�.
		std::string GetResourcePathByName(const std::string& name, Pg::Data::Enums::eAssetDefine define);

		//�� Ÿ���� ����ϸ� �̿� ���ϴ� ��� ���ҽ��� ��ȯ�ϴ� �Լ�. ���ϰ� ū ��.
		std::vector<std::shared_ptr<GraphicsResource>> GetAllResourcesByDefine(Pg::Data::Enums::eAssetDefine define);

		//2�� ���ҽ� ������ AssetManager�� ��ġ�� �ʰ� GraphicsResourceManager���� CreateResource�Ǿ��� ��,
		//��������� SecondaryResourceList�� �ش� ������ �߰��� �� �ִ� ���.
		void AddSecondaryResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		//2�������� ������ ����Ʈ�� ��ȯ, ��� ������ nullptr ��ȯ.
		std::map<std::string, Pg::Data::Enums::eAssetDefine>* GetSecondaryResources();

		//�� Iteration���� Ȱ��Ǵ� 2�� �߻� ���ҽ����� �����.
		void ClearSecondaryResourcesList();

		//������ ���� ���� �� Texture�� ������ ���� ����Ʈ ���� ������ �� �ִ�.
		RenderTexture2D* GetDefaultTexture(eAssetTextureType textureType, eSizeTexture textureSize = _512x512);
	private:
		//GraphicsMain����, ���ҽ� �ε��� �� Ȱ��ȴ�.
		void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define);
		void UnloadResource(const std::string& filePath);

		//���ҽ��� �ִ� ��찡 ������ ��, ���ҽ��� ��ȯ�Ѵ�. (���ø� Ÿ������)
		template<typename T>
		std::shared_ptr<T> GetResourceTemplated(const std::string& path);

		//���ҽ��� ���� ��찡 ������ ��, ���ҽ��� �����Ѵ�. 
		template<typename T>
		void CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define);

		//���ҽ��� ��ε��ϴ� �Լ�. AssetManager���� ���ÿ� �ߵ�. ���� �����ϸ� True ��ȯ.
		inline bool DeleteResource(const std::string& path);

	private:
		//������ ���ҽ� ������ ���� ���Ǵ� ��. (MAIN)
		std::unordered_map<std::string, std::weak_ptr<Pg::Data::Resources::GraphicsResource>> _resources;
		//weak_ptr�� ����(expire)�Ǵ� ���� ���� ���ؼ�, ������ Shared_ptr ����. (�������� ���� �뵵�̱⿡, ȿ������ �ʿ�X)
		std::map<std::string, std::shared_ptr<Pg::Data::Resources::GraphicsResource>> _scopeResourceMap;

	private:
		std::unique_ptr<Pg::Graphics::Loader::AssetBasic3DLoader> _asset3DLoader;
		std::unique_ptr<Pg::Graphics::Loader::AssetBasic2DLoader> _asset2DLoader;
		std::unique_ptr<Pg::Graphics::Loader::AssetCombinedLoader> _assetCombinedLoader;
	private:
		//2������ �ּ��� �߻��Ǿ� (Ex. Embedded Texture), AssetManaeger�� ������ �� �� �ְ� �ϴ� Map.
		std::map<std::string, Pg::Data::Enums::eAssetDefine> _toAddSecondaryResourcesMap;

	};

	template<typename T>
	void Pg::Graphics::Manager::GraphicsResourceManager::CreateResource(const std::string& path, Pg::Data::Enums::eAssetDefine define)
	{
		//�̹� AssetManager�� ���������� static�ϰ� üũ �Ϸ�.
		//AssetManager�� ��ϰ� ������ �Ǿ�� �Ѵ�.

		auto res = _resources[path].lock();

		assert((!res) && "������ �̹� ������� ���ҽ��� ���� ������ �ٽ� ������� �ߴٴ� ���̴�. ������ ���ľ�.");

		// ������, ���ø����� ���� ������ ���� �� Load.
		_resources[path] = res = std::make_shared<T>(define, path);

		//Scope ������ ���� shared_ptr�� ������ �����ؾ� �Ѵ�.
		_scopeResourceMap.insert(std::make_pair(path, res));

		res->InternalLoad();

		//�� ���·� ��ȯ����� �Ѵ�.
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[Graphics] ���ҽ� '") + path + "'�� �ش� Ÿ������ ��ȯ�ϴ� ���� �Ұ���!!");
			assert(false);
		}
		return;
	}

	template<typename T>
	std::shared_ptr<T>
		Pg::Graphics::Manager::GraphicsResourceManager::GetResourceTemplated(const std::string& path)
	{
		//shared_ptr�� ��ȯ.
		auto res = _resources[path].lock();

		if (!res)
		{
			PG_ERROR("������ ���� ���ҽ��� ���� ������ ��������� �ߴٴ� ��. ������ ���ľ�.");
			assert(false);
		}

		//�����·� ��ȯ
		auto return_value = std::dynamic_pointer_cast<T>(res);
		if (!return_value)
		{
			throw std::runtime_error(std::string("[Graphics] ���ҽ� '") + path + "'�� �ش� Ÿ������ ��ȯ�ϴ� ���� �Ұ���!!");
			assert(false);
		}
		return return_value;

	}

	bool GraphicsResourceManager::DeleteResource(const std::string& path)
	{
		//���ҽ� Ȱ���� ���� weak_ptr.lock()���� üũ.
		auto res = _resources[path].lock();

		//���� ���� �� �ִ� Resource�� ������?
		if (res)
		{
			res->InternalUnload();
			_resources.erase(path);

			//���� std::shared_ptr ������ ������ �ϸ� �ȵȴ�.
			_scopeResourceMap.erase(path);

			return true;
		}
		else
		{
			return false;
		}
	}
}


