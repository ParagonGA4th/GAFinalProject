#include "GraphicsResourceHelper.h"

//RendererComponent들이 더 추가될 수록 여기도 변해야 한다.
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
		//템플릿 특수화 초기화의 역할을 할 것이다.
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
		//안되는 경우를 걸러낸다. (Base 클래스들)
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::BaseRenderer).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase2D).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::RendererBase3D).name())) == 0)
		{
			PG_WARN("Wrong Renderer Type Name Entered!");
		}

		//원래는 더 깊숙히 들어가야 하지만, 렌더러 컴포넌트가 아직 나오지 않은 지금은 하드코딩.
		//이렇게 되어도 안된다! 더 자세한 렌더러 타입이 들어가야지.
		// 포인터가 들어와야 한다. typeid(자신).name(); 이렇게, 포인터가 들어오기 때문에.
		//2D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::ImageRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::TextRenderer*).name())) == 0)
		{
			return 0;
		}

		//3D 실제.
		if (rendererTypeName.compare(std::string(typeid(Pg::Data::StaticMeshRenderer*).name())) == 0 ||
			rendererTypeName.compare(std::string(typeid(Pg::Data::SkinnedMeshRenderer*).name())) == 0)
		{
			return 1;
		}

		//렌더러가 아닌 것.
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
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
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
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
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
			//여기까지 오면 안된다.
			assert(false && "Invalid Type");
			//유효하지 않다.
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

		std::size_t ind = tRet.find(substring); //Substring 시작 위치 찾기.

		if (ind != std::string::npos) 
		{
			tRet.erase(ind, substring.length());
		}
		else 
		{
			assert(false && "$DefaultMaterial_$이 안 들어있다!");
		}

		return tRet;
	}

	std::string GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(const std::string& defMatName, const std::string& varName, std::string* renderTextureNameSrc, unsigned int cnt)
	{
		//Texture2DArray 감지를 위한 체계.
		std::string tRet = Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX;
		tRet.append(defMatName);
		tRet.append("^");
		tRet.append(varName);

		for (int i = 0; i < cnt; i++)
		{
			//캐럿으로 구분할 것.
			tRet.append("^");
			tRet.append(renderTextureNameSrc[i]);
		}
		//FileName으로 오해되지 않기 위해서.
		//다만, GetLine할 때 하나를 빼어야 한다.
		tRet.append("^.pgt2arr");
		
		return tRet;
	}

	void GraphicsResourceHelper::GetTextureNamesFromDefaultTex2DArrayName(const std::string& defTex2DArrName, std::vector<std::string>& outStringVector)
	{
		//위 함수에서 만든 String을 다시 "해석"하기.
		assert(outStringVector.empty() && "미리 들어온 벡터가 비어 있지 않다!");

		std::string tMain = defTex2DArrName;
		std::size_t ind = defTex2DArrName.find(Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX); //Substring 시작 위치 찾기.

		if (ind != std::string::npos)
		{
			//찾았음.
			tMain.erase(ind, Pg::Defines::GENERATED_MATERIAL_TEXTURE2DARRAY_PREFIX.length());
		}
		else
		{
			assert(false && "$DefaultMaterial_Texture2DArray$가 안 들어있다!");
		}
		
		
		std::string token;
		std::stringstream ss(tMain);

		//VarName 역시 제거해야 한다.
		UINT tCount = 0;

		while (std::getline(ss, token, '^')) 
		{
			if (tCount > 1)
			{
				outStringVector.push_back(token);
			}
			tCount++;
		}

		//확장자 표시 제거. (^.pgt2arr)
		outStringVector.pop_back();

		assert(!outStringVector.empty());
		return;
	}

	void GraphicsResourceHelper::ReadPGT2ARRContents(const std::string& pgt2arrContent, std::vector<std::string>& outStringVector)
	{
		//위 함수에서 만든 String을 다시 "해석"하기.
		assert(outStringVector.empty() && "미리 들어온 벡터가 비어 있지 않다!");

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
				assert(false && "아직 해당 종류 대해서는 디폴트 텍스쳐 경로 준비되지 않았음!");
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
			assert(false && "Default PBR Texture Array로 호환되지 않는 리소스의 Width와 Height.");
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
		std::size_t ind = tMain.find(Pg::Defines::GENERATED_MATERIAL_PREFIX); //Substring 시작 위치 찾기.

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