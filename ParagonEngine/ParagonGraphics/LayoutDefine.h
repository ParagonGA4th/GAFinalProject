#pragma once
#include <DirectXMath.h>

/// <summary>
/// �׷��� �������� ���Ǵ� ����� �ϳ��� ���ϵʿ� ����, �̸� �����س���! 
/// </summary>

struct ID3D11InputLayout;

namespace Pg::Graphics
{
	class LayoutDefine
	{
	public:
		//Individual Components
		//Static Mesh�� �䱸�ϴ� ����. + InstancedMesh�� �⺻�� �Ǳ⵵ �Ѵ�.
		struct Vin1stStatic_Individual
		{
			Vin1stStatic_Individual() = default;

			//�̰͸� �ް� �־�� �Ѵ�.
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh ����.
			DirectX::XMFLOAT2	_lightmapUV;
		};

		//Skinned Mesh�� �䱸�ϴ� ����. 
		struct Vin1stSkinned_Individual
		{
			Vin1stSkinned_Individual() = default;

			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh ����.

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

		//��� 3D Mesh���� 2nd Vertex Buffer. 
		struct Vin2ndAll_Individual
		{
			Vin2ndAll_Individual() = default;

			DirectX::XMFLOAT3	_normalL;
			DirectX::XMFLOAT3	_tangentL;
			DirectX::XMFLOAT3	_color;
		};

		//3rd�� ������ �ٸ���. �� �ν��Ͻ� vs �ν��Ͻ�.
		//�켱, ���ڴ� VertexCount�� ������ (���� ��)
		// ���ڴ� Instanced Object Count�� ������. (�� ���� �ν��Ͻ� ������ �ִ� ������Ʈ ����)
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
		//������ ���� ���̾ƿ���.
		//Alignment Padding�� �߻��ϸ� �ȵȴ�.
		//�ƽ�����, ���� �ٽ� �ۼ��ؾ� �� ��.
		struct Vin1stPassInstanced_Layout
		{
			//Vin1stStatic_Individual
			DirectX::XMFLOAT3	_posL;
			DirectX::XMFLOAT2	_tex;
			float		_meshMatID; //Multi-Mesh ����.
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
			float		_meshMatID; //Multi-Mesh ����.
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
			float		_meshMatID; //Multi-Mesh ����.

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
		//	float		_meshMatID; //Multi-Mesh ����.
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
		//	float		_meshMatID; //Multi-Mesh ����.
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
		//</������ ���� ���̾ƿ���>

		//<�ν��Ͻ� ���� ���̾ƿ� ������ ������ ���� �ʴ´�.>
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
		//</�ν��Ͻ� ���� ���̾ƿ� ������ ������ ���� �ʴ´�.>

		static void Initialize();

		//�ν��Ͻ��� ���̾ƿ� ��ȯ.
		static ID3D11InputLayout* GetInstanced1stLayout();
		
		//Static Mesh �䱸 ���̾ƿ� ��ȯ
		static ID3D11InputLayout* GetStatic1stLayout();

		//Skinned Mesh �䱸 ���̾ƿ� ��ȯ
		static ID3D11InputLayout* GetSkinned1stLayout();

		//2nd Layout ��ȯ
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
