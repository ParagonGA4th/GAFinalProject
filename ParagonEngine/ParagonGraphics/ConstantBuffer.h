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
		ConstantBuffer();
		ConstantBuffer(unsigned int size);
		virtual ~ConstantBuffer();

		//Constant Buffer의 기본이 되는 함수들.
		void Update();
		void BindVS(unsigned int index);
		void UnbindVS(unsigned int index);
		void BindPS(unsigned int index);
		void UnbindPS(unsigned int index);
		ID3D11Buffer* GetBuffer();

		//실제로 CBuffer의 구조체적 내용물을 채운다.
		T* GetDataStruct();

		//

	private:
		ID3D11Buffer* _Buffer;
		ID3D11Buffer* _NullBuffer;
		T* _cbData;
		D3D11_SUBRESOURCE_DATA _subresource;
		D3D11_BUFFER_DESC _desc;

	private:
		LowDX11Storage* _DXStorage;
	};
}

// 구현부
namespace Pg::Graphics
{
	template<typename T>
	ConstantBuffer<T>::ConstantBuffer()
		:_DXStorage(LowDX11Storage::GetInstance()),
		_Buffer(nullptr), _NullBuffer(nullptr),
		_cbData(nullptr)
	{
		//템플릿 데이터에 따라 구조체를 만드는 코드.
		_cbData = new T();

		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(T) - 1) / 16) + 1) * 16;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		_desc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		_desc.Usage = D3D11_USAGE_DYNAMIC;
		_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_desc.MiscFlags = 0;

		_subresource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&_desc, &_subresource, &(_Buffer)));

#if defined(DEBUG) | defined(_DEBUG)
		std::string tName = typeid(T).name();
		tName += "_CB";
		HR(_Buffer->SetPrivateData(WKPDID_D3DDebugObjectName, tName.length(), tName.data()));
#endif
	}

	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(unsigned int size)
		:_DXStorage(LowDX11Storage::GetInstance()),
		_Buffer(nullptr), _NullBuffer(nullptr),
		_cbData(nullptr)
	{
		//템플릿 데이터에 따라 구조체를 만드는 코드.
		_cbData = new T();

		//Constant Buffer 자체를 만드는 코드.
		int sizeCB = (((sizeof(T) - 1) / 16) + 1) * 16 * size;	// declspec 으로 16바이트 정렬할 수 있다?
		assert(sizeCB % 16 == 0);

		_desc.ByteWidth = sizeCB; // 상수버퍼는 16바이트 정렬
		_desc.Usage = D3D11_USAGE_DYNAMIC;
		_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_desc.MiscFlags = 0;

		_subresource.pSysMem = _cbData;

		HR(_DXStorage->_device->CreateBuffer(&_desc, &_subresource, &(_Buffer)));

#if defined(DEBUG) | defined(_DEBUG)
		std::string tName = typeid(T).name();
		tName += "_CB";
		HR(_Buffer->SetPrivateData(WKPDID_D3DDebugObjectName, tName.length(), tName.data()));
#endif
	}

	template<typename T>
	ConstantBuffer<T>::~ConstantBuffer()
	{
		if (_cbData != nullptr)
		{
			delete _cbData;
		}

		ReleaseCOM(_Buffer);
	}

	template<typename T>
	void Pg::Graphics::ConstantBuffer<T>::Update()
	{
		//Mapped Subresource 메모리 클린.
		D3D11_MAPPED_SUBRESOURCE res;
		ZeroMemory(&res, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HR(_DXStorage->_deviceContext->Map(_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

		//자신의 Data 시작 포인터 받아오기.
		T* data = reinterpret_cast<T*>(res.pData);

		//이미 자신은 GetDataStruct를 통해서 업데이트되었을 것. 
		*(data) = *_cbData;

		_DXStorage->_deviceContext->Unmap(_Buffer, 0);
	}

	template<typename T>
	void ConstantBuffer<T>::BindVS(unsigned int index)
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(index, 1, &_Buffer);
		
	}

	template<typename T>
	void ConstantBuffer<T>::UnbindVS(unsigned int index)
	{
		_DXStorage->_deviceContext->VSSetConstantBuffers(index, 1, &_NullBuffer);
	}

	template<typename T>
	ID3D11Buffer* ConstantBuffer<T>::GetBuffer()
	{
		return _Buffer;
	}

	template<typename T>
	T* ConstantBuffer<T>::GetDataStruct()
	{
		return _cbData;
	}

	template<typename T>
	void ConstantBuffer<T>::BindPS(unsigned int index)
	{
		_DXStorage->_deviceContext->PSSetConstantBuffers(index, 1, &_Buffer);
	}

	template<typename T>
	void ConstantBuffer<T>::UnbindPS(unsigned int index)
	{
		_DXStorage->_deviceContext->PSSetConstantBuffers(index, 1, &_NullBuffer);
	}

	
}