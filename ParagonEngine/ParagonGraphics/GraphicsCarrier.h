#pragma once
#include <d3d11.h>
#include <array>
#include <algorithm>
/// <summary>
/// 자주 그래픽스 엔진 내부에서 이동해야 하는 클래스들
/// 묶어서 보관하는 캐리어 구조체.
/// </summary>
namespace Pg::Graphics
{
	struct GraphicsCarrier
	{
	public:
		GraphicsCarrier()
			: _rtvArray(), _rtvCount(0), _srvArray(), _srvCount(0), _dsv(nullptr)
		{
			//기본은 모두 nullptr로 채운다.
			std::fill(_rtvArray.begin(), _rtvArray.end(), nullptr);
			std::fill(_srvArray.begin(), _srvArray.end(), nullptr);
		}

		std::array<ID3D11RenderTargetView*, 15> _rtvArray;
		unsigned int _rtvCount;
		std::array<ID3D11ShaderResourceView*, 15> _srvArray;
		unsigned int _srvCount;
		ID3D11DepthStencilView* _dsv;
	};
}
