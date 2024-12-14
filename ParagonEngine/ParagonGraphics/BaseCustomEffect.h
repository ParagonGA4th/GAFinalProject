#pragma once
#include "LowDX11Storage.h"

#include <d3d11.h>
#include <cassert>
namespace Pg::Graphics
{
	class VERenderingSet;

	//�⺻������ Alpha Blend�� ���� ������ Alpha Clip�� ���̰�,
	//�� �ݴ�� ������ ��.

	class BaseCustomEffect
	{
	public:
		//Apply Override�� �ǵ��� ��� ��Ա� ���ؼ�.
		BaseCustomEffect(VERenderingSet* veSet) : 
			_veSet(veSet), _DXStorage(LowDX11Storage::GetInstance()) {}
		
		virtual void SetTexture(unsigned int index, ID3D11ShaderResourceView*& value) abstract;
		virtual unsigned int GetTextureCount() abstract;

	protected:
		VERenderingSet* _veSet;
		LowDX11Storage* _DXStorage;
	};
}
