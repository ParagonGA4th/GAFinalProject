#pragma once
#include <string>

/// <summary>
/// PgShaderParser로 인해 만들어진
/// .pgmat 포맷을 실제로 RenderMaterial로 읽어올 수 있게 하는 코드.
/// 
/// CombinedLoader가 유일한 객체를 들고 있을 것이다.
/// </summary>

namespace Pg
{
	class MaterialParser
	{
	public:
		MaterialParser();
		~MaterialParser();

		void Reset();
		void ParsePgMat(const std::string& pgMatFilePath);
		
		//
		struct MaterialParsingData
		{
			std::string _vertexShaderName;
		};

	private:
		MaterialParsingData* _materialParsingData;
	};
}


