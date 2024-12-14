#pragma once
#include "DX11Headers.h"
#include <string>

/// <summary>
/// Primitive Object (Ex. Grid & Axis)
/// 렌더를 위한 만들어진 클래스.
/// </summary>

namespace Pg::Data
{
	class CameraData;
}

namespace Pg::Graphics
{
	class LowDX11Storage;

	class WireframeRenderObject
	{
	public:
		WireframeRenderObject();
		virtual ~WireframeRenderObject();

	public:
		virtual void BuildBuffers() abstract;
	public:
		void CreateConstantBuffers();
	public:
		//일단은 전부다 0,0,0에 그리는 상태이니, 일괄적으로 처리해도 ㄱㅊ.
		void UpdateConstantBuffers(Pg::Data::CameraData* camData);
		void BindConstantBuffers();
		void Render();
		void UnbindConstantBuffers();

	protected:
		//일단은 Wireframe 오브젝트들 전부 동일한 CBuffer를 공유.
		struct CB
		{
			DirectX::XMFLOAT4X4 worldMatrix;
			DirectX::XMFLOAT4X4 viewProjMatrix;
			DirectX::XMFLOAT3 camPos;
		};

	protected:
		LowDX11Storage* _DXStorage;

		//Vertex Buffer, Index Buffer.
		ID3D11Buffer* _VB;
		ID3D11Buffer* _IB;

		//Constant Buffer.
		D3D11_SUBRESOURCE_DATA _cbSubresData;
		ID3D11Buffer* _cBuffer;
		ID3D11Buffer* _nullBuffer = nullptr;
		WireframeRenderObject::CB _cbData;

		UINT _indexSize = 0;
	};

}