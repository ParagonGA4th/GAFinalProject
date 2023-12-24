#include "MaterialParser.h"
#include "RenderMaterial.h"


#include <algorithm>
#include <cassert>

namespace Pg::Graphics
{
	
	MaterialParser::MaterialParser()
	{

	}

	MaterialParser::~MaterialParser()
	{

	}

	//대표적인 예시 : "test4.pgmat"
	void MaterialParser::ParsePgMat(const std::string& pgmatPath)
	{
		//Material Parsing Data 객체를 만들기. 
		_vsParseData = std::make_unique<ShaderParsingData>();
		_psParseData = std::make_unique<ShaderParsingData>();

		pugi::xml_document doc;
		doc.load_file(pgmatPath.c_str());

		//.pgmat 파일 형식을 읽어들이기.
		pugi::xml_node tRootNode = doc.child("PgMaterial");

		//VS
		pugi::xml_node tVSNode = tRootNode.child("VertexShader");
		ParseShaderMat(&tVSNode, _vsParseData.get());

		//PS
		pugi::xml_node tPSNode = tRootNode.child("PixelShader");
		ParseShaderMat(&tPSNode, _psParseData.get());

		assert("");
	}

	void MaterialParser::LoadRenderMaterial(RenderMaterial* renderMat)
	{
		///231225, 이거 하고 있었음.
	}

	void MaterialParser::Reset()
	{
		if (_vsParseData != nullptr)
		{
			_vsParseData.release();
		}

		if (_psParseData != nullptr)
		{
			_psParseData.release();
		}
	}

	void MaterialParser::ParseShaderMat(pugi::xml_node* shdNode, ShaderParsingData* parsingData)
	{
		//Shader : FileName 파싱하기.
		{
			pugi::xml_node tVSFileNameNode = shdNode->child("FileName");
			parsingData->_shaderName = tVSFileNameNode.text().get();
		}

		//Shader : Constant Buffer 파싱하기.
		{
			pugi::xml_node tCbNode = shdNode->child("ConstantBuffer");
			{
				pugi::xml_node tCbNameNode = tCbNode.child("BufferName");
				parsingData->_cbData._name = tCbNameNode.text().get();
			}
			{
				pugi::xml_node tCbByteSizeNode = tCbNode.child("ByteCount");
				//UINT로 바꾸어 받기.
				parsingData->_cbData._byteCount = std::stoul(tCbByteSizeNode.text().get());
			}
			{
				pugi::xml_node tCbRegCountNode = tCbNode.child("RegisterCount");
				//UINT로 바꾸어 받기.
				parsingData->_cbData._registerCount = std::stoul(tCbRegCountNode.text().get());
			}
			{
				pugi::xml_node tCbVarOrderNode = tCbNode.child("VariableOrder");

				//VarOrder의 자식 모든 Node Iteration => 후에 연동하는데 활용될 것. 인덱스 보존은 중요!
				for (pugi::xml_node tVarOrderChildNode = tCbVarOrderNode.first_child();
					tVarOrderChildNode; tVarOrderChildNode = tVarOrderChildNode.next_sibling())
				{
					parsingData->_cbData._varData.push_back(std::make_pair(tVarOrderChildNode.text().get(), CbVariableParsingData()));
				}
			}
			{
				pugi::xml_node tCbVarNode = tCbNode.child("Variables");

				//모든 Child Iteration.
				for (pugi::xml_node tVarChildNode = tCbVarNode.first_child();
					tVarChildNode; tVarChildNode = tVarChildNode.next_sibling())
				{
					//Attribute 들어가기.
					std::string tVarName = tVarChildNode.attribute("Name").value();

					//VariableOrder와 부합하는 요소를 찾기.
					auto it = std::find_if(parsingData->_cbData._varData.begin(), parsingData->_cbData._varData.end(),
						[&tVarName](const std::pair<std::string, CbVariableParsingData>& element) { return element.first == tVarName; });

					if (it == parsingData->_cbData._varData.end())
					{
						assert(false && ".pgmat에 애초에 발견되지 못하는 값이 있으면 안됨.");
					}
					it->second._varType = GetCbVarType(tVarChildNode.attribute("Type").value());
					it->second._size = std::stoul(tVarChildNode.attribute("Size").value());
					it->second._startOffset = std::stoul(tVarChildNode.attribute("StartOffset").value());
					GetCbVarValue(&tVarChildNode, it->second._varType, it->second._varValue);
				}
			}
		}
		{
			pugi::xml_node tTexNode = shdNode->child("Textures");
			{
				pugi::xml_node tTexVarOrderNode = tTexNode.child("VariableOrder");

				//VarOrder의 자식 모든 Node Iteration => 후에 연동하는데 활용될 것. 인덱스 보존은 중요!
				for (pugi::xml_node tTexOrderChildNode = tTexVarOrderNode.first_child();
					tTexOrderChildNode; tTexOrderChildNode = tTexOrderChildNode.next_sibling())
				{
					parsingData->_texData._varData.push_back(std::make_pair(tTexOrderChildNode.text().get(), TexVariableParsingData()));
				}
			}
			{
				pugi::xml_node tTexVarNode = tTexNode.child("Variables");

				for (pugi::xml_node tVarChildNode = tTexVarNode.first_child();
					tVarChildNode; tVarChildNode = tVarChildNode.next_sibling())
				{
					//Attribute 들어가기.
					std::string tVarName = tVarChildNode.attribute("Name").value();

					//VariableOrder와 부합하는 요소를 찾기.
					auto it = std::find_if(parsingData->_texData._varData.begin(), parsingData->_texData._varData.end(),
						[&tVarName](const std::pair<std::string, TexVariableParsingData>& element) { return element.first == tVarName; });

					if (it == parsingData->_texData._varData.end())
					{
						assert(false && ".pgmat에 애초에 발견되지 못하는 값이 있으면 안됨.");
					}

					it->second._textureType = GetTexVarType(tVarChildNode.attribute("Type").value());
					it->second._srvReturnType = GetTexReturnVarType(tVarChildNode.attribute("ResourceReturnType").value());
					it->second._registerCount = std::stoul(tVarChildNode.attribute("RegisterCount").value());
					it->second._fileName = tVarChildNode.attribute("Value").value();
				}
			}
			//Texture1D
			//Texture2D
			//Texture2DArray
			//TextureCube
		}
	}


	eCbVarType MaterialParser::GetCbVarType(const std::string& varString)
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

	void MaterialParser::GetCbVarValue(pugi::xml_node* parNode, eCbVarType varType, CbVarValue& varValue)
	{
		if (varType == _CB_BOOL)
		{
			pugi::xml_node tXNode = parNode->child("x");
			int tVal = std::stoi(tXNode.text().get());
			bool tBoolVal = static_cast<bool>(tVal);

			varValue = tBoolVal;
		}
		else if (varType == _CB_UINT)
		{
			pugi::xml_node tXNode = parNode->child("x");
			unsigned int tVal = std::stoul(tXNode.text().get());
			varValue = tVal;
		}
		else if (varType == _CB_INT)
		{
			pugi::xml_node tXNode = parNode->child("x");
			int tVal = std::stoi(tXNode.text().get());
			varValue = tVal;
		}
		else if (varType == _CB_FLOAT)
		{
			pugi::xml_node tXNode = parNode->child("x");
			float tVal = std::stof(tXNode.text().get());
			varValue = tVal;
		}
		else if (varType == _CB_FLOAT2)
		{
			pugi::xml_node tXNode = parNode->child("x");
			float tValX = std::stof(tXNode.text().get());

			pugi::xml_node tYNode = parNode->child("y");
			float tValY = std::stof(tYNode.text().get());

			varValue = DirectX::XMFLOAT2(tValX, tValY);
		}
		else if (varType == _CB_FLOAT3)
		{
			pugi::xml_node tXNode = parNode->child("x");
			float tValX = std::stof(tXNode.text().get());

			pugi::xml_node tYNode = parNode->child("y");
			float tValY = std::stof(tYNode.text().get());

			pugi::xml_node tZNode = parNode->child("z");
			float tValZ = std::stof(tZNode.text().get());

			varValue = DirectX::XMFLOAT3(tValX, tValY, tValZ);
		}
		else if (varType == _CB_FLOAT4)
		{
			pugi::xml_node tXNode = parNode->child("x");
			float tValX = std::stof(tXNode.text().get());

			pugi::xml_node tYNode = parNode->child("y");
			float tValY = std::stof(tYNode.text().get());

			pugi::xml_node tZNode = parNode->child("z");
			float tValZ = std::stof(tZNode.text().get());

			pugi::xml_node tWNode = parNode->child("w");
			float tValW = std::stof(tWNode.text().get());

			varValue = DirectX::XMFLOAT4(tValX, tValY, tValZ, tValW);
		}
		else
		{
			assert(false && "Shouldn't Reach");
		}
	}

	eTexVarType MaterialParser::GetTexVarType(const std::string& varString)
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

	eTexReturnVarType MaterialParser::GetTexReturnVarType(const std::string& varString)
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

}