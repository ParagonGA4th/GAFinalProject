#pragma once
#include "../ParagonData/GraphicsResource.h"
#include "../ParagonHelper/ByteBuffer.hpp"
#include "ShaderParsingData.h"
#include <vector>
#include <string>
#include <tuple>
#include <DirectXMath.h>

/// <summary>
/// ���� Material�� 1��1 �����Ǵ� Ŭ����. ���ӿ������� ���޵� Material�� �����Ǿ� �����ȴ�.
/// </summary>

struct ID3D11Buffer;
struct D3D11_SUBRESOURCE_DATA;
struct ID3D11InputLayout;

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
	//�������� SetValue ������ ���� Constant Buffer �迭 -> Byte Set ���� Enum ���� ���� ����
	//								VariableName,		VariableType, ByteOffset
	using CbMaterialPair = std::pair<std::string, std::pair<eCbVarType, unsigned int>>;

	////							VariableName,			TextureType,	RegisterNumber, ���� Texture.
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

		//���������� �ε�, ��ε����ִ� �Լ���. ���� �ڽ� Ŭ�������� ����ü�� ����/�Ҹ��ڿ��� ȣ��Ǿ��.
		virtual void InternalLoad() override;
		virtual void InternalUnload() override;

		// �� ������Ʈ���� ������Ʈ�Ǵ°� �ƴ�. 
		// ���� Material ��� �ִ� ������Ʈ ������ ������Ʈ: Texture, SRV, Material�� ConstantBuffer.
		void Bind(ID3D11InputLayout* customInputLayout = nullptr);
		void Unbind();

		unsigned int& GetMaterialID();

		//���� ������ ������ ����. (���� Transparency)
		bool GetIsUseAlphaBlending();

	public:
		//SetXXX �Լ���. Vertex Shader, Pixel Shader ������ �ٸ���.
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
		//�������� VS / PS ����� ���� ����ü.
		struct MatShaderIntrinsics
		{
			MatShaderIntrinsics() : _cbByteUpdateBuffer(), _cbBufferSize(0), _cBuffer(nullptr)
			{
				_cbByteUpdateBuffer = std::make_unique<Pg::Util::ByteBuffer>();
				_cbSubResData = std::make_unique<D3D11_SUBRESOURCE_DATA>();
			}
			//������ Shader Update ������ Ȱ��� ����Ʈ�����̴�.
			//SetFloat �� ���꿡 Ȱ��� ����.
			std::unique_ptr<Pg::Util::ByteBuffer> _cbByteUpdateBuffer;
			uint32_t _cbBufferSize;
			uint32_t _cbRegisterNum;
			ID3D11Buffer* _cBuffer;
			std::unique_ptr<D3D11_SUBRESOURCE_DATA> _cbSubResData;

			std::vector<CbMaterialPair> _cbByteVector;
			std::vector<TexMaterialPair> _texPlaceVector;
		};

		//Remapping�� ����.
		enum class eInitState { _NONE, _FROM_SCENE, _ADDED_LATER };

	private:
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _vsIntrinsics;
		std::unique_ptr<RenderMaterial::MatShaderIntrinsics> _psIntrinsics;

		RenderVertexShader* _vertexShader;
		RenderPixelShader* _pixelShader;

		//������ �Ǹ鼭 ���� Opaque Material ID�� �ο��ȴ�.
		unsigned int _materialID{ 1 }; //1�� ����.

		//�ܺο��� Scene�� ���۵Ǿ��� �� ��ŷ��.
		RenderMaterial::eInitState _initState{ RenderMaterial::eInitState::_NONE };

		//IsUseAlphaBlending
		bool _isUseAlphaBlending{ false };
		//���� ������ ���Ǿ��� ��, Static / Skinned ���α��� �Ǵ�. -> �ٸ� Shader�� �Ҵ��ϱ� ����!
		bool _whenAlphaIsSkinned{ false }; 

	private:
		LowDX11Storage* _DXStorage;
	};

}


