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
			Vin1stStatic() = default;
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
			Vin1stSkinned() = default;

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

		struct Vin2nd
		{
			Vin2nd() = default;
			Vin2nd(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 norm, DirectX::XMFLOAT2 uv);
			DirectX::XMFLOAT3 posL;
			DirectX::XMFLOAT3 normalL;
			DirectX::XMFLOAT2 tex;
		};

		struct VinCubemap
		{
			VinCubemap() = default;
			VinCubemap(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv);
			DirectX::XMFLOAT3 posL;
			DirectX::XMFLOAT2 tex;
		};

		struct VinWireframePrimitive
		{
			VinWireframePrimitive() = default;
			VinWireframePrimitive(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 col);
			DirectX::XMFLOAT3 posL;
			DirectX::XMFLOAT4 color;
		};

		static void Initialize();

		//Static Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetStatic1stLayout();

		//Skinned Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetSkinned1stLayout();

		//2nd Layout 반환
		static ID3D11InputLayout* Get2ndLayout();

		static ID3D11InputLayout* GetWireframePrimitiveLayout();
		static ID3D11InputLayout* GetCubemapLayout();

	private:
		static ID3D11InputLayout* _static1stLayout;
		static ID3D11InputLayout* _skinned1stLayout;
		static ID3D11InputLayout* _2ndLayout;
		static ID3D11InputLayout* _wireframePrimitiveLayout; 
		static ID3D11InputLayout* _cubemapLayout;

	private:
		static void CreateStatic1stLayout();
		static void CreateSkinned1stLayout();
		static void Create2ndLayout();
		static void CreateWireframePrimitiveLayout();
		static void CreateCubemapLayout();

	};

}
