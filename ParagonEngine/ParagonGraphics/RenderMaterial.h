#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonUtil/ByteBuffer.hpp"
#include "ShaderParsingData.h"
#include <vector>
#include <string>
#include <tuple>
#include <DirectXMath.h>

/// <summary>
/// 실제 Material과 1대1 대응되는 클래스. 게임엔진에서 저잗된 Material과 연동되어 보관된다.
/// </summary>

namespace Pg::Graphics
{
	class RenderTexture;
	class MaterialParser;

	namespace Loader
	{
		class AssetCombinedLoader;
	}
}

namespace Pg::Graphics
{
	//내부적인 SetValue 로직을 위한 Constant Buffer 배열 -> Byte Set 값은 Enum 값에 의해 결정
	//								VariableName,		VariableType, ByteOffset
	using CbMaterialPair = std::pair<std::string, std::pair<eCbVarType, unsigned int>>;

	////							VariableName,			TextureType,	RegisterNumber, 실제 Texture.
	using TexMaterialPair = std::pair<std::string, std::tuple<eTexVarType, unsigned int, RenderTexture*>>;
}

namespace Pg::Graphics
{
	class RenderMaterial : public Pg::Data::Resources::GraphicsResource
	{
		friend class Pg::Graphics::Loader::AssetCombinedLoader;
		friend class Pg::Graphics::MaterialParser;
	public:
		RenderMaterial(Pg::Data::Enums::eAssetDefine define, const std::string& filePath);
		~RenderMaterial();

		//내부적으로 로드, 언로드해주는 함수들. 최종 자식 클래스들의 구현체의 생성/소멸자에서 호출되어야.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

	public:
		//SetXXX 함수들. Vertex Shader, Pixel Shader 전용이 다르다.
		void SetBoolVS(const std::string& varName, bool value);
		void SetBoolPS(const std::string& varName, bool value);
		void SetUnsignedIntVS(const std::string& varName, unsigned int value);
		void SetUnsignedIntPS(const std::string& varName, unsigned int value);
		void SetIntVS(const std::string& varName, int value);
		void SetIntPS(const std::string& varName, int value);
		void SetFloatVS(const std::string& varName, float value);
		void SetFloatPS(const std::string& varName, float value);
		void SetFloat2VS(const std::string& varName, DirectX::XMFLOAT2 value);
		void SetFloat2PS(const std::string& varName, DirectX::XMFLOAT2 value);
		void SetFloat3VS(const std::string& varName, DirectX::XMFLOAT3 value);
		void SetFloat3PS(const std::string& varName, DirectX::XMFLOAT3 value);
		void SetFloat4VS(const std::string& varName, DirectX::XMFLOAT4 value);
		void SetFloat4PS(const std::string& varName, DirectX::XMFLOAT4 value);

	public:
		//내부적인 VS / PS 운용을 위한 구조체.
		struct MatShaderIntrinsics
		{
			MatShaderIntrinsics()
			{
				_cbByteUpdateBuffer = std::make_unique<Pg::Util::ByteBuffer>();
			}
			//실제로 Shader Update 로직에 활용될 바이트버퍼이다.
			//SetFloat 등 연산에 활용될 예정.
			std::unique_ptr<Pg::Util::ByteBuffer> _cbByteUpdateBuffer;
			uint32_t _cbBufferSize;
			uint32_t _cbRegisterNum;
			
			std::vector<CbMaterialPair> _cbByteVector;
			std::vector<TexMaterialPair> _texPlaceVector;
		};
	private:
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _vsIntrinsics;
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _psIntrinsics;
	};

}


