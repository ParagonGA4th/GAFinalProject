#pragma once

#include "DX11Headers.h"
#include "ConstantBufferBase.h"

/// <summary>
/// DX11 상수 버퍼를 래핑하는 클래스 템플릿
/// 
/// 2023. 09. 14. 김정식
/// </summary>
namespace Pg::Graphics
{
	class LowDX11Storage;

	template<typename T>
	class ConstantBuffer : public ConstantBufferBase
	{
	public:
		ConstantBuffer(T* cbData);
		ConstantBuffer(T* cbData, unsigned int size);
		virtual ~ConstantBuffer();

	private:
		LowDX11Storage* _DXStorage;

	public:
		ID3D11Buffer* _Buffer;
		T* _cbData;
		D3D11_SUBRESOURCE_DATA _subresource;

	public:
		virtual void UpdateAndBind(UINT num) override;
		virtual ID3D11Buffer* GetBuffer() override;

	private:
	};

}

// 구현부
namespace Pg::Graphics
{
	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(T* cbData)
		:_DXStorage(LowDX11Storage::GetInstance()),
		_Buffer(nullptr),
		_cbData(cbData)
	{
		int sizeCB = (((sizeof(T)-1) / 16 ) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?

		_DXStorage->_ConstantBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		_DXStorage->_ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_DXStorage->_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_DXStorage->_ConstantBufferDesc.CPUAccessFlags = 0;
		_DXStorage->_ConstantBufferDesc.MiscFlags = 0;

		_subresource.pSysMem = cbData;

		HRESULT hr = _DXStorage->_device->CreateBuffer(&(_DXStorage->_ConstantBufferDesc), &_subresource, &(_Buffer));

	}

	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(T* cbData, unsigned int size)
		:_DXStorage(LowDX11Storage::GetInstance()),
		_Buffer(nullptr),
		_cbData(cbData)
	{
		int sizeCB = (((sizeof(T) - 1) / 16) + 1) * 16 * size;	// declspec 으로 16바이트 정렬할 수 있다?

		_DXStorage->_ConstantBufferDesc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		_DXStorage->_ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_DXStorage->_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_DXStorage->_ConstantBufferDesc.CPUAccessFlags = 0;
		_DXStorage->_ConstantBufferDesc.MiscFlags = 0;

		_subresource.pSysMem = cbData;

		HRESULT hr = _DXStorage->_device->CreateBuffer(&(_DXStorage->_ConstantBufferDesc), &_subresource, &(_Buffer));

	}

	template<typename T>
	ConstantBuffer<T>::~ConstantBuffer()
	{
		delete _cbData;
		delete _Buffer;
	}

	template<typename T>
	void ConstantBuffer<T>::UpdateAndBind(UINT index)
	{	
		_DXStorage->_deviceContext->UpdateSubresource(_Buffer, 0, NULL, _cbData, 0, 0);
		_DXStorage->_deviceContext->VSSetConstantBuffers(index, 1, &_Buffer);
		_DXStorage->_deviceContext->PSSetConstantBuffers(index, 1, &_Buffer);
	}

	template<typename T>
	ID3D11Buffer* ConstantBuffer<T>::GetBuffer()
	{
		return _Buffer;
	}
}