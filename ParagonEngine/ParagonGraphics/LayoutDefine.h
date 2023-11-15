#pragma once
#include <DirectXMath.h>

/// <summary>
/// 그래픽 엔진에서 사용되는 양식이 하나로 통일됨에 따라, 미리 정의해놓음! 
/// </summary>

struct ID3D11InputLayout;

namespace Pg::Graphics
{
	class LayoutDefine
	{
	public:
		//Static Mesh가 요구하는 포맷. 
		struct Vin1stStatic
		{
			Vin1stStatic();
			Vin1stStatic(DirectX::XMFLOAT3 posVal);
			Vin1stStatic(DirectX::XMFLOAT3 posVal, DirectX::XMFLOAT3 normalVal,
				DirectX::XMFLOAT3 tangentVal, DirectX::XMFLOAT4 colorVal, 
				DirectX::XMFLOAT3 texVal, unsigned int matIDVal);

			DirectX::XMFLOAT3 posL;
			DirectX::XMFLOAT3 normalL;
			DirectX::XMFLOAT3 tangentL;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT3 tex;
			unsigned int	  matID;
		};

		//Skinned Mesh가 요구하는 포맷. 
		struct Vin1stSkinned
		{
			Vin1stSkinned();
			DirectX::XMFLOAT3 posL;
			DirectX::XMFLOAT3 normalL;
			DirectX::XMFLOAT3 tangentL;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT3 tex;
			unsigned int	  matID;
			
			unsigned int	  blendIndice0;
			unsigned int	  blendIndice1;
			unsigned int	  blendIndice2;
			unsigned int	  blendIndice3;

			float			  blendWeight0;
			float			  blendWeight1;
			float			  blendWeight2;
		};

		static void Initialize();

		//Static Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetStatic1stLayout();

		//Skinned Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetSkinned1stLayout();

		static ID3D11InputLayout* Get2ndLayout();

	private:
		static ID3D11InputLayout* _static1stLayout;
		static ID3D11InputLayout* _skinned1stLayout;
		static ID3D11InputLayout* _2ndLayout;

	private:


	};

}
