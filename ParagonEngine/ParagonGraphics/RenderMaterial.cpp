#include "RenderMaterial.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "AssetCombinedLoader.h"
#include "RenderTexture.h"
#include "RenderVertexShader.h"
#include "RenderPixelShader.h"

#include <cassert>

namespace Pg::Graphics
{
	using Pg::Data::Resources::GraphicsResource;

	RenderMaterial::RenderMaterial(Pg::Data::Enums::eAssetDefine define, const std::string& filePath) :
		GraphicsResource(define, typeid(this).name(), filePath)
	{
		_DXStorage = LowDX11Storage::GetInstance();

		_vsIntrinsics = std::make_unique<RenderMaterial::MatShaderIntrinsics>();
		_psIntrinsics = std::make_unique<RenderMaterial::MatShaderIntrinsics>();
	}

	RenderMaterial::~RenderMaterial()
	{

	}

	void RenderMaterial::InternalLoad()
	{
		using Pg::Graphics::Manager::GraphicsResourceManager;
		using Pg::Graphics::Helper::GraphicsResourceHelper;
		using Pg::Graphics::Loader::AssetCombinedLoader;

		GraphicsResourceManager* tResManager = Pg::Graphics::Manager::GraphicsResourceManager::Instance();
		AssetCombinedLoader* tComLoader = tResManager->GetCombinedLoader();

		//만약 자신 내부에 "DefaultMaterial_"이 있으면 디폴트 매터리얼 로드, 아니면 커스텀 로드.
		if (_filePath.find(Pg::Defines::GENERATED_MATERIAL_PREFIX) != std::string::npos)
		{
			//디폴트 매터리얼이다.
			tComLoader->LoadDefaultRenderMaterial(_filePath, this);
		}
		else
		{
			//커스텀 매터리얼이다.
			tComLoader->LoadCustomRenderMaterial(_filePath, this);
		}	
	}

	void RenderMaterial::InternalUnload()
	{
		//Internal Unload Logic, TBA.
	}

	void RenderMaterial::SetBoolVS(const std::string& varName, bool value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_BOOL);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->putInt((int)value, tByteOffset);
	}

	void RenderMaterial::SetBoolPS(const std::string& varName, bool value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_BOOL);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->putInt((int)value, tByteOffset);
	}

	void RenderMaterial::SetUnsignedIntVS(const std::string& varName, unsigned int value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_UINT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->putUnsignedInt(value, tByteOffset);
	}

	void RenderMaterial::SetUnsignedIntPS(const std::string& varName, unsigned int value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_UINT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->putUnsignedInt(value, tByteOffset);
	}

	void RenderMaterial::SetIntVS(const std::string& varName, int value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_INT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->putInt(value, tByteOffset);
	}

	void RenderMaterial::SetIntPS(const std::string& varName, int value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_INT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->putInt(value, tByteOffset);
	}

	void RenderMaterial::SetFloatVS(const std::string& varName, float value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->putFloat(value, tByteOffset);
	}

	void RenderMaterial::SetFloatPS(const std::string& varName, float value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->putFloat(value, tByteOffset);
	}

	void RenderMaterial::SetFloat2VS(const std::string& varName, DirectX::XMFLOAT2 value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT2);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->PutXMFloat2(value, tByteOffset);
	}

	void RenderMaterial::SetFloat2PS(const std::string& varName, DirectX::XMFLOAT2 value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT2);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->PutXMFloat2(value, tByteOffset);
	}

	void RenderMaterial::SetFloat3VS(const std::string& varName, DirectX::XMFLOAT3 value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT3);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->PutXMFloat3(value, tByteOffset);
	}

	void RenderMaterial::SetFloat3PS(const std::string& varName, DirectX::XMFLOAT3 value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT3);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->PutXMFloat3(value, tByteOffset);
	}

	void RenderMaterial::SetFloat4VS(const std::string& varName, DirectX::XMFLOAT4 value)
	{
		auto tIterator = std::find_if(_vsIntrinsics->_cbByteVector.begin(), _vsIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _vsIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT4);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_vsIntrinsics->_cbByteUpdateBuffer->PutXMFloat4(value, tByteOffset);
	}

	void RenderMaterial::SetFloat4PS(const std::string& varName, DirectX::XMFLOAT4 value)
	{
		auto tIterator = std::find_if(_psIntrinsics->_cbByteVector.begin(), _psIntrinsics->_cbByteVector.end(),
			[&varName](const CbMaterialPair& element) { return element.first == varName; });

		if (tIterator == _psIntrinsics->_cbByteVector.end())
		{
			assert(false && "없는 변수 값을 변경하려고 함");
		}

		//해당 값이 실제 타입과 맞는지를 체크.
		eCbVarType tVarType = tIterator->second.first;
		assert(tVarType == _CB_FLOAT4);

		//바이트 오프셋 가지고 오기 + Place (HLSL식)
		unsigned int tByteOffset = tIterator->second.second;
		_psIntrinsics->_cbByteUpdateBuffer->PutXMFloat4(value, tByteOffset);
	}

	void RenderMaterial::Bind(ID3D11InputLayout* customInputLayout)
	{
		//VS Binding
		_vertexShader->Bind(customInputLayout);
		_pixelShader->Bind();

		//Vertex Shader
		{
			//VS Constant Buffer Update. (현재로서는 각 Material 당 하나만 지원)
			if (_vsIntrinsics->_cbBufferSize > 0)
			{
				D3D11_MAPPED_SUBRESOURCE res;
				ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

				//Buffer = Single Subresource. 레지스터 넘버가 아니다! 이 코드가 0이 맞는 이유.
				//RegisterNumber는 실제로 GPU에 셋해줄 때 달라진다.
				HR(_DXStorage->_deviceContext->Map(_vsIntrinsics->_cBuffer, 0,
					D3D11_MAP_WRITE_DISCARD, 0, &res));

				//버퍼의 크기와 먼저 받았던 Constant Buffer의 크기가 같은지 확인.
				assert(_vsIntrinsics->_cbBufferSize == _vsIntrinsics->_cbByteUpdateBuffer->size());

				//_vsIntrinsics->_cbByteUpdateBuffer->setReadPos(0);
				//_vsIntrinsics->_cbByteUpdateBuffer->getBytes(tGetByteArray, _vsIntrinsics->_cbBufferSize);
				//res.pData = _vsIntrinsics->_cbByteUpdateBuffer->GetStartAddress();
				memcpy(res.pData, _vsIntrinsics->_cbByteUpdateBuffer->GetStartAddress(), _vsIntrinsics->_cbBufferSize);

				_DXStorage->_deviceContext->Unmap(_vsIntrinsics->_cBuffer, 0);

				//VS Constant Buffer Set.
				_DXStorage->_deviceContext->VSSetConstantBuffers(_vsIntrinsics->_cbRegisterNum, 1, &(_vsIntrinsics->_cBuffer));
			}
			
			//VS Textures Update.
			{
				for (auto& it : _vsIntrinsics->_texPlaceVector)
				{
					const auto& [bTexType, bRegNum, bRenderTexture] = it.second;
					_DXStorage->_deviceContext->VSSetShaderResources(bRegNum, 1, &(bRenderTexture->GetSRV()));
				}
			}
		}

		//Pixel Shader
		{
			//PS Constant Buffer Update. (현재로서는 각 Material 당 하나만 지원)
			if (_psIntrinsics->_cbBufferSize > 0)
			{
				D3D11_MAPPED_SUBRESOURCE res;
				ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

				//Buffer = Single Subresource. 레지스터 넘버가 아니다! 이 코드가 0이 맞는 이유.
				//RegisterNumber는 실제로 GPU에 셋해줄 때 달라진다.
				HR(_DXStorage->_deviceContext->Map(_psIntrinsics->_cBuffer, 0,
					D3D11_MAP_WRITE_DISCARD, 0, &res));

				//버퍼의 크기와 먼저 받았던 Constant Buffer의 크기가 같은지 확인.
				assert(_psIntrinsics->_cbBufferSize == _psIntrinsics->_cbByteUpdateBuffer->size());

				//ReadPos를 리셋.
				//_psIntrinsics->_cbByteUpdateBuffer->setReadPos(0);
				//_psIntrinsics->_cbByteUpdateBuffer->getBytes((uint8_t*)res.pData, _psIntrinsics->_cbBufferSize);
				//res.pData = _psIntrinsics->_cbByteUpdateBuffer->GetStartAddress();
				memcpy(res.pData, _psIntrinsics->_cbByteUpdateBuffer->GetStartAddress(), _psIntrinsics->_cbBufferSize);

				_DXStorage->_deviceContext->Unmap(_psIntrinsics->_cBuffer, 0);

				//PS Constant Buffer Set.
				_DXStorage->_deviceContext->PSSetConstantBuffers(_psIntrinsics->_cbRegisterNum, 1, &(_psIntrinsics->_cBuffer));
			}

			//PS Textures Update.
			{
				for (auto& it : _psIntrinsics->_texPlaceVector)
				{
					const auto& [bTexType, bRegNum, bRenderTexture] = it.second;
					_DXStorage->_deviceContext->PSSetShaderResources(bRegNum, 1, &(bRenderTexture->GetSRV()));
				}
			}
		}
	}

	void RenderMaterial::Unbind()
	{
		_vertexShader->Unbind();
		_pixelShader->Unbind();

		//언바인딩 코드. 
		if (_vsIntrinsics->_cbBufferSize > 0)
		{
			//할당해제.
			ID3D11Buffer* nullBuffer = nullptr;
			_DXStorage->_deviceContext->VSSetConstantBuffers(_vsIntrinsics->_cbRegisterNum, 1, &nullBuffer);
		}
		if (_psIntrinsics->_cbBufferSize > 0)
		{
			//할당해제.
			ID3D11Buffer* nullBuffer = nullptr;
			_DXStorage->_deviceContext->PSSetConstantBuffers(_psIntrinsics->_cbRegisterNum, 1, &nullBuffer);
		}

		for (int i = 0; i < _vsIntrinsics->_texPlaceVector.size(); i++)
		{
			auto bRegNum = std::get<1>(_vsIntrinsics->_texPlaceVector[i].second);

			//할당해제.
			ID3D11ShaderResourceView* nullSRV = nullptr;
			_DXStorage->_deviceContext->VSSetShaderResources(bRegNum, 1, &nullSRV);
		}

		for (int i = 0; i < _psIntrinsics->_texPlaceVector.size(); i++)
		{
			auto bRegNum = std::get<1>(_psIntrinsics->_texPlaceVector[i].second);

			//할당 해제.
			ID3D11ShaderResourceView* nullSRV = nullptr;
			_DXStorage->_deviceContext->PSSetShaderResources(bRegNum, 1, &nullSRV);
		}
	}

	unsigned int& RenderMaterial::GetMaterialID()
	{
		return _materialID;
	}

	bool RenderMaterial::GetIsUseAlphaBlending()
	{
		return _isUseAlphaBlending;
	}
}