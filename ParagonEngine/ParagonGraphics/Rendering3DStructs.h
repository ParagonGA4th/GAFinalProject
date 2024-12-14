#pragma once
#include "RenderObjectInstancedMesh3D.h"
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderMaterial.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/CameraData.h"
#include "SingleLightmapSet.h"
///RenderObject3DList�� ���̴� ����ü ����Ʈ.

//Alpha Blending�Ǵ� ������Ʈ���� ���� Tuple. ���ϸ� Ű���� �ʱ� ���� ���� ����ü ����.
//���� ���߿� Sorting�� ���� �Ǳ� ����.
namespace Pg::Graphics
{
	struct AlphaBlendedTuple
	{
		AlphaBlendedTuple(Pg::Data::GameObject* go, RenderMaterial* mat, bool isSkinned) :
			_obj(go), _renderMat(mat), _isSkinned(isSkinned) {}
		~AlphaBlendedTuple() 
		{
		}

		//std::sort�� ������ Copy�� �ƴ϶� Move Semantics�� ���� ������ �� �ְ�.
		AlphaBlendedTuple(const AlphaBlendedTuple& rhs) = delete;
		AlphaBlendedTuple& operator=(const AlphaBlendedTuple& rhs) = delete;

		AlphaBlendedTuple(AlphaBlendedTuple&& other) noexcept
			: _obj(other._obj), _renderMat(other._renderMat), _isSkinned(other._isSkinned)
		{
			this->_eitherStaticMesh = std::move(other._eitherStaticMesh);
			this->_eitherSkinnedMesh = std::move(other._eitherSkinnedMesh);
			_cameraRelativeDistSquared = other._cameraRelativeDistSquared;
		}

		AlphaBlendedTuple& operator=(AlphaBlendedTuple&& other) noexcept
		{
			this->_obj = other._obj;
			this->_renderMat = other._renderMat;
			this->_isSkinned = other._isSkinned;

			this->_eitherStaticMesh = std::move(other._eitherStaticMesh);
			this->_eitherSkinnedMesh = std::move(other._eitherSkinnedMesh);
			this->_cameraRelativeDistSquared = other._cameraRelativeDistSquared;

			return *this;
		}
		//Move�� ����� ��.

		//Sorting. : Operator Overload.
		bool operator>(const AlphaBlendedTuple& rhs) const {
			return this->_cameraRelativeDistSquared > rhs._cameraRelativeDistSquared;
		}

		bool operator<(const AlphaBlendedTuple& rhs) const {
			return this->_cameraRelativeDistSquared < rhs._cameraRelativeDistSquared;
		}

		//<�Һ� ������>
		const Pg::Data::GameObject* _obj;
		RenderMaterial* _renderMat;
		bool _isSkinned; //�� ���� ���� � ���� �����ؾ� ������ �޶�����.

		std::unique_ptr<RenderObjectStaticMesh3D> _eitherStaticMesh;
		std::unique_ptr<RenderObjectSkinnedMesh3D> _eitherSkinnedMesh;
		//</�Һ� ������>

		//�� ������ ���ϴ� ������: BACK_TO_FRONT �Ǿ�� �Ѵ�. (>) �����ε��ؾ�. 
		float _cameraRelativeDistSquared{ std::numeric_limits<float>::max() };
	};

	struct InstancedStaticPair
	{
		InstancedStaticPair(RenderMaterial* mat, std::shared_ptr<RenderObjectInstancedMesh3D> ro)
			: _renderMaterial(mat), _instancedRenderObject(ro) {}
		InstancedStaticPair(const InstancedStaticPair& rhs)
		{
			_renderMaterial = rhs._renderMaterial;
			_instancedRenderObject = rhs._instancedRenderObject;
			_instanceVB = rhs._instanceVB;
		}

		RenderMaterial* _renderMaterial{ nullptr };
		std::shared_ptr<RenderObjectInstancedMesh3D> _instancedRenderObject;
		ID3D11Buffer* _instanceVB{ nullptr };
	};

	//�̰� New �� ���̴�.
	struct BufferInstancedPairList
	{
		ID3D11Buffer* _vb{ nullptr };
		std::vector<InstancedStaticPair> _instancedStaticPairVec;
		//���� �ε����� ����, ������ SV_InstanceID�� �����ؼ� �̷��� �� ���̴�.
		std::vector<SingleLightMapSet> _instancedLightMapSetVec;
	};
}