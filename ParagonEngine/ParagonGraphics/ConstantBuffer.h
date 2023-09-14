#pragma once

#include "DX11Headers.h"

/// <summary>
/// DX11 상수 버퍼를 래핑하는 클래스 템플릿
/// 
/// 2023. 09. 14. 김정식
/// </summary>
namespace Pg::Graphics
{
	class LowDX11Storage;

	template<typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer(LowDX11Storage* DXStorage, T* cbData);

	private:
		LowDX11Storage* _DXStorage;

	public:
		ID3D11Buffer* _ConstantBuffer;
		T* _cbData;
		D3D11_SUBRESOURCE_DATA _subresource;

	public:
		void Update(T* cbData);

	private:
	};
}

namespace Pg::Graphics
{
	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(LowDX11Storage* DXStorage, T* cbData)
		:_DXStorage(DXStorage),
		_ConstantBuffer(nullptr),
		_cbData(cbData)
	{
		int sizeCB = (((sizeof(T)-1) / 16 ) + 1) * 16;

		_DXStorage->_ConstantBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		_DXStorage->_ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_DXStorage->_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_DXStorage->_ConstantBufferDesc.CPUAccessFlags = 0;
		_DXStorage->_ConstantBufferDesc.MiscFlags = 0;

		_subresource.pSysMem = cbData;

		HRESULT hr = _DXStorage->_device->CreateBuffer(&(_DXStorage->_ConstantBufferDesc), &_subresource, &(_ConstantBuffer));

	}

	template<typename T>
	void ConstantBuffer<T>::Update(T* cbData)
	{
		_cbData = cbData;
		_DXStorage->_deviceContext->UpdateSubresource(_ConstantBuffer, 0, NULL, cbData, 0, 0);
		_DXStorage->_deviceContext->VSSetConstantBuffers(0, 1, &(_ConstantBuffer));
	}
}