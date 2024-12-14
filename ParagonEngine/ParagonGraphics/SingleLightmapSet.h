#pragma once
#include <DirectXMath.h>
/// <summary>
/// SingleLightmapSet. �������� SV_InstanceID�� �ش��� ���̴�.
/// </summary>

namespace Pg::Graphics
{
	//������Ʈ (�ν��Ͻ��� �ϳ��� ���� ���̴�)
	struct SingleLightMapSet
	{
		SingleLightMapSet() = default;
		SingleLightMapSet(DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 offset, unsigned int lightmapID)
			: _scale(scale), _offset(offset), _lightmapID(lightmapID) {}

		DirectX::XMFLOAT2 _scale{ 1.f,1.f };
		DirectX::XMFLOAT2 _offset{ 0.f,0.f };
		unsigned int _lightmapID{ 0 };
		DirectX::XMFLOAT3 _padding{};
	};
}
