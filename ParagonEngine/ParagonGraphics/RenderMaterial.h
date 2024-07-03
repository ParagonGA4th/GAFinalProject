#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonHelper/ByteBuffer.hpp"
#include "ShaderParsingData.h"
#include <vector>
#include <string>
#include <tuple>
#include <DirectXMath.h>

/// <summary>
/// 실제 Material과 1대1 대응되는 클래스. 게임엔진에서 저잗된 Material과 연동되어 보관된다.
/// </summary>

struct ID3D11Buffer;
struct D3D11_SUBRESOURCE_DATA;

namespace Pg::Graphics
{
	class RenderTexture;
	class MaterialParser;
	class RenderVertexShader;
	class RenderPixelShader;
	class LowDX11Storage;

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

		// 매 오브젝트마다 업데이트되는거 아님. 
		// 같은 Material 들고 있는 오브젝트 단위로 업데이트: Texture, SRV, Material별 ConstantBuffer.
		void Bind();
		void Unbind();

		unsigned int& GetMaterialID();

		//알파 블렌딩을 쓰는지 리턴. (쓰면 Transparency)
		bool GetIsUseAlphaBlending();

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
			MatShaderIntrinsics() : _cbByteUpdateBuffer(), _cbBufferSize(0), _cBuffer(nullptr)
			{
				_cbByteUpdateBuffer = std::make_unique<Pg::Util::ByteBuffer>();
				_cbSubResData = std::make_unique<D3D11_SUBRESOURCE_DATA>();
			}
			//실제로 Shader Update 로직에 활용될 바이트버퍼이다.
			//SetFloat 등 연산에 활용될 예정.
			std::unique_ptr<Pg::Util::ByteBuffer> _cbByteUpdateBuffer;
			uint32_t _cbBufferSize;
			uint32_t _cbRegisterNum;
			ID3D11Buffer* _cBuffer;
			std::unique_ptr<D3D11_SUBRESOURCE_DATA> _cbSubResData;

			std::vector<CbMaterialPair> _cbByteVector;
			std::vector<TexMaterialPair> _texPlaceVector;
		};

		//Remapping을 위해.
		enum class eInitState { _NONE, _FROM_SCENE, _ADDED_LATER };

	private:
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _vsIntrinsics;
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _psIntrinsics;

		RenderVertexShader* _vertexShader;
		RenderPixelShader* _pixelShader;

		//렌더가 되면서 새로 Opaque Material ID가 부여된다.
		unsigned int _materialID{ 1 }; //1로 시작.

		//외부에서 Scene이 시작되었을 때 마킹됨.
		RenderMaterial::eInitState _initState{ RenderMaterial::eInitState::_NONE };

		//IsUseAlphaBlending
		bool _isUseAlphaBlending{ false };

	private:
		LowDX11Storage* _DXStorage;
	};

}


