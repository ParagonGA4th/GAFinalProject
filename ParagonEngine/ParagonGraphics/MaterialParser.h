#pragma once
#include <string>
#include <memory>
#include "ShaderParsingData.h"
#include "AssetTextureType.h"
#include "RenderMaterial.h"
#include "../ParagonHelper/pugixml.hpp"

/// <summary>
/// PgShaderParser로 인해 만들어진
/// .pgmat 포맷을 실제로 RenderMaterial로 읽어올 수 있게 하는 코드.
/// 
/// CombinedLoader가 유일한 객체를 들고 있을 것이다.
/// </summary>

namespace Pg::Graphics
{
	class Asset3DModelData;
}

namespace Pg::Graphics
{
	class MaterialParser
	{
	public:
		MaterialParser();
		~MaterialParser();

		//커스텀 매터리얼.
		void LoadCustomRenderMaterial(const std::string& pgmatPath, RenderMaterial* renderMat);
	
		//디폴트 매터리얼.
		void LoadDefaultRenderMaterialInstance(const std::string& defInstMatName, RenderMaterial* renderMat);

		//Scene이 바뀔 때마다 업데이트, 아직 Initialize되지 않은 Material만 새로 아이디 부여.
		void RemapMaterialIdForUninitMaterials();

		//중간에 추가될 때 마다 일부 매터리얼 추가만 부여.
		void RemapAppendedMatID();

		//처음 Scene을 로드하기 전에 eInitState를 _NONE으로 리셋.
		void ResetAllKnownInitStates();
	private:
		//큰 레이어들.
		void ParsePgMat(const std::string& pgmatPath, RenderMaterial* renderMat);
		//실제로 리소스 매니저에 있는 VS, PS를 배치한다.
		void PlaceShaders(RenderMaterial* renderMat);
		void LoadRenderMaterial(RenderMaterial* renderMat);
		void ClearPreviousShaderData();

		//디폴트 셰이더들을 렌더 머테리얼 안에 배치한다.
		void PlaceDefaultShaders(RenderMaterial* renderMat);
		void LoadDefaultRenderMaterial(RenderMaterial* renderMat, const std::string& defInstMatName);
		//디폴트 매터리얼에서 기본적으로 올려주는 리소스들을 먼저 올려주기로 약속되었기 때문에, 본래의 기능을 전만큼 다하지는 않을 것이다.
		void PlaceDefaultMaterialTextureArrayBuffer(const std::string& defInstMatName, RenderMaterial::MatShaderIntrinsics* intrinsic,
			Asset3DModelData* asset3dModel, eAssetTextureType type, const std::string& varName, unsigned int registerNum);

	private:
		//작은 레이어들.
		void ParseShaderMat(pugi::xml_node* shdNode, ShaderParsingData* parsingData);	
		void LoadShaderIntrinsics(RenderMaterial::MatShaderIntrinsics* intrinsic, ShaderParsingData* parseData);
		void CreateConstantBuffer(RenderMaterial::MatShaderIntrinsics* intrinsic);
		void GiveMaterialID(RenderMaterial* renderMat);
	private:
		void GetCbVarValue(pugi::xml_node* parNode, eCbVarType varType, CbVarValue& varValue);

	private:
		std::unique_ptr<ShaderParsingData> _vsParseData = nullptr;
		std::unique_ptr<ShaderParsingData> _psParseData = nullptr;

		//리셋되면 다시 2로 돌아간다. 
		//지금까지 Material에게 부여한 ID이다.
		//Scene이 바뀔때마다 모든 Material들의 ID가 다시 부여된다.
		//오히려 2부터 시작하게 되었다. => : 1은 디폴트.
		unsigned int _matIdRecord = 2;
	};
}


