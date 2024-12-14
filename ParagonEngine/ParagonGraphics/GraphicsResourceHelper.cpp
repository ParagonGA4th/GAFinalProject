#include "GraphicsResourceHelper.h"

//RendererComponent���� �� �߰��� ���� ���⵵ ���ؾ� �Ѵ�.
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/BaseRenderer.h"
#include "../ParagonData/RendererBase2D.h"
#include "../ParagonData/RendererBase3D.h" 

#include "../ParagonData/ImageRenderer.h"
#include "../ParagonData/TextRenderer.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/SkinnedMeshRenderer.h"

#include "../ParagonUtil/Log.h"

#include <cassert>
#include <sstream>
#include <filesystem>

namespace Pg::Graphics::Helper
{	
	void GraphicsResourceHelper::Initialize()
	{
		//���ø� Ư��ȭ �ʱ�ȭ�� ������ �� ���̴�.
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE1D>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2D>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_TEXTURECUBE>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_FONT>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_3DMODEL>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_CUBEMAP>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_RENDERMATERIAL>();
		AssetDefineType<Pg::Data::Enums::eAssetDefine::_ANIMATION>();
	}

	short GraphicsResourceHelper::IsRenderer3D(const std::string& rendererTypeName)
	{
		//�ȵǴ� ��츦 �ɷ�����. (Base Ŭ������)
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::BaseRenderer).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase2D).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase3D).name())) == 0)
		{
			PG_WARN("Wrong Renderer Type Name Entered!");
		}

		//������ �� ����� ���� ������, ������ ������Ʈ�� ���� ������ ���� ������ �ϵ��ڵ�.
		//�̷��� �Ǿ �ȵȴ�! �� �ڼ��� ������ Ÿ���� ������.
		// �����Ͱ� ���;� �Ѵ�. typeid(�ڽ�).name(); �̷���, �����Ͱ� ������ ������.
		//2D ����.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0)
		{
			return 0;
		}

		//3D ����.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
		{
			return 1;
		}

		//�������� �ƴ� ��.
		return -1;
	}

	eCbVarType GraphicsResourceHelper::GetCbVarType(const std::string& varString)
	{
		if (varString.compare("bool") == 0)
		{
			return _CB_BOOL;
		}
		else if (varString.compare("uint") == 0)
		{
			return _CB_UINT;
		}
		else if (varString.compare("int") == 0)
		{
			return _CB_INT;
		}
		else if (varString.compare("float") == 0)
		{
			return _CB_FLOAT;
		}
		else if (varString.compare("float2") == 0)
		{
			return _CB_FLOAT2;
		}
		else if (varString.compare("float3") == 0)
		{
			return _CB_FLOAT3;
		}
		else if (varString.compare("float4") == 0)
		{
			return _CB_FLOAT4;
		}
		else
		{
			//������� ���� �ȵȴ�.
			assert(false && "Invalid Type");
			//��ȿ���� �ʴ�.
			return _CB_BOOL;
		}
	}

	eTexVarType GraphicsResourceHelper::GetTexVarType(const std::string& varString)
	{
		if (varString.compare("Texture1D") == 0)
		{
			return _TEX_TEXTURE1D;
		}
		else if (varString.compare("Texture2D") == 0)
		{
			return _TEX_TEXTURE2D;
		}
		else if (varString.compare("Texture2DArray") == 0)
		{
			return _TEX_TEXTURE2DARRAY;
		}
		else if (varString.compare("TextureCube") == 0)
		{
			return _TEX_TEXTURECUBE;
		}
		else
		{
			//������� ���� �ȵȴ�.
			assert(false && "Invalid Type");
			//��ȿ���� �ʴ�.
			return _TEX_TEXTURE1D;
		}
	}

	eTexReturnVarType GraphicsResourceHelper::GetTexReturnVarType(const std::string& varString)
	{
		if (varString.compare("PG_RETURN_TYPE_UNORM") == 0)
		{
			return _TEXRET_UNORM;
		}
		else if (varString.compare("PG_RETURN_TYPE_SNORM") == 0)
		{
			return _TEXRET_SNORM;
		}
		else if (varString.compare("PG_RETURN_TYPE_SINT") == 0)
		{
			return _TEXRET_SINT;
		}
		else if (varString.compare("PG_RETURN_TYPE_UINT") == 0)
		{
			return _TEXRET_UINT;
		}
		else if (varString.compare("PG_RETURN_TYPE_FLOAT") == 0)
		{
			return _TEXRET_FLOAT;
		}
		else
		{
			//������� ���� �ȵȴ�.
			assert(false && "Invalid Type");
			//��ȿ���� �ʴ�.
			return _TEXRET_UNORM;
		}
	}

	Pg::Data::Enums::eAssetDefine GraphicsResourceHelper::GetAssetDefine(eTexVarType texVarType)
	{
		using Pg::Data::Enums::eAssetDefine;
		eAssetDefine tRet;

		switch (texVarType)
		{
			case Pg::Graphics::_TEX_TEXTURE1D:
			{
				tRet = eAssetDefine::_TEXTURE1D;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURE2D:
			{
				tRet = eAssetDefine::_TEXTURE2D;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURE2DARRAY:
			{
				tRet = eAssetDefine::_TEXTURE2DARRAY;
			}
			break;
			case Pg::Graphics::_TEX_TEXTURECUBE:
			{
				tRet = eAssetDefine::_TEXTURECUBE;
			}
			break;
		}

		return tRet;
	}

	std::string GraphicsResourceHelper::GetDefaultMaterialNameFromMeshName(const std::string& name)
	{
		std::string tStr = Pg::Defines::GENERATED_MATERIAL_PREFIX;
		tStr += name;
		return tStr;
	}

	std::string GraphicsResourceHelper::GetMeshNameFromDefaultMaterialName(const std::string& name)
	{
		std::string tRet = name;
		std::string substring = Pg::Defines::GENERATED_MATERIAL_PREFIX;

		std::size_t ind = tRet.find(substring); //Substring ���� ��ġ ã��.

		if (ind != std::string::npos) 
		{
			tRet.erase(ind, substring.length());
		}
		else 
		{
			assert(false && "$DefaultMaterial_$�� �� ����ִ�!");
		}

		return tRet;
	}

	std::string GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(const std::string& defMatName, const std::string& varName, std::string* renderTextureNameSrc, unsigned int cnt)
	{
		//Texture2DArray ������ ���� ü��.
		std::string tRet = Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX;
		tRet.append(defMatName);
		tRet.append("^");
		tRet.append(varName);

		for (int i = 0; i < cnt; i++)
		{
			//ĳ������ ������ ��.
			tRet.append("^");
			tRet.append(renderTextureNameSrc[i]);
		}
		//FileName���� ���ص��� �ʱ� ���ؼ�.
		//�ٸ�, GetLine�� �� �ϳ��� ����� �Ѵ�.
		tRet.append("^.pgt2arr");
		
		return tRet;
	}

	void GraphicsResourceHelper::GetTextureNamesFromDefaultTex2DArrayName(const std::string& defTex2DArrName, std::vector<std::string>& outStringVector)
	{
		//�� �Լ����� ���� String�� �ٽ� "�ؼ�"�ϱ�.
		assert(outStringVector.empty() && "�̸� ���� ���Ͱ� ��� ���� �ʴ�!");

		std::string tMain = defTex2DArrName;
		std::size_t ind = defTex2DArrName.find(Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX); //Substring ���� ��ġ ã��.

		if (ind != std::string::npos)
		{
			//ã����.
			tMain.erase(ind, Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX.length());
		}
		else
		{
			assert(false && "$DefaultMaterial_Texture2DArray$�� �� ����ִ�!");
		}
		
		
		std::string token;
		std::stringstream ss(tMain);

		//VarName ���� �����ؾ� �Ѵ�.
		UINT tCount = 0;

		while (std::getline(ss, token, '^')) 
		{
			if (tCount > 1)
			{
				outStringVector.push_back(token);
			}
			tCount++;
		}

		//Ȯ���� ǥ�� ����. (^.pgt2arr)
		outStringVector.pop_back();

		assert(!outStringVector.empty());
		return;
	}

	void GraphicsResourceHelper::ReadPGT2ARRContents(const std::string& pgt2arrContent, std::vector<std::string>& outStringVector)
	{
		//�� �Լ����� ���� String�� �ٽ� "�ؼ�"�ϱ�.
		assert(outStringVector.empty() && "�̸� ���� ���Ͱ� ��� ���� �ʴ�!");

		std::string token;
		std::stringstream ss(pgt2arrContent);
		while (std::getline(ss, token, '^'))
		{
			outStringVector.push_back(token);
		}

		assert(!outStringVector.empty());
	}

	std::string GraphicsResourceHelper::GetDefaultTexturePath(eAssetTextureType textureType, eSizeTexture sizeType)
	{
		using namespace Pg::Defines;

		//256/512/1024/2048
		const std::string* tDiffuseTexture[4] = { &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_256, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_512, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_1024, &ASSET_DEFAULT_DIFFUSE_TEXTURE_PATH_2048};
		const std::string* tNormalTexture[4] = { &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_256, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_512, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_1024, &ASSET_DEFAULT_NORMAL_TEXTURE_PATH_2048 };
		const std::string* tArmTexture[4] = { &ASSET_DEFAULT_ARM_TEXTURE_PATH_256, &ASSET_DEFAULT_ARM_TEXTURE_PATH_512, &ASSET_DEFAULT_ARM_TEXTURE_PATH_1024, &ASSET_DEFAULT_ARM_TEXTURE_PATH_2048 };
		const std::string* tSpecularTexture[4] = { &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_256, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_512, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_1024, &ASSET_DEFAULT_SPECULAR_TEXTURE_PATH_2048 };

		unsigned int tIndex = (unsigned int)sizeType;

		switch (textureType)
		{
			case PG_TextureType_DIFFUSE:
			{
				return *(tDiffuseTexture[tIndex]);
			}
			break;
			case PG_TextureType_NORMALS:
			{
				return *(tNormalTexture[tIndex]);
			}
			break;
			case PG_TextureType_ARM:
			{
				return *(tArmTexture[tIndex]);
			}
			break;
			case PG_TextureType_SPECULAR:
			{
				return *(tSpecularTexture[tIndex]);
			}
			break;
			default:
			{
				assert(false && "���� �ش� ���� ���ؼ��� ����Ʈ �ؽ��� ��� �غ���� �ʾ���!");
			}
			break;
		}
	}

	Pg::Graphics::eSizeTexture GraphicsResourceHelper::GetSizeTextureFromUINT(unsigned int width, unsigned int height)
	{
		Pg::Graphics::eSizeTexture tRet = static_cast<Pg::Graphics::eSizeTexture>(0);

		if (width == 256 && height == 256)
		{
			tRet = _256x256;
		}
		else if (width == 512 && height == 512)
		{
			tRet = _512x512;
		}
		else if (width == 1024 && height == 1024)
		{
			tRet = _1024x1024;
		}
		else if (width == 2048 && height == 2048)
		{
			tRet = _2048x2048;
		}
		else
		{
			assert(false && "Default PBR Texture Array�� ȣȯ���� �ʴ� ���ҽ��� Width�� Height.");
		}

		return tRet;
	}

	Pg::Graphics::eTextureExtension GraphicsResourceHelper::GetTexExtFromPath(const std::string& path)
	{
		std::filesystem::path tPath(path);
		std::string tExtString = tPath.extension().string();
		
		if (tExtString == ".dds" || tExtString == ".DDS")
		{
			return eTextureExtension::_DDS;
		}
		if (tExtString == ".tga" || tExtString == ".TGA")
		{
			return eTextureExtension::_TGA;
		}
		if (tExtString == ".png" || tExtString == ".PNG")
		{
			return eTextureExtension::_PNG;
		}
		if (tExtString == ".exr" || tExtString == ".EXR")
		{
			return eTextureExtension::_EXR;
		}
		return eTextureExtension::_UNMANAGED;
	}

	bool GraphicsResourceHelper::IsMaterialDefaultMaterial(RenderMaterial* renderMat)
	{
		std::string tMain = renderMat->GetFileName();
		std::size_t ind = tMain.find(Pg::Defines::GENERATED_MATERIAL_PREFIX); //Substring ���� ��ġ ã��.

		if (ind != std::string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}