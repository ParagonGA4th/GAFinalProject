#include "MaterialParser.h"
#include "RenderTexture.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "DX11Headers.h"
#include "LowDX11Storage.h"
#include "Asset3DModelData.h"
#include "AssetBasic2DLoader.h"
#include "MaterialCluster.h"
#include "RenderTexture2D.h"
#include "RenderTexture2DArray.h"
#include "MaterialCluster.h"

#include "../ParagonData/ParagonDefines.h"
#include <algorithm>
#include <cassert>
#include <filesystem>

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

	void MaterialParser::LoadCustomRenderMaterial(const std::string& pgmatPath, RenderMaterial* renderMat)
	{
		ParsePgMat(pgmatPath, renderMat);
		PlaceShaders(renderMat);
		LoadRenderMaterial(renderMat);
		ClearPreviousShaderData();
	}

	void MaterialParser::LoadDefaultRenderMaterialInstance(const std::string& defInstMatName, RenderMaterial* renderMat)
	{
		//����Ʈ Material�� �ε��Ѵ�. 

		LoadDefaultRenderMaterial(renderMat, defInstMatName); // �� ����, Alpha Blending ���� Ȯ�� ����.
		PlaceDefaultShaders(renderMat);
		ClearPreviousShaderData();
		//intrinsic->_cbBufferSize = 0;
	}

	void MaterialParser::RemapMaterialIdForUninitMaterials()
	{
		//�ٽ� �ο��� �ؾ� �ϴ� ���� ����.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//�� �̻� RenderMat���� �������� �ʴ´�.
		//����, �ʿ��� ��ŭ�� ���� + Material �߰��ɼ��� ++�� ����ϱ� ����.
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);

		for (auto& it : tMatVec)
		{
			//������ ���Ҵ� ����.
			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
			if (tRenderMat->_initState == RenderMaterial::eInitState::_NONE)
			{
				if (!(GraphicsResourceHelper::IsMaterialDefaultMaterial(tRenderMat)))
				{
					//Custom Material�� ���� �ش�, ���� �ʱ�ȭ �� �Ǿ��� ��.
					tRenderMat->GetMaterialID() = _matIdRecord;
					tRenderMat->_initState = RenderMaterial::eInitState::_FROM_SCENE;

					//��ġ�� ���� �ʱ� ���ؼ�.
					_matIdRecord++;
				}
				else
				{
					//Default ID. (1)
					tRenderMat->GetMaterialID() = 1;
					tRenderMat->_initState = RenderMaterial::eInitState::_FROM_SCENE; // �� �̻� INIT���� �ʰ�.
				}
			}
		}

		assert("");
		//�� ����������, MatIdRecord�� ��ġ�� �ʴ´�.
		//Appended�� ����� �� ��.
	}

	void MaterialParser::RemapAppendedMatID()
	{
		//������� ����.
		assert(false && "������� ����, ���� ���忡����");
		//using Pg::Graphics::Manager::GraphicsResourceManager;
		//using Pg::Graphics::Helper::GraphicsResourceHelper;
		//
		//auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//
		//for (auto& it : tMatVec)
		//{
		//	//������ ���Ҵ� ����.
		//	RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
		//	bool isDefaultMat = GraphicsResourceHelper::IsMaterialDefaultMaterial(tRenderMat);
		//	
		//
		//	//NONE�϶��� ���ο� ID�� �ο�.
		//	if (tRenderMat->_initState == RenderMaterial::eInitState::_NONE)
		//	{
		//		tRenderMat->GetMaterialID() = _matIdRecord;
		//		tRenderMat->_initState = RenderMaterial::eInitState::_ADDED_LATER;
		//		//��ġ�� ���� �ʱ� ���ؼ�.
		//		_matIdRecord++;
		//	}
		//}
	}

	void MaterialParser::ResetAllKnownInitStates()
	{
		//���̻� �����ϴ� ����� ������ ������ �ȵ�. Default ����, ������ ��� �߰��� ���̱� ����.
		//ó������ �����ϴ� ��Ŀ������ �ƴ϶�, ���� Init �� �Ǿ����� +�ϴ� ������� �ǰ� �� ��.
		//auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//
		//for (auto& it : tMatVec)
		//{
		//	RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
		//	tRenderMat->_initState = RenderMaterial::eInitState::_NONE;
		//}
	}

	//��ǥ���� ���� : "test4.pgmat"
	void MaterialParser::ParsePgMat(const std::string& pgmatPath, RenderMaterial* renderMat)
	{
		//Material Parsing Data ��ü�� �����. 
		_vsParseData = std::make_unique<ShaderParsingData>();
		_psParseData = std::make_unique<ShaderParsingData>();

		pugi::xml_document doc;
		doc.load_file(pgmatPath.c_str());

		//.pgmat ���� ������ �о���̱�.
		pugi::xml_node tRootNode = doc.child("PgMaterial");

		//IsUseAlphaBlending Bool -> �׳� ���������� ���.
		pugi::xml_node tAlphaNode = tRootNode.child("IsUseAlphaBlending");
		int tVal = std::stoi(tAlphaNode.text().get());
		renderMat->_isUseAlphaBlending = static_cast<bool>(tVal);

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
		//ShaderIntrinsics �ε�.
		//VS
		LoadShaderIntrinsics(renderMat->_vsIntrinsics.get(), _vsParseData.get());
		//PS
		LoadShaderIntrinsics(renderMat->_psIntrinsics.get(), _psParseData.get());

		//Constant Buffer �����.
		//VS
		CreateConstantBuffer(renderMat->_vsIntrinsics.get());
		//PS
		CreateConstantBuffer(renderMat->_psIntrinsics.get());

		//MaterialID �ο� -> ����Ʈ ���͸����� ���, Scene�� �Ľ��ؼ� �ε��ϴµ�, ������ �ִ� ����Ʈ�� ������� �ε��ϱ� ���ؼ�.
		//GiveMaterialID(renderMat);
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
		//Shader : FileName �Ľ��ϱ�.
		{
			pugi::xml_node tVSFileNameNode = shdNode->child("FileName");
			parsingData->_shaderName = tVSFileNameNode.text().get();
		}

		//Shader : Constant Buffer �Ľ��ϱ�.
		{
			pugi::xml_node tCbNode = shdNode->child("ConstantBuffer");
			{
				pugi::xml_node tCbNameNode = tCbNode.child("BufferName");
				parsingData->_cbData._name = tCbNameNode.text().get();
			}
			{
				pugi::xml_node tCbByteSizeNode = tCbNode.child("ByteCount");
				//UINT�� �ٲپ� �ޱ�.
				parsingData->_cbData._byteCount = std::stoul(tCbByteSizeNode.text().get());
			}
			{
				pugi::xml_node tCbRegCountNode = tCbNode.child("RegisterCount");
				//UINT�� �ٲپ� �ޱ�.
				parsingData->_cbData._registerCount = std::stoul(tCbRegCountNode.text().get());
			}
			{
				pugi::xml_node tCbVarOrderNode = tCbNode.child("VariableOrder");

				//VarOrder�� �ڽ� ��� Node Iteration => �Ŀ� �����ϴµ� Ȱ��� ��. �ε��� ������ �߿�!
				for (pugi::xml_node tVarOrderChildNode = tCbVarOrderNode.first_child();
					tVarOrderChildNode; tVarOrderChildNode = tVarOrderChildNode.next_sibling())
				{
					parsingData->_cbData._varData.push_back(std::make_pair(tVarOrderChildNode.text().get(), CbVariableParsingData()));
				}
			}
			{
				pugi::xml_node tCbVarNode = tCbNode.child("Variables");

				//��� Child Iteration.
				for (pugi::xml_node tVarChildNode = tCbVarNode.first_child();
					tVarChildNode; tVarChildNode = tVarChildNode.next_sibling())
				{
					//Attribute ����.
					std::string tVarName = tVarChildNode.attribute("Name").value();

					//VariableOrder�� �����ϴ� ��Ҹ� ã��.
					auto it = std::find_if(parsingData->_cbData._varData.begin(), parsingData->_cbData._varData.end(),
						[&tVarName](const std::pair<std::string, CbVariableParsingData>& element) { return element.first == tVarName; });

					if (it == parsingData->_cbData._varData.end())
					{
						assert(false && ".pgmat�� ���ʿ� �߰ߵ��� ���ϴ� ���� ������ �ȵ�.");
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

				//VarOrder�� �ڽ� ��� Node Iteration => �Ŀ� �����ϴµ� Ȱ��� ��. �ε��� ������ �߿�!
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
					//Attribute ����.
					std::string tVarName = tVarChildNode.attribute("Name").value();

					//VariableOrder�� �����ϴ� ��Ҹ� ã��.
					auto it = std::find_if(parsingData->_texData._varData.begin(), parsingData->_texData._varData.end(),
						[&tVarName](const std::pair<std::string, TexVariableParsingData>& element) { return element.first == tVarName; });

					if (it == parsingData->_texData._varData.end())
					{
						assert(false && ".pgmat�� ���ʿ� �߰ߵ��� ���ϴ� ���� ������ �ȵ�.");
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
		///240116 ����Ʈ���� ���� ����.

		//<ConstantBuffer>
		//ByteCount��ŭ Constant Buffer ���� ���´�. + ���.
		intrinsic->_cbBufferSize = parseData->_cbData._byteCount;
		intrinsic->_cbByteUpdateBuffer->resize(intrinsic->_cbBufferSize);

		//Register Number ���.
		intrinsic->_cbRegisterNum = parseData->_cbData._registerCount;

		//����Ʈ ���ۿ� �� �� ����.
		for (int i = 0; i < parseData->_cbData._varData.size(); i++)
		{
			intrinsic->_cbByteVector.push_back(CbMaterialPair());
			CbMaterialPair& tAddedCbMatPair = intrinsic->_cbByteVector.back();

			//Variable Name �ű��.
			tAddedCbMatPair.first = parseData->_cbData._varData[i].first;
			//Structured Binding.
			auto& [bVarType, bByteOffset] = tAddedCbMatPair.second;
			//VarType ��������.
			bVarType = parseData->_cbData._varData[i].second._varType;
			//ByteOffset ��������.
			bByteOffset = parseData->_cbData._varData[i].second._startOffset;

			//������ ByteBuffer�� �� �ֱ�. ByteOffset = Index 1��1 ���� �����ϴ�.
			CbVarValue& tVarParsedValue = parseData->_cbData._varData[i].second._varValue;
			switch (bVarType)
			{
				case (_CB_BOOL):
				{
					//HLSL�� Bool = 4Byte. == C�� BOOL Typedef.
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

			//���� ������ �ִ� ���: �׷��Ƚ� ���ҽ� �Ŵ������� ã�ƾ� �Ѵ�!
			auto it = GraphicsResourceManager::Instance()->GetResourceByName(
				parseData->_texData._varData[i].second._fileName, GraphicsResourceHelper::GetAssetDefine(bTexType));
			bRenderTexture = static_cast<RenderTexture*>(it.get());
		}
		//</Textures>
	}

	void MaterialParser::PlaceShaders(RenderMaterial* renderMat)
	{
		//���� ������ �ִ� ���: �׷��Ƚ� ���ҽ� �Ŵ������� ã�ƾ� �Ѵ�!
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
		//Constant Buffer�� ��������� �ʾҴٸ�, ���� ��ü�� ������ �ʴ´�.
		if (intrinsic->_cbBufferSize == 0)
		{
			//Constant Buffer�� ���ٴ� ��.
			return;
		}

		int sizeCB = (((intrinsic->_cbBufferSize - 1) / 16) + 1) * 16;	// declspec ���� 16����Ʈ ������ �� �ִ�?
		assert(sizeCB % 16 == 0);
		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // ������۴� 16����Ʈ ����
		tCBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		tCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		tCBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tCBufferDesc.MiscFlags = 0;

		intrinsic->_cbSubResData->SysMemPitch = 0;
		intrinsic->_cbSubResData->SysMemSlicePitch = 0;
		intrinsic->_cbSubResData->pSysMem = intrinsic->_cbByteUpdateBuffer->GetStartAddress();

		HR(LowDX11Storage::GetInstance()->_device->CreateBuffer(&tCBufferDesc, intrinsic->_cbSubResData.get(), &(intrinsic->_cBuffer)));
	}

	void MaterialParser::GiveMaterialID(RenderMaterial* renderMat)
	{
		//Material ID�� �ο�. -> Default Material�� ��Ÿ�� �ε带 ����, �߰� �Ҵ�.
		renderMat->_materialID = _matIdRecord;
		//"Handle" ��, +1. ��ġ�� ���� �ʱ� ����.
		_matIdRecord++;
	}

	void MaterialParser::PlaceDefaultShaders(RenderMaterial* renderMat)
	{
		//���� ������ �ִ� ���: �׷��Ƚ� ���ҽ� �Ŵ������� ã�ƾ� �Ѵ�!

		//�� ���ǹ��� RenderMaterial�� Deferred�� ��쿡�� �Ϲ� Material,
		//AlphaBlending�� ��쿡�� Forward�� ���´ٴ� ��Ȳ�� ���� ���ǹ�.
		//���ӿ����� ����Ʈ ���͸��� ȣȯ����.
		if (renderMat->GetIsUseAlphaBlending())
		{
			if (renderMat->_whenAlphaIsSkinned)
			{
				//Skinned Alpha
				//VS
				auto vsRes = GraphicsResourceManager::Instance()->GetResourceByName(
					Pg::Defines::DEFAULT_ALPHA_SKINNED_VS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
				renderMat->_vertexShader = static_cast<RenderVertexShader*>(vsRes.get());
				//PS
				auto psRes = GraphicsResourceManager::Instance()->GetResourceByName(
					Pg::Defines::DEFAULT_ALPHA_PS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER);
				renderMat->_pixelShader = static_cast<RenderPixelShader*>(psRes.get());
			}
			else
			{
				//Static Alpha
				//VS
				auto vsRes = GraphicsResourceManager::Instance()->GetResourceByName(
					Pg::Defines::DEFAULT_ALPHA_STATIC_VS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
				renderMat->_vertexShader = static_cast<RenderVertexShader*>(vsRes.get());
				//PS
				auto psRes = GraphicsResourceManager::Instance()->GetResourceByName(
					Pg::Defines::DEFAULT_ALPHA_PS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER);
				renderMat->_pixelShader = static_cast<RenderPixelShader*>(psRes.get());
			}
		}
		else
		{
			//Deferred.
			//VS
			auto vsRes = GraphicsResourceManager::Instance()->GetResourceByName(
				Pg::Defines::DEFAULT_APPENDS_RENDER_VS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER);
			renderMat->_vertexShader = static_cast<RenderVertexShader*>(vsRes.get());
			//PS
			auto psRes = GraphicsResourceManager::Instance()->GetResourceByName(
				Pg::Defines::DEFAULT_APPENDS_RENDER_PS_NAME, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER);
			renderMat->_pixelShader = static_cast<RenderPixelShader*>(psRes.get());
		}
	}

	void MaterialParser::LoadDefaultRenderMaterial(RenderMaterial* renderMat, const std::string& defInstMatName)
	{
		//�ƿ� ConstantBuffer�� ��� ���̵� Ȱ������ �ʴ´�.
		renderMat->_vsIntrinsics->_cbBufferSize = 0;
		renderMat->_psIntrinsics->_cbBufferSize = 0;

		std::string tMeshName = GraphicsResourceHelper::GetMeshNameFromDefaultMaterialName(defInstMatName);

		auto tRes = GraphicsResourceManager::Instance()->GetResourceByName(tMeshName, Pg::Data::Enums::eAssetDefine::_3DMODEL);
		Asset3DModelData* tModelData = static_cast<Asset3DModelData*>(tRes.get());
		assert(tModelData != nullptr);

		//���� Resource Binding�� ������� �� ��, AlphaBlending �� üũ�ؼ� �ִ� �뵵�� Function�� ���δ�.
		renderMat->_isUseAlphaBlending = tModelData->_isUseAlphaBlending;

		if (renderMat->_isUseAlphaBlending)
		{
			//IsSkinned, Alpha�� ��쿡 ���.
			renderMat->_whenAlphaIsSkinned = tModelData->IsSkinned();
		}

		//PBR �⺻���� �ٲ�� �� ��, ����Ʈ ���͸����� SRV�� �ϴ� ���� ���� �ʿ䰡 �����.
		//PS Intrinsics : Diffuse �� �ֱ�.
		//PlaceDefaultMaterialTextureArrayBuffer(defInstMatName, renderMat->_psIntrinsics.get(), tModelData, PG_TextureType_DIFFUSE, "t2_DiffuseTextureArray", 25);
		//
		////PS Intrinsics : Normal �� �ֱ�.
		//PlaceDefaultMaterialTextureArrayBuffer(defInstMatName, renderMat->_psIntrinsics.get(), tModelData, PG_TextureType_NORMALS, "t2_NormalTextureArray", 26);

		//������ MaterialID �ο� -> ������ ���⼭ ���� �ʴ´�. Scene�� ���ε�Ǿ��� �� �ϰ������� Material ID�� ��ο��Ǳ� ����.
		//GiveMaterialID(renderMat);
	}

	void MaterialParser::PlaceDefaultMaterialTextureArrayBuffer(const std::string& defInstMatName, RenderMaterial::MatShaderIntrinsics* intrinsic,
		Asset3DModelData* asset3dModel, eAssetTextureType type,
		const std::string& varName, unsigned int registerNum)
	{
		//���� ������ �ִ� ���: �׷��Ƚ� ���ҽ� �Ŵ������� ã�ƾ� �Ѵ�!
		//Material Cluster�κ��� ���� ã�ƿͼ�, Texture���� �־�� �Ѵ�.

		std::vector<std::string> tRenderT2Vec;
		tRenderT2Vec.resize(asset3dModel->GetMaterialCount());

		//�̸��� �и�.
		for (short i = 0; i < asset3dModel->GetMaterialCount(); i++)
		{
			MaterialCluster* tMatCluster = asset3dModel->GetMaterialByIndex(i);
			std::string tPath = "";
			if (tMatCluster->GetTextureByType(type) != nullptr)
			{
				//������ ���� ���� ���, ���� �ε��ؼ� �ִ´�.
				tPath = tMatCluster->GetTextureByType(type)->GetFilePath();
				std::filesystem::path tFSP = tPath;
				tRenderT2Vec.at(i) = tFSP.filename().string();
			}
			else
			{
				//���� ���, Ÿ�Կ� �´� �⺻ ���ҽ��� �ִ´�. �� ���, Default Textures�� �ε�� ��.
				std::filesystem::path tFSP = GraphicsResourceHelper::GetDefaultTexturePath(type);
				tRenderT2Vec.at(i) = tFSP.filename().string();
			}
		}
		//������ ��� �� �ش� MaterialCluster ������ Texture2DArray�� ũ�� �������� ������ ����.
		//���ҽ� �Ŵ����� �̹� ���� �ֱ� ����. Material�� �����ǰ�.
		//�׷��� �����Ѵ�.

		//���������� Default Material Texture2DArray ǥ�ø� �Ѵ�.

		std::string tTempTex2DArrName = GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(defInstMatName, varName, tRenderT2Vec.data(), tRenderT2Vec.size());

		//$DefaultMaterial_Texture2DArray_$�� ��� �ֱ⿡, Default Material ���� Texture2DArray�� �ε�� ��.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

		//��ü �����Ͽ� ���� �ִٰ� ���. (Graphics���� �˻��߱� ������, AssetManager�� �������)
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

		//���� �ش� Texture2DArray�� ���������. �ش� ���� ������ �����ͼ�, Material �ε忡 �°� �ٲپ���� �Ѵ�.
		intrinsic->_texPlaceVector.push_back(TexMaterialPair());
		TexMaterialPair& tAddedTexMatPair = intrinsic->_texPlaceVector.back();
		tAddedTexMatPair.first = varName;
		auto& [bTexType, bRegisterNum, bRenderTexture] = tAddedTexMatPair.second;
		bTexType = eTexVarType::_TEX_TEXTURE2DARRAY;
		bRegisterNum = registerNum;

		//����������, �ε��ߴ� RenderTexture2DArray���� ����.
		auto tTex2DRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
		bRenderTexture = static_cast<RenderTexture*>(tTex2DRes.get());

		return;
	}





}