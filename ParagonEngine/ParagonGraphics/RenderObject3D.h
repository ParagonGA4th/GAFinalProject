#pragma once
#include "RenderObjectBase.h"

#include "ConstantBufferDefine.h"
#include "ConstantBuffer.h"

#include "DX11Headers.h"

/// <summary>
/// GameObject가 그래픽엔진의 렌더링 로직이랑 1대1 연동될 수 있게 하는 클래스이다.
/// 3D랑 호환된다.
/// 
/// Core에서 BaseRenderer가 붙은 GameObject들 :
/// RenderObject는 그래픽엔진에 독자적으로 관리되지 않는다.
/// </summary>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

namespace Pg::Data
{
	class Transform;
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;
	class Asset3DModelData;
	class VertexShader;
	class PixelShader;
	class ConstantBufferDefine;
	struct ConstantBufferDefine::cbPerObjectBase;
	class RenderTexture2D;

}

namespace Pg::Graphics
{
	class RenderObject3D : public Pg::Graphics::RenderObjectBase
	{
	public:
		RenderObject3D(Pg::Data::BaseRenderer* baseRenderer);
		virtual ~RenderObject3D();
		
		virtual void Initialize();
		virtual void Render();

	protected:
		Asset3DModelData* _modelData = nullptr;

	public:
		ConstantBufferDefine::cbPerObjectBase* _constantBufferStruct;

	protected:
		LowDX11Storage* _DXStorage;

		ID3D11Buffer* VB;
		ID3D11Buffer* IB;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		ID3D11SamplerState* _samplerState;

		ID3D11InputLayout* _inputLayout;

	protected:
		virtual void BindBuffers() abstract;

		void BindShaders();
		void UnbindShaders();

		virtual void BindInputLayout();
		void UnbindInputLayout();
	
	protected:
		std::vector<RenderTexture2D*> _textures;

		
	public:	
		//virtual void AddTexture(std::wstring filepath);
		virtual void AddTexture(RenderTexture2D* texture);
		void BindTextures();

	public:
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();

	protected:
		ID3D11Device* _device;
		ID3D11DeviceContext* _devCon;

	public:
		virtual void UpdateConstantBuffers(Pg::Data::CameraData* camData) abstract;
		virtual void BindConstantBuffers() abstract;
		virtual void UnbindConstantBuffers() abstract;


	public:
		// 상수 버퍼들을 저장하는 벡터
		std::vector< ConstantBufferBase* > _constantBuffers;

		// 상수 버퍼 데이터를 추가하는 함수
		template <typename T>
		void CreateConstantBuffer(T* cbData)
		{
			ConstantBufferBase* tCBuffer = new ConstantBuffer<T>(cbData);
			_constantBuffers.emplace_back(tCBuffer);
		}

	};
}