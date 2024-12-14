#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdint.h>
#include <DirectXMath.h>

namespace Pg::Graphics
{
	//인덱스를 바꾸면 안된다. std::variant의 get 순서와 일치.
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
		//PgShaderParser의 eResourceReturnType와 대응.
		_TEXRET_UNORM = 1,
		_TEXRET_SNORM = 2,
		_TEXRET_SINT = 3,
		_TEXRET_UINT = 4,
		_TEXRET_FLOAT = 5
	};

	using CbVarValue = std::variant<bool, uint32_t, int32_t, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4>;

	//모든 Variable 형태에 호환되어야 한다.
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
		std::string _name; // 상수버퍼의 이름.
		unsigned int _byteCount; // 상수버퍼의 바이트 사이즈.
		unsigned int _registerCount; // 상수버퍼의 레지스터 숫자.
		std::vector<std::pair<std::string, CbVariableParsingData>> _varData;
	};

	struct TexVariableParsingData
	{
		TexVariableParsingData() = default;
		eTexVarType _textureType;
		eTexReturnVarType _srvReturnType;
		unsigned int _registerCount;
		std::string _fileName; //연결될 파일 이름.
	};

	struct TexturesParsingData
	{
		//Variable Name / Variable Value (인덱스 유지)
		std::vector<std::pair<std::string, TexVariableParsingData>> _varData;
	};

	struct ShaderParsingData
	{
		std::string _shaderName; //셰이더의 이름.
		ConstantBufferParsingData _cbData; //상수 버퍼에 관련된 데이터
		TexturesParsingData _texData;
	};
}

