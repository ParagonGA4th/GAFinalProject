#pragma once
#include <d3d11.h>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>

#include "GBufferDepthStencil.h"
#include "GBufferRender.h"

/// <summary>
/// ���� �׷��Ƚ� ���� ���ο��� �̵��ؾ� �ϴ� Ŭ������
/// ��� �����ϴ� ĳ���� ����ü.
/// </summary>
namespace Pg::Graphics
{
	struct D3DCarrier
	{
	public:
		D3DCarrier()
			: _rtvArray(), _rtvCount(0), _srvArray(), _srvCount(0), _dsv(nullptr), _quadMainGDS(nullptr), _quadMainRT(nullptr)
		{
			//�⺻�� ��� nullptr�� ä���.
			std::fill(_rtvArray.begin(), _rtvArray.end(), nullptr);
			std::fill(_srvArray.begin(), _srvArray.end(), nullptr);
		}
		
		//GBuffer ���� RTVs & SRVs. (Pos, UV Coords, Etc)
		std::vector<std::unique_ptr<GBufferRender>> _gBufRequiredInfoRT;
		std::unique_ptr<GBufferDepthStencil> _gBufRequiredInfoDSV; // Opaque.

		std::vector<ID3D11RenderTargetView*> _gBufRequiredRTVArray;
		std::vector<ID3D11ShaderResourceView*> _gBufRequiredSRVArray;

		//GBuffer ���� RTV/SRV ���� nullptr �迭 ����.
		std::vector<ID3D11RenderTargetView*> NullRTV{};
		std::vector<ID3D11ShaderResourceView*> NullSRV{};

		//DSV. -> Main Light�� ��������.
		std::unique_ptr<GBufferRender> _mainLightGBufRT;
		std::unique_ptr<GBufferDepthStencil> _mainLightGBufDSV;

		//MainRenderTarget�� �����ϱ� ������ ��� �������� Quad�� �̷������.
		//�ܼ� ���� ���ް� ����ȭ�ϱ� ���� ������ ���� ����.
		GBufferDepthStencil* _quadMainGDS;
		GBufferRender* _quadMainRT;

		//Post Processing�� Ȱ��ȴ�.
		GBufferRender* _PPSwitch1{ nullptr };
		GBufferRender* _PPSwitch2{ nullptr };

		//PostProcessing�� �߰�������, ������ �ϴ� ���� ��ġ�ؾ� �Ѵ�.
		ID3D11ShaderResourceView* _toSendSRVToEngine{ nullptr };

		//PBRBuffer : Binding�� ���� Array.
		std::array<ID3D11RenderTargetView*, 3> _pbrBindArray;
		std::array<ID3D11RenderTargetView*, 3> _pbrNullBindArray;

		std::array<ID3D11RenderTargetView*, 15> _rtvArray;
		unsigned int _rtvCount;
		std::array<ID3D11ShaderResourceView*, 15> _srvArray;
		unsigned int _srvCount;
		ID3D11DepthStencilView* _dsv;

		DirectX::XMMATRIX _mainLightPerspectiveViewProjMatrix;

		//���Ŀ� ������ ���̺귯���� ���̴� Carrier�� ����� �Ѵ�!
	};
}
