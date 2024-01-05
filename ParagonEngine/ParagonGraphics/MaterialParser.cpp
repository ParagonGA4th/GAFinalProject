#include "MaterialParser.h"
#include "RenderTexture.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "DX11Headers.h"
#include "LowDX11Storage.h"
#include <algorithm>
#include <cassert>

namespace Pg::Graphics
{
	using Pg::Graphics::Manager::GraphicsResourceManager;
	using Pg::Graphics::Helper::GraphicsResourceHelper;

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
		//ShaderIntrinsics 로딩.
		//VS
		LoadShaderIntrinsics(renderMat->_vsIntrinsics.get(), _vsParseData.get());
		//PS
		LoadShaderIntrinsics(renderMat->_psIntrinsics.get(), _psParseData.get());

		//Constant Buffer 만들기.
		//VS
		CreateConstantBuffer(renderMat->_vsIntrinsics.get());
		//PS
		CreateConstantBuffer(renderMat->_psIntrinsics.get());

		//고유한 MaterialID 부여
		GiveMaterialID(renderMat);
	}

	void MaterialParser::ClearPreviousShaderData()
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
					it->second._varType = GraphicsResourceHelper::GetCbVarType(tVarChildNode.attribute("Type").value());
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

					it->second._textureType = GraphicsResourceHelper::GetTexVarType(tVarChildNode.attribute("Type").value());
					it->second._srvReturnType = GraphicsResourceHelper::GetTexReturnVarType(tVarChildNode.attribute("ResourceReturnType").value());
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

	void MaterialParser::LoadShaderIntrinsics(RenderMaterial::MatShaderIntrinsics* intrinsic, ShaderParsingData* parseData)
	{
		//<ConstantBuffer>
		//ByteCount만큼 Constant Buffer 값을 놓는다. + 기록.
		intrinsic->_cbBufferSize = parseData->_cbData._byteCount;
		intrinsic->_cbByteUpdateBuffer->resize(intrinsic->_cbBufferSize);

		//Register Number 기록.
		intrinsic->_cbRegisterNum = parseData->_cbData._registerCount;

		//바이트 버퍼에 값 등 나열.
		for (int i = 0; i < parseData->_cbData._varData.size(); i++)
		{
			intrinsic->_cbByteVector.push_back(CbMaterialPair());
			CbMaterialPair& tAddedCbMatPair = intrinsic->_cbByteVector.back();

			//Variable Name 옮기기.
			tAddedCbMatPair.first = parseData->_cbData._varData[i].first;
			//Structured Binding.
			auto& [bVarType, bByteOffset] = tAddedCbMatPair.second;
			//VarType 가져오기.
			bVarType = parseData->_cbData._varData[i].second._varType;
			//ByteOffset 가져오기.
			bByteOffset = parseData->_cbData._varData[i].second._startOffset;

			//실제로 ByteBuffer에 값 넣기. ByteOffset = Index 1대1 대응 가능하다.
			CbVarValue& tVarParsedValue = parseData->_cbData._varData[i].second._varValue;
			switch (bVarType)
			{
				case (_CB_BOOL):
				{
					//HLSL식 Bool = 4Byte. == C식 BOOL Typedef.
					intrinsic->_cbByteUpdateBuffer->putInt(static_cast<int>(std::get<_CB_BOOL>(tVarParsedValue)), bByteOffset);
				}
				break;
				case (_CB_UINT):
				{
					intrinsic->_cbByteUpdateBuffer->putUnsignedInt(std::get<_CB_UINT>(tVarParsedValue), bByteOffset);
				}
				break;
				case (_CB_INT):
				{
					intrinsic->_cbByteUpdateBuffer->putInt(std::get<_CB_INT>(tVarParsedValue), bByteOffset);
				}
				break;
				case (_CB_FLOAT):
				{
					intrinsic->_cbByteUpdateBuffer->putFloat(std::get<_CB_FLOAT>(tVarParsedValue), bByteOffset);
				}
				break;
				case (_CB_FLOAT2):
				{
					intrinsic->_cbByteUpdateBuffer->PutXMFloat2(std::get<_CB_FLOAT2>(tVarParsedValue), bByteOffset);
				}
				break;
				case (_CB_FLOAT3):
				{
					intrinsic->_cbByteUpdateBuffer->PutXMFloat3(std::get<_CB_FLOAT3>(tVarParsedValue), bByteOffset);
				}
				break;
				case (_CB_FLOAT4):
				{
					intrinsic->_cbByteUpdateBuffer->PutXMFloat4(std::get<_CB_FLOAT4>(tVarParsedValue), bByteOffset);
				}
				break;
			}
		}
		//</ConstantBuffer>

		//<Textures>
		for (int i = 0; i < parseData->_texData._varData.size(); i++)
		{
			intrinsic->_texPlaceVector.push_back(TexMaterialPair());
			TexMaterialPair& tAddedTexMatPair = intrinsic->_texPlaceVector.back();

			//Variable Name
			tAddedTexMatPair.first = parseData->_texData._varData[i].first;
			//Structured Binding
			auto& [bTexType, bRegisterNum, bRenderTexture] = tAddedTexMatPair.second;

			bTexType = parseData->_texData._varData[i].second._textureType;
			bRegisterNum = parseData->_texData._varData[i].second._registerCount;
			
			//실제 파일을 넣는 방식: 그래픽스 리소스 매니저에서 찾아야 한다!
			auto it = GraphicsResourceManager::Instance()->GetResourceByName(
				parseData->_texData._varData[i].second._fileName, GraphicsResourceHelper::GetAssetDefine(bTexType));
			bRenderTexture = static_cast<RenderTexture*>(it.get());
		}
		//</Textures>
	}

	void MaterialParser::PlaceShaders(RenderMaterial* renderMat)
	{
		//실제 파일을 넣는 방식: 그래픽스 리소스 매니저에서 찾아야 한다!
		//VS
		auto vsRes = GraphicsResourceManager::Instance()->GetResourceByName(
			_vsParseData->_shaderName, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
		renderMat->_vertexShader = static_cast<RenderVertexShader*>(vsRes.get());
		//PS
		auto psRes = GraphicsResourceManager::Instance()->GetResourceByName(
			_psParseData->_shaderName, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER);
		renderMat->_pixelShader = static_cast<RenderPixelShader*>(psRes.get());
	}


	void MaterialParser::CreateConstantBuffer(RenderMaterial::MatShaderIntrinsics* intrinsic)
	{
		int sizeCB = (((intrinsic->_cbBufferSize -1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);
		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tCBufferDesc.MiscFlags = 0;
		intrinsic->_cbSubResData->pSysMem = intrinsic->_cbByteUpdateBuffer->GetStartAddress();

		HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tCBufferDesc, intrinsic->_cbSubResData.get(), &(intrinsic->_cBuffer)));
	}

	void MaterialParser::GiveMaterialID(RenderMaterial* renderMat)
	{
		//Material ID를 부여.
		renderMat->_materialID = _matIdRecord;
		//"Handle" 값, +1. 겹치게 하지 않기 위해.
		_matIdRecord++;
	}


}