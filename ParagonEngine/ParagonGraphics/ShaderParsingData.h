#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdint.h>
#include <DirectXMath.h>

namespace Pg::Graphics
{
	//�ε����� �ٲٸ� �ȵȴ�. std::variant�� get ������ ��ġ.
	enum eCbVarType
	{
		_CB_BOOL = 0,
		_CB_UINT = 1,
		_CB_INT = 2,
		_CB_FLOAT = 3,
		_CB_FLOAT2 = 4,
		_CB_FLOAT3 = 5,
		_CB_FLOAT4 = 6
	};

	enum eTexVarType
	{
		_TEX_TEXTURE1D = 0,
		_TEX_TEXTURE2D = 1,
		_TEX_TEXTURE2DARRAY = 2,
		_TEX_TEXTURECUBE = 3
	};

	enum eTexReturnVarType : unsigned int
	{
		//PgShaderParser�� eResourceReturnType�� ����.
		_TEXRET_UNORM = 1,
		_TEXRET_SNORM = 2,
		_TEXRET_SINT = 3,
		_TEXRET_UINT = 4,
		_TEXRET_FLOAT = 5
	};

	using CbVarValue = std::variant<bool, uint32_t, int32_t, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4>;

	//��� Variable ���¿� ȣȯ�Ǿ�� �Ѵ�.
	struct CbVariableParsingData
	{
		CbVariableParsingData() = default;
		eCbVarType _varType;
		unsigned int _size;
		unsigned int _startOffset;
		CbVarValue _varValue;
	};

	struct ConstantBufferParsingData
	{
		std::string _name; // ��������� �̸�.
		unsigned int _byteCount; // ��������� ����Ʈ ������.
		unsigned int _registerCount; // ��������� �������� ����.
		std::vector<std::pair<std::string, CbVariableParsingData>> _varData;
	};

	struct TexVariableParsingData
	{
		TexVariableParsingData() = default;
		eTexVarType _textureType;
		eTexReturnVarType _srvReturnType;
		unsigned int _registerCount;
		std::string _fileName; //����� ���� �̸�.
	};

	struct TexturesParsingData
	{
		//Variable Name / Variable Value (�ε��� ����)
		std::vector<std::pair<std::string, TexVariableParsingData>> _varData;
	};

	struct ShaderParsingData
	{
		std::string _shaderName; //���̴��� �̸�.
		ConstantBufferParsingData _cbData; //��� ���ۿ� ���õ� ������
		TexturesParsingData _texData;
	};
}

