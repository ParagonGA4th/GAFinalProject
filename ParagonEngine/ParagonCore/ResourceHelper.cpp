#include "ResourceHelper.h"
#include <algorithm>
#include <filesystem>

namespace Pg::Core::Helper
{
	using Pg::Core::Enums::eResourceType;

	ResourceHelper::ResourceHelper()
	{
		//
	}

	ResourceHelper::~ResourceHelper()
	{

	}

	Pg::Core::Enums::eResourceType ResourceHelper::Ext2ResourceType(const std::string& extension)
	{
		//대문자로 저장된 문자는 소문자로 변환.
		std::string tExt = extension;

		std::transform(tExt.begin(), tExt.end(), tExt.begin(), [](auto it) {return std::tolower(it); });
		return ExtensionSorter()[extension];
	}

	std::string ResourceHelper::ForcePathUniform(const std::string& filePath)
	{
		std::filesystem::path tPath(filePath);
		return tPath.generic_string();
	}

	ResourceHelper::ExtensionSorter::ExtensionSorter()
	{
		//파일 확장자 ResourceType으로 변환.

		//3DModel
		this->operator[](".fbx") = eResourceType::_3DMODEL;
		this->operator[](".obj") = eResourceType::_3DMODEL;

		//2DTexture
		this->operator[](".png") = eResourceType::_2DTEXTURE;
		this->operator[](".jpg") = eResourceType::_2DTEXTURE;
		this->operator[](".dds") = eResourceType::_2DTEXTURE;

		//Prefab은 따로 양식이 나와야 하고, 이는 별도의 양식이 필요하다.
		//커스텀 확장자가 만들어져야 한다는 말.

		//Material은 따로 양식이 나와야 하고, 이는 별도의 양식이 필요하다.
		//커스텀 확장자가 만들어져야 한다는 말.

		//Shader
		this->operator[](".hlsl") = eResourceType::_PGSHADER;

		//Font는 따로 양식이 나와야 하고, 이는 별도의 양식이 필요하다.
		//커스텀 확장자가 만들어져야 한다는 말.

		this->operator[](".mp3") = eResourceType::_PGSOUND;
		this->operator[](".wav") = eResourceType::_PGSOUND;

		//....
	}

	ResourceHelper::ExtensionSorter::~ExtensionSorter()
	{

	}

}
