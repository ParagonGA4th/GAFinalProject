#pragma once
#include "ConstantBufferDefine.h"
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

/// <summary>
/// Deferred Rendering이 전역적으로 자리잡기 전에, 
/// 다중 Material 적용되는 Mesh 렌더 데모를 위해.
/// </summary>

// #TODO_231015 : 문제 : SRV가 일단 첫번째로 FBX에서 잘 안 넘어오고, 동시에 여러개의 Mesh에 알맞게 렌더하는 것도 어긋나는 상황. 데이터를 자세하게 보고, XXXMaterialDraw를 오가면서 문제 찾아야!
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
		MultimaterialMesh(const std::string& filePath);
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

		//결국 얘가 호출되어야 한다.
		void MultiMaterialDraw();

		//망가지는지, 아닌지를 모기 위해 쓰는 단일 DrawIndexed.
		void SingleMaterialDraw();

		//SingleMaterialDraw에서 여러 개의 Mesh가 혼용된 버전.
		void SingleMaterialMultiMeshDraw();

	private:
		DirectX::XMMATRIX _worldMat;
		DirectX::XMMATRIX _worldInvTransposeMat;
	private:
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
	private:
		//SRV 자체의 문제인지 테스트 위해서, SRV를 자체적으로 만듬.
		ID3D11ShaderResourceView* _testSRV = nullptr;
		ID3D11Resource* _testResource = nullptr;
	};
}


