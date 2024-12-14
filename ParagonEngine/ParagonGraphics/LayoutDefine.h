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
		//Individual Components
		//Static Mesh가 요구하는 포맷. + InstancedMesh의 기본이 되기도 한다.
		struct Vin1stStatic_Individual
		{
			Vin1stStatic_Individual() = default;

			//이것만 받고 있어야 한다.
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh 관련.
			DirectX::XMFLOAT2	_lightmapUV;
		};

		//Skinned Mesh가 요구하는 포맷. 
		struct Vin1stSkinned_Individual
		{
			Vin1stSkinned_Individual() = default;

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
			float			  _blendWeight3;

			//unsigned int _nodeIndex;
		};

		//모든 3D Mesh들의 2nd Vertex Buffer. 
		struct Vin2ndAll_Individual
		{
			Vin2ndAll_Individual() = default;

			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;
		};

		//3rd는 사용법이 다르다. 비 인스턴스 vs 인스턴스.
		//우선, 전자는 VertexCount에 종속적 (단일 모델)
		// 후자는 Instanced Object Count에 종속적. (씬 내부 인스턴스 렌더러 있는 오브젝트 개수)
		//struct Vin3rdStaticSkinned_Individual
		//{
		//	Vin3rdStaticSkinned_Individual() = default;
		//
		//	unsigned int  _objectID;
		//	unsigned int  _matID;
		//};

		struct Vin3rdInstanced_Individual
		{
			Vin3rdInstanced_Individual() = default;
			DirectX::XMMATRIX _transform;
			unsigned int  _objectID;
			unsigned int  _matID;
		};

	public:
		//실제로 쓰일 레이아웃들.
		//Alignment Padding이 발생하면 안된다.
		//아쉽지만, 전부 다시 작성해야 할 듯.
		struct Vin1stPassInstanced_Layout
		{
			//Vin1stStatic_Individual
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh 관련.
			DirectX::XMFLOAT2	_lightmapUV;

			//Vin2ndAll_Individual
			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;

			//Vin3rdInstanced_Individual
			DirectX::XMMATRIX _transform;
			unsigned int  _objectID;
			unsigned int  _matID;
		};

		struct Vin1stPassStatic_Layout
		{
			//Vin1stStatic_Individual
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh 관련.
			DirectX::XMFLOAT2	_lightmapUV;

			//Vin2ndAll_Individual
			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;
		};

		struct Vin1stPassSkinned_Layout
		{
			//Vin1stSkinned_Individual
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

			//Vin2ndAll_Individual
			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;
		};

		//struct VinPerThirdPassStatic_Layout
		//{
		//	//Vin1stStatic_Individual
		//	DirectX::XMFLOAT3	_posL;
		//	DirectX::XMFLOAT2	_tex;
		//	float		_meshMatID; //Multi-Mesh 관련.
		//	DirectX::XMFLOAT2	_lightmapUV;
		//
		//	//Vin3rdStaticSkinned_Individual
		//	unsigned int  _objectID;
		//	unsigned int  _matID;
		//};
		//
		//struct VinPerThirdPassSkinned_Layout
		//{
		//	//Vin1stSkinned_Individual
		//	DirectX::XMFLOAT3	_posL;
		//	DirectX::XMFLOAT2	_tex;
		//	float		_meshMatID; //Multi-Mesh 관련.
		//
		//	unsigned int	  _blendIndice0;
		//	unsigned int	  _blendIndice1;
		//	unsigned int	  _blendIndice2;
		//	unsigned int	  _blendIndice3;
		//
		//	float			  _blendWeight0;
		//	float			  _blendWeight1;
		//	float			  _blendWeight2;
		//
		//	unsigned int _nodeIndex;
		//
		//	//Vin3rdStaticSkinned_Individual
		//	unsigned int  _objectID;
		//	unsigned int  _matID;
		//};
		//</실제로 쓰일 레이아웃들>

		//<인스턴싱 따른 레이아웃 재정의 영향을 받지 않는다.>
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

		struct VinSpriteCustom
		{
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 tex;
			DirectX::XMFLOAT4 pos;
		};
		//</인스턴싱 따른 레이아웃 재정의 영향을 받지 않는다.>

		static void Initialize();

		//인스턴스된 레이아웃 반환.
		static ID3D11InputLayout* GetInstanced1stLayout();
		
		//Static Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetStatic1stLayout();

		//Skinned Mesh 요구 레이아웃 반환
		static ID3D11InputLayout* GetSkinned1stLayout();

		//2nd Layout 반환
		static ID3D11InputLayout* GetDeferredQuadLayout();

		static ID3D11InputLayout* GetWireframePrimitiveLayout();
		static ID3D11InputLayout* GetCubemapLayout();
		static ID3D11InputLayout* GetSpriteCustomLayout();

		//static ID3D11InputLayout* GetPerObjMatStaticLayout();
		//static ID3D11InputLayout* GetPerObjMatSkinnedLayout();
	private:
		static ID3D11InputLayout* _instanced1stLayout;
		static ID3D11InputLayout* _static1stLayout;
		static ID3D11InputLayout* _skinned1stLayout;
		static ID3D11InputLayout* _deferredQuadLayout;
		static ID3D11InputLayout* _wireframePrimitiveLayout; 
		static ID3D11InputLayout* _cubemapLayout;
		static ID3D11InputLayout* _spriteCustomLayout;
		//static ID3D11InputLayout* _vinPerObjMatStaticLayout;
		//static ID3D11InputLayout* _vinPerObjMatSkinnedLayout;

	private:
		static void CreateInstanced1stLayout();
		static void CreateStatic1stLayout();
		static void CreateSkinned1stLayout();
		static void CreateDeferredQuadLayout();
		static void CreateWireframePrimitiveLayout();
		static void CreateCubemapLayout();
		static void CreateSpriteCustomLayout();
		//static void CreatePerObjMatStaticLayout();
		//static void CreatePerObjMatSkinnedLayout();

	};

}
