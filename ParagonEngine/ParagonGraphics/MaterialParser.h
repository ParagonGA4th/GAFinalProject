#pragma once
#include <string>
#include <memory>
#include "ShaderParsingData.h"

/// <summary>
/// PgShaderParser로 인해 만들어진
/// .pgmat 포맷을 실제로 RenderMaterial로 읽어올 수 있게 하는 코드.
/// 
/// CombinedLoader가 유일한 객체를 들고 있을 것이다.
/// </summary>

namespace Pg::Graphics
{
	class MaterialParser
	{
	public:
		MaterialParser();
		~MaterialParser();

		void Reset();
		void ParsePgMat(const std::string& pgmatPath);

	private:
		void ParseShaderMat(pugi::xml_node* shdNode, ShaderParsingData* parsingData);

	private:
		eCbVarType GetCbVarType(const std::string& varString);
		void GetCbVarValue(pugi::xml_node* parNode, eCbVarType varType, CbVarValue& varValue);

		eTexVarType GetTexVarType(const std::string& varString);
		eTexReturnVarType GetTexReturnVarType(const std::string& varString);

	private:
		std::unique_ptr<ShaderParsingData> _vsParseData = nullptr;
		std::unique_ptr<ShaderParsingData> _psParseData = nullptr;
	};
}


