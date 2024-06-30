#pragma once
#include <d3d11.h>
#include <cassert>
namespace Pg::Graphics
{
	class VERenderingSet;

	//기본적으로 Alpha Blend가 꺼져 있으면 Alpha Clip일 것이고,
	//그 반대면 돌리는 것.

	class BaseCustomEffect
	{
	public:
		//Apply Override를 의도한 대로 써먹기 위해서.
		BaseCustomEffect(VERenderingSet* veSet) : _veSet(veSet) {}
		
		virtual void SetTexture(unsigned int index, ID3D11ShaderResourceView* value) abstract;
		virtual unsigned int GetTextureCount() abstract;

	protected:
		VERenderingSet* _veSet;
	};
}
