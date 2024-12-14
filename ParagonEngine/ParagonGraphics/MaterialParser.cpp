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
		//디폴트 Material을 로드한다. 

		LoadDefaultRenderMaterial(renderMat, defInstMatName); // 더 먼저, Alpha Blending 여부 확인 위해.
		PlaceDefaultShaders(renderMat);
		ClearPreviousShaderData();
		//intrinsic->_cbBufferSize = 0;
	}

	void MaterialParser::RemapMaterialIdForUninitMaterials()
	{
		//다시 부여를 해야 하니 값을 리셋.
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;

		//더 이상 RenderMat들을 리셋하지 않는다.
		//그저, 필요한 만큼만 렌더 + Material 추가될수록 ++로 등록하기 위해.
		auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);

		for (auto& it : tMatVec)
		{
			//참조라 재할당 가능.
			RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
			if (tRenderMat->_initState == RenderMaterial::eInitState::_NONE)
			{
				if (!(GraphicsResourceHelper::IsMaterialDefaultMaterial(tRenderMat)))
				{
					//Custom Material일 때만 해당, 아직 초기화 안 되었을 때.
					tRenderMat->GetMaterialID() = _matIdRecord;
					tRenderMat->_initState = RenderMaterial::eInitState::_FROM_SCENE;

					//겹치게 하지 않기 위해서.
					_matIdRecord++;
				}
				else
				{
					//Default ID. (1)
					tRenderMat->GetMaterialID() = 1;
					tRenderMat->_initState = RenderMaterial::eInitState::_FROM_SCENE; // 더 이상 INIT되지 않게.
				}
			}
		}

		assert("");
		//이 시점에서는, MatIdRecord가 겹치지 않는다.
		//Appended될 기반이 될 것.
	}

	void MaterialParser::RemapAppendedMatID()
	{
		//사용하지 않음.
		assert(false && "사용하지 않음, 게임 빌드에서는");
		//using Pg::Graphics::Manager::GraphicsResourceManager;
		//using Pg::Graphics::Helper::GraphicsResourceHelper;
		//
		//auto tMatVec = GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//
		//for (auto& it : tMatVec)
		//{
		//	//참조라 재할당 가능.
		//	RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
		//	bool isDefaultMat = GraphicsResourceHelper::IsMaterialDefaultMaterial(tRenderMat);
		//	
		//
		//	//NONE일때만 새로운 ID를 부여.
		//	if (tRenderMat->_initState == RenderMaterial::eInitState::_NONE)
		//	{
		//		tRenderMat->GetMaterialID() = _matIdRecord;
		//		tRenderMat->_initState = RenderMaterial::eInitState::_ADDED_LATER;
		//		//겹치게 하지 않기 위해서.
		//		_matIdRecord++;
		//	}
		//}
	}

	void MaterialParser::ResetAllKnownInitStates()
	{
		//더이상 리셋하는 기능을 가지고 있으면 안됨. Default 뺴고, 앞으로 계속 추가될 것이기 때문.
		//처음부터 리셋하는 매커니즘이 아니라, 아직 Init 안 되었으면 +하는 방식으로 되게 될 것.
		//auto tMatVec = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetAllResourcesByDefine(Data::Enums::eAssetDefine::_RENDERMATERIAL);
		//
		//for (auto& it : tMatVec)
		//{
		//	RenderMaterial* tRenderMat = static_cast<RenderMaterial*>(it.get());
		//	tRenderMat->_initState = RenderMaterial::eInitState::_NONE;
		//}
	}

	//대표적인 예시 : "test4.pgmat"
	void MaterialParser::ParsePgMat(const std::string& pgmatPath, RenderMaterial* renderMat)
	{
		//Material Parsing Data 객체를 만들기. 
		_vsParseData = std::make_unique<ShaderParsingData>();
		_psParseData = std::make_unique<ShaderParsingData>();

		pugi::xml_document doc;
		doc.load_file(pgmatPath.c_str());

		//.pgmat 파일 형식을 읽어들이기.
		pugi::xml_node tRootNode = doc.child("PgMaterial");

		//IsUseAlphaBlending Bool -> 그냥 개별적으로 기록.
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

		//MaterialID 부여 -> 디폴트 매터리얼의 경우, Scene을 파싱해서 로드하는데, 기존에 있는 리스트와 상관없이 로드하기 위해서.
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
		///240116 바이트버퍼 문제 여기.

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
		//Constant Buffer가 만들어지지 않았다면, 버퍼 자체를 만들지 않는다.
		if (intrinsic->_cbBufferSize == 0)
		{
			//Constant Buffer가 없다는 말.
			return;
		}

		int sizeCB = (((intrinsic->_cbBufferSize - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);
		D3D11_BUFFER_DESC tCBufferDesc;
		tCBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
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
		//Material ID를 부여. -> Default Material의 런타임 로드를 위해, 추가 할당.
		renderMat->_materialID = _matIdRecord;
		//"Handle" 값, +1. 겹치게 하지 않기 위해.
		_matIdRecord++;
	}

	void MaterialParser::PlaceDefaultShaders(RenderMaterial* renderMat)
	{
		//실제 파일을 넣는 방식: 그래픽스 리소스 매니저에서 찾아야 한다!

		//이 조건문은 RenderMaterial이 Deferred의 경우에는 일반 Material,
		//AlphaBlending의 경우에는 Forward로 나온다는 상황에 맞춘 조건문.
		//게임에서는 디폴트 매터리얼만 호환하자.
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
		//아예 ConstantBuffer를 어느 쪽이든 활용하지 않는다.
		renderMat->_vsIntrinsics->_cbBufferSize = 0;
		renderMat->_psIntrinsics->_cbBufferSize = 0;

		std::string tMeshName = GraphicsResourceHelper::GetMeshNameFromDefaultMaterialName(defInstMatName);

		auto tRes = GraphicsResourceManager::Instance()->GetResourceByName(tMeshName, Pg::Data::Enums::eAssetDefine::_3DMODEL);
		Asset3DModelData* tModelData = static_cast<Asset3DModelData*>(tRes.get());
		assert(tModelData != nullptr);

		//기존 Resource Binding이 사라지고 난 뒤, AlphaBlending 값 체크해서 넣는 용도로 Function이 쓰인다.
		renderMat->_isUseAlphaBlending = tModelData->_isUseAlphaBlending;

		if (renderMat->_isUseAlphaBlending)
		{
			//IsSkinned, Alpha인 경우에 기록.
			renderMat->_whenAlphaIsSkinned = tModelData->IsSkinned();
		}

		//PBR 기본으로 바뀌고 난 뒤, 디폴트 매터리얼의 SRV에 일단 뭐를 넣을 필요가 사라짐.
		//PS Intrinsics : Diffuse 값 넣기.
		//PlaceDefaultMaterialTextureArrayBuffer(defInstMatName, renderMat->_psIntrinsics.get(), tModelData, PG_TextureType_DIFFUSE, "t2_DiffuseTextureArray", 25);
		//
		////PS Intrinsics : Normal 값 넣기.
		//PlaceDefaultMaterialTextureArrayBuffer(defInstMatName, renderMat->_psIntrinsics.get(), tModelData, PG_TextureType_NORMALS, "t2_NormalTextureArray", 26);

		//고유한 MaterialID 부여 -> 이제는 여기서 하지 않는다. Scene이 리로드되었을 때 일괄적으로 Material ID가 재부여되기 때문.
		//GiveMaterialID(renderMat);
	}

	void MaterialParser::PlaceDefaultMaterialTextureArrayBuffer(const std::string& defInstMatName, RenderMaterial::MatShaderIntrinsics* intrinsic,
		Asset3DModelData* asset3dModel, eAssetTextureType type,
		const std::string& varName, unsigned int registerNum)
	{
		//실제 파일을 넣는 방식: 그래픽스 리소스 매니저에서 찾아야 한다!
		//Material Cluster로부터 값을 찾아와서, Texture값을 넣어야 한다.

		std::vector<std::string> tRenderT2Vec;
		tRenderT2Vec.resize(asset3dModel->GetMaterialCount());

		//이름만 분리.
		for (short i = 0; i < asset3dModel->GetMaterialCount(); i++)
		{
			MaterialCluster* tMatCluster = asset3dModel->GetMaterialByIndex(i);
			std::string tPath = "";
			if (tMatCluster->GetTextureByType(type) != nullptr)
			{
				//실제로 값이 있을 경우, 값을 로딩해서 넣는다.
				tPath = tMatCluster->GetTextureByType(type)->GetFilePath();
				std::filesystem::path tFSP = tPath;
				tRenderT2Vec.at(i) = tFSP.filename().string();
			}
			else
			{
				//없을 경우, 타입에 맞는 기본 리소스를 넣는다. 이 경우, Default Textures가 로드될 것.
				std::filesystem::path tFSP = GraphicsResourceHelper::GetDefaultTexturePath(type);
				tRenderT2Vec.at(i) = tFSP.filename().string();
			}
		}
		//어차피 모든 이 해당 MaterialCluster 내부의 Texture2DArray는 크게 관리받을 이유가 없다.
		//리소스 매니저에 이미 값이 있기 때문. Material이 관리되고.
		//그래도 보관한다.

		//내부적으로 Default Material Texture2DArray 표시를 한다.

		std::string tTempTex2DArrName = GraphicsResourceHelper::GetGeneratedTex2DArrayNameFromValues(defInstMatName, varName, tRenderT2Vec.data(), tRenderT2Vec.size());

		//$DefaultMaterial_Texture2DArray_$이 들어 있기에, Default Material 전용 Texture2DArray로 로드될 것.
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->LoadResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

		//전체 저장목록에 갖고 있다고 기록. (Graphics에서 검사했기 때문에, AssetManager로 보내줘야)
		Pg::Graphics::Manager::GraphicsResourceManager::Instance()->AddSecondaryResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);

		//이제 해당 Texture2DArray가 만들어졌다. 해당 값을 이제는 가져와서, Material 로드에 맞게 바꾸어줘야 한다.
		intrinsic->_texPlaceVector.push_back(TexMaterialPair());
		TexMaterialPair& tAddedTexMatPair = intrinsic->_texPlaceVector.back();
		tAddedTexMatPair.first = varName;
		auto& [bTexType, bRegisterNum, bRenderTexture] = tAddedTexMatPair.second;
		bTexType = eTexVarType::_TEX_TEXTURE2DARRAY;
		bRegisterNum = registerNum;

		//마지막으로, 로드했던 RenderTexture2DArray까지 투입.
		auto tTex2DRes = Pg::Graphics::Manager::GraphicsResourceManager::Instance()->GetResource(tTempTex2DArrName, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
		bRenderTexture = static_cast<RenderTexture*>(tTex2DRes.get());

		return;
	}





}