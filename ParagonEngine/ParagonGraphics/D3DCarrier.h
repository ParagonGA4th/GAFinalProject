#pragma once
#include <d3d11.h>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>

#include "GBufferDepthStencil.h"
#include "GBufferRender.h"

/// <summary>
/// 자주 그래픽스 엔진 내부에서 이동해야 하는 클래스들
/// 묶어서 보관하는 캐리어 구조체.
/// </summary>
namespace Pg::Graphics
{
	struct D3DCarrier
	{
	public:
		D3DCarrier()
			: _rtvArray(), _rtvCount(0), _srvArray(), _srvCount(0), _dsv(nullptr), _quadMainGDS(nullptr), _quadMainRT(nullptr)
		{
			//기본은 모두 nullptr로 채운다.
			std::fill(_rtvArray.begin(), _rtvArray.end(), nullptr);
			std::fill(_srvArray.begin(), _srvArray.end(), nullptr);
		}
		
		//GBuffer 관련 RTVs & SRVs. (Pos, UV Coords, Etc)
		std::vector<std::unique_ptr<GBufferRender>> _gBufRequiredInfoRT;
		std::unique_ptr<GBufferDepthStencil> _gBufRequiredInfoDSV; // Opaque.

		std::vector<ID3D11RenderTargetView*> _gBufRequiredRTVArray;
		std::vector<ID3D11ShaderResourceView*> _gBufRequiredSRVArray;

		//GBuffer 관련 RTV/SRV 맞춰 nullptr 배열 마련.
		std::vector<ID3D11RenderTargetView*> NullRTV{};
		std::vector<ID3D11ShaderResourceView*> NullSRV{};

		//DSV. -> Main Light의 관점에서.
		std::unique_ptr<GBufferRender> _mainLightGBufRT;
		std::unique_ptr<GBufferDepthStencil> _mainLightGBufDSV;

		//MainRenderTarget에 도달하기 전까지 모든 렌더링은 Quad에 이루어진다.
		//단순 정보 전달과 차별화하기 위해 별도의 변수 설정.
		GBufferDepthStencil* _quadMainGDS;
		GBufferRender* _quadMainRT;

		//Post Processing에 활용된다.
		GBufferRender* _PPSwitch1{ nullptr };
		GBufferRender* _PPSwitch2{ nullptr };

		//PostProcessing이 추가되지만, 보내야 하는 값은 일치해야 한다.
		ID3D11ShaderResourceView* _toSendSRVToEngine{ nullptr };

		//PBRBuffer : Binding을 위한 Array.
		std::array<ID3D11RenderTargetView*, 3> _pbrBindArray;
		std::array<ID3D11RenderTargetView*, 3> _pbrNullBindArray;

		std::array<ID3D11RenderTargetView*, 15> _rtvArray;
		unsigned int _rtvCount;
		std::array<ID3D11ShaderResourceView*, 15> _srvArray;
		unsigned int _srvCount;
		ID3D11DepthStencilView* _dsv;

		DirectX::XMMATRIX _mainLightPerspectiveViewProjMatrix;

		//추후에 별도로 라이브러리에 쓰이는 Carrier들 빼어야 한다!
	};
}
