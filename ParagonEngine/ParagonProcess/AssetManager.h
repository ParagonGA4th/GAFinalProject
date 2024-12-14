#pragma once
#include "../ParagonData/AssetDefines.h"

#include <string>
#include <unordered_map>
#include <vector>

/// <summary>
/// ���� ���� ���� ��� �ּ�/���ҽ� ������ �Ѱ��� �Ŵ���.
/// �������� ����� Engine, Graphics�� ���� ���ҽ� �Ŵ������� ����ϰ�, 
/// �� AssetManager�� ������ ������ �����Ǵ� ��ο� ����ϴ�.
/// singleton-cpp�� �ν��Ͻ��� �����ȴ�.
/// </summary>

namespace Pg::Core
{
	class ProcessMain;
	class IEngine;
	class IGraphics;
}

namespace Pg::Core::Manager
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();
		
		//�⺻ ���ҽ� ���� ���� ����.
		void InitializeDefaults();

		//AssetManager �ʱ�ȭ. Engine/Graphics�� ���ҽ� �Ŵ����� �޾ƿ´�.
		void Initialize(Pg::Core::ProcessMain* core, const std::string& resourceListPath);

		//������ ���� Graphics, Engine�� ���ҽ� �Ŵ����� �����ϱ� ����, ���� ������Ʈ.
		void Update(Pg::Core::IEngine* engine, Pg::Core::IGraphics* graphics);

		//�ּ� �Ŵ��� Ư����, ���� �̸��� ���� ���ϵ��� ���� �� ������ �ȵȴ�.
		void AssureNoNameDuplicates();

		//���ҽ��� ������ �߾� AssetManager �ȿ� �����ϴ����� �Ǵ����ش�. (*���� ���� ��η� �ٸ� Asset Enum �� ������ ����� �� �� �����ؾ�.)
		bool IsExistResource(const std::string& filepath);

		//���ҽ��� �ε��� Queue�� �ִ´�.
		void LoadResource(const std::string& filepath, Pg::Data::Enums::eAssetDefine define);

		//���ҽ��� ��ε��� Queue�� �ִ´�.
		void UnloadResource(const std::string& filepath);
	
		//��� ���� ��� ��ü Path List ��ȯ.
		std::vector<std::string> GetResourcesPathByDefine(Pg::Data::Enums::eAssetDefine define);
		
		//��� ���� ��� ��ü Name List ��ȯ.
		std::vector<std::string> GetResourcesNameByDefine(Pg::Data::Enums::eAssetDefine define);

		//��� �� Material�� ������ �� ��, Default Material�� ���� ��ȯ.
		std::vector<std::string> GetMaterialNamesWithoutDefault();

	private:
		//FBX�� �鿩�� �� �Ӻ����� �ؽ��� ���� ��, 
		//������ �׷��Ƚ�/�������� ���� �� ���� �ε� �ڿ� �߰��ؾ� �ϴ� ���ҽ� ���� -> ȣȯ ���� �Լ�.
		void CheckForGraphicsToProcessLoad(Pg::Core::IGraphics* graphics);

	private:
		//Ư�� ��ġ�� ���� ���ҽ��� ���� ��� ���Ǵ� ����Ʈ ���ҽ� �ε�.
		void LoadDefaultResources();

		//�ӽ÷� �̸� �ε��� ���ҽ����� �ε��س��´�!
		void LoadResourcesFromCSV(const std::string& resourceListPath);

	private:
		Pg::Core::ProcessMain* _coreMain;

		//������ �����ϴ� ���ҽ� ��. <���� ��� / Asset�� ����>
		std::unordered_map<std::string, Pg::Data::Enums::eAssetDefine> _resourceMap;

		//�� �����Ӹ��� ������Ʈ�Ǵ� ���ҽ� ��� (�ε� ����)
		std::vector<std::pair<std::string, Pg::Data::Enums::eAssetDefine>> _perFrameToLoadResources;

		//�� �����Ӹ��� ������Ʈ�Ǵ� ���ҽ� ��� (��ε� ����)
		std::vector<std::string> _perFrameToUnloadResources;
	};
}
