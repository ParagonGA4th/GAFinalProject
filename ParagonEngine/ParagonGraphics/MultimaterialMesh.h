#pragma once
#include "ConstantBufferDefine.h"
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

/// <summary>
/// Deferred Rendering이 전역적으로 자리잡기 전에, 
/// 다중 Material 적용되는 Mesh 렌더 데모를 위해.
/// </summary>

namespace Pg::Data
{
	struct CameraData;
}

namespace Pg::Graphics
{
	class Asset3DModelData;
}

namespace Pg::Graphics
{
	class MultimaterialMesh
	{
	public:
		MultimaterialMesh();
		~MultimaterialMesh();

		//Initializing. (로드된 Asset3DModelData를 갖고 오는 역할)
		void Initialize();

		//단순한 Mesh Rendering.
		void Draw(Pg::Data::CameraData* camData);

	private:
		void CreateSamplerState();
		void CreateVertexPixelShader();
		void CreateRasterizerState();
		void CreateConstantBuffer();
		void SetupMatrices();
	private:
		void UpdateConstantBuffer(Pg::Data::CameraData* camData);

	private:
		DirectX::XMMATRIX _worldMat;
		DirectX::XMMATRIX _worldInvTransposeMat;
	private:
		std::string _filePath;
		Asset3DModelData* _modelData = nullptr;
		ConstantBufferDefine::cbPerObjectBase* _constantBufferStruct;
	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _devCon;

		ID3D11VertexShader* _vertexShader = nullptr;
		ID3D11PixelShader* _pixelShader = nullptr;

		D3D11_SUBRESOURCE_DATA _cbufferSubresourceData;
		ID3D11Buffer* _constantBuffer = nullptr;
		ID3D11SamplerState* _samplerState = nullptr;
		ID3D11RasterizerState* _rasterizerState = nullptr;
	};
}


