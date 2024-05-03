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

			//이것만 받고 있어야 한다.
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh 관련.
			DirectX::XMFLOAT2	_lightmapUV;
		};

		//Skinned Mesh가 요구하는 포맷. 
		struct Vin1stSkinned
		{
			Vin1stSkinned() = default;

			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh 관련.

			unsigned int	  _blendIndice0;
			unsigned int	  _blendIndice1;
			unsigned int	  _blendIndice2;
			unsigned int	  _blendIndice3;

			float			  _blendWeight0;
			float			  _blendWeight1;
			float			  _blendWeight2;

			unsigned int _nodeIndex;
		};

		//모든 3D Mesh들의 2nd Vertex Buffer. 
		struct Vin2ndAll
		{
			Vin2ndAll() = default;

			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;
		};

		//3rd는 사용법이 다르다. 비 인스턴스 vs 인스턴스.
		//우선, 전자는 VertexCount에 종속적 (단일 모델)
		// 후자는 Instanced Object Count에 종속적. (씬 내부 인스턴스 렌더러 있는 오브젝트 개수)
		struct Vin3rdStaticSkinned
		{
			Vin3rdStaticSkinned() = default;

			unsigned int  _objectID;
			unsigned int  _matID;
		};

		struct Vin3rdInstanced
		{
			Vin3rdInstanced() = default;

			unsigned int  _objectID;
			unsigned int  _matID;
			DirectX::XMMATRIX _transform;
		};

		struct VinDeferredQuad
		{
			VinDeferredQuad() = default;
			VinDeferredQuad(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 norm, DirectX::XMFLOAT2 uv);
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
		static ID3D11InputLayout* GetDeferredQuadLayout();

		static ID3D11InputLayout* GetWireframePrimitiveLayout();
		static ID3D11InputLayout* GetCubemapLayout();

		static ID3D11InputLayout* GetPerObjMatStaticLayout();
		static ID3D11InputLayout* GetPerObjMatSkinnedLayout();
	private:
		static ID3D11InputLayout* _static1stLayout;
		static ID3D11InputLayout* _skinned1stLayout;
		static ID3D11InputLayout* _deferredQuadLayout;
		static ID3D11InputLayout* _wireframePrimitiveLayout; 
		static ID3D11InputLayout* _cubemapLayout;
		static ID3D11InputLayout* _vinPerObjMatStaticLayout;
		static ID3D11InputLayout* _vinPerObjMatSkinnedLayout;

	private:
		static void CreateStatic1stLayout();
		static void CreateSkinned1stLayout();
		static void CreateDeferredQuadLayout();
		static void CreateWireframePrimitiveLayout();
		static void CreateCubemapLayout();
		static void CreatePerObjMatStaticLayout();
		static void CreatePerObjMatSkinnedLayout();

	};

}
