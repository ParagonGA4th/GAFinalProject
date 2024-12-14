#pragma once
#include "DX11Headers.h"
#include <string>

/// <summary>
/// Primitive Object (Ex. Grid & Axis)
/// ������ ���� ������� Ŭ����.
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
		//�ϴ��� ���δ� 0,0,0�� �׸��� �����̴�, �ϰ������� ó���ص� ����.
		void UpdateConstantBuffers(Pg::Data::CameraData* camData);
		void BindConstantBuffers();
		void Render();
		void UnbindConstantBuffers();

	protected:
		//�ϴ��� Wireframe ������Ʈ�� ���� ������ CBuffer�� ����.
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