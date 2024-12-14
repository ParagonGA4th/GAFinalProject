#pragma once
#include <string>
#include <memory>
#include "ShaderParsingData.h"
#include "AssetTextureType.h"
#include "RenderMaterial.h"
#include "../ParagonHelper/pugixml.hpp"

/// <summary>
/// PgShaderParser�� ���� �������
/// .pgmat ������ ������ RenderMaterial�� �о�� �� �ְ� �ϴ� �ڵ�.
/// 
/// CombinedLoader�� ������ ��ü�� ��� ���� ���̴�.
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

		//Ŀ���� ���͸���.
		void LoadCustomRenderMaterial(const std::string& pgmatPath, RenderMaterial* renderMat);
	
		//����Ʈ ���͸���.
		void LoadDefaultRenderMaterialInstance(const std::string& defInstMatName, RenderMaterial* renderMat);

		//Scene�� �ٲ� ������ ������Ʈ, ���� Initialize���� ���� Material�� ���� ���̵� �ο�.
		void RemapMaterialIdForUninitMaterials();

		//�߰��� �߰��� �� ���� �Ϻ� ���͸��� �߰��� �ο�.
		void RemapAppendedMatID();

		//ó�� Scene�� �ε��ϱ� ���� eInitState�� _NONE���� ����.
		void ResetAllKnownInitStates();
	private:
		//ū ���̾��.
		void ParsePgMat(const std::string& pgmatPath, RenderMaterial* renderMat);
		//������ ���ҽ� �Ŵ����� �ִ� VS, PS�� ��ġ�Ѵ�.
		void PlaceShaders(RenderMaterial* renderMat);
		void LoadRenderMaterial(RenderMaterial* renderMat);
		void ClearPreviousShaderData();

		//����Ʈ ���̴����� ���� ���׸��� �ȿ� ��ġ�Ѵ�.
		void PlaceDefaultShaders(RenderMaterial* renderMat);
		void LoadDefaultRenderMaterial(RenderMaterial* renderMat, const std::string& defInstMatName);
		//����Ʈ ���͸��󿡼� �⺻������ �÷��ִ� ���ҽ����� ���� �÷��ֱ�� ��ӵǾ��� ������, ������ ����� ����ŭ �������� ���� ���̴�.
		void PlaceDefaultMaterialTextureArrayBuffer(const std::string& defInstMatName, RenderMaterial::MatShaderIntrinsics* intrinsic,
			Asset3DModelData* asset3dModel, eAssetTextureType type, const std::string& varName, unsigned int registerNum);

	private:
		//���� ���̾��.
		void ParseShaderMat(pugi::xml_node* shdNode, ShaderParsingData* parsingData);	
		void LoadShaderIntrinsics(RenderMaterial::MatShaderIntrinsics* intrinsic, ShaderParsingData* parseData);
		void CreateConstantBuffer(RenderMaterial::MatShaderIntrinsics* intrinsic);
		void GiveMaterialID(RenderMaterial* renderMat);
	private:
		void GetCbVarValue(pugi::xml_node* parNode, eCbVarType varType, CbVarValue& varValue);

	private:
		std::unique_ptr<ShaderParsingData> _vsParseData = nullptr;
		std::unique_ptr<ShaderParsingData> _psParseData = nullptr;

		//���µǸ� �ٽ� 2�� ���ư���. 
		//���ݱ��� Material���� �ο��� ID�̴�.
		//Scene�� �ٲ𶧸��� ��� Material���� ID�� �ٽ� �ο��ȴ�.
		//������ 2���� �����ϰ� �Ǿ���. => : 1�� ����Ʈ.
		unsigned int _matIdRecord = 2;
	};
}


