#pragma once
#include <string>
#include <memory>
#include "ShaderParsingData.h"
#include "RenderMaterial.h"
#include "../ParagonUtil/pugixml.hpp"

/// <summary>
/// PgShaderParser로 인해 만들어진
/// .pgmat 포맷을 실제로 RenderMaterial로 읽어올 수 있게 하는 코드.
/// 
/// CombinedLoader가 유일한 객체를 들고 있을 것이다.
/// </summary>

namespace Pg::Graphics
{

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

	private:
		//큰 레이어들.
		void ParsePgMat(const std::string& pgmatPath);
		//실제로 리소스 매니저에 있는 VS, PS를 배치한다.
		void PlaceShaders(RenderMaterial* renderMat);
		void LoadRenderMaterial(RenderMaterial* renderMat);
		void ClearPreviousShaderData();

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
	
		//리셋되도 클리어되지 않는다. 지금까지 Material에게 부여한 ID이다.
		//1부터 시작.
		unsigned int _matIdRecord = 1;
	};
}


