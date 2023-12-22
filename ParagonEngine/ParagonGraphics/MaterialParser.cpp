#include "MaterialParser.h"
#include "../ParagonUtil/pugixml.hpp"

namespace Pg
{
	
	MaterialParser::MaterialParser()
	{

	}

	MaterialParser::~MaterialParser()
	{

	}

	void MaterialParser::ParsePgMat(const std::string& pgMatFilePath)
	{
		pugi::xml_document doc;
		doc.load(pgMatFilePath.c_str());

		//.pgmat 파일 형식을 읽어들이기.
		pugi::xml_node tRootNode = doc.child("PgMaterial");
		pugi::xml_node tVSNode = tRootNode.child("VertexShader");

		///231222 Material Parser 하는 중!
		////VertexShader FileName 파싱하기.
		//{
		//	pugi::xml_node tVSFileNameNode = tVSNode.child("FileName");
		//	_vertexShaderName = tVSFileNameNode.text().get();
		//}
		////
		
	}

	void MaterialParser::Reset()
	{

	}

}