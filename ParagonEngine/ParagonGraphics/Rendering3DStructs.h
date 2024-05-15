#pragma once
#include "RenderObjectInstancedMesh3D.h"
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderMaterial.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/CameraData.h"
#include "SingleLightmapSet.h"
///RenderObject3DList에 쓰이는 구조체 리스트.

//Alpha Blending되는 오브젝트들을 위한 Tuple. 부하를 키우지 않기 위해 따로 구조체 만듬.
//또한 나중에 Sorting이 쉽게 되기 위해.
namespace Pg::Graphics
{
	struct AlphaBlendedTuple
	{
		AlphaBlendedTuple(Pg::Data::GameObject* go, RenderMaterial* mat, bool isSkinned) :
			_obj(go), _renderMat(mat), _isSkinned(isSkinned) {}

		//std::sort가 강제로 Copy가 아니라 Move Semantics에 의해 동작할 수 있게.
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
		//Move만 허용할 것.

		//Sorting. : Operator Overload.
		bool operator>(const AlphaBlendedTuple& rhs) const {
			return this->_cameraRelativeDistSquared > rhs._cameraRelativeDistSquared;
		}

		bool operator<(const AlphaBlendedTuple& rhs) const {
			return this->_cameraRelativeDistSquared < rhs._cameraRelativeDistSquared;
		}

		//<불변 데이터>
		const Pg::Data::GameObject* _obj;
		const RenderMaterial* _renderMat;
		bool _isSkinned; //이 값을 따라서 어떤 값을 렌더해야 할지가 달라진다.

		std::unique_ptr<RenderObjectStaticMesh3D> _eitherStaticMesh;
		std::unique_ptr<RenderObjectSkinnedMesh3D> _eitherSkinnedMesh;
		//</불변 데이터>

		//매 프레임 변하는 데이터: BACK_TO_FRONT 되어야 한다. (>) 오버로드해야. 
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

	//이걸 New 할 것이다.
	struct BufferInstancedPairList
	{
		ID3D11Buffer* _vb{ nullptr };
		std::vector<InstancedStaticPair> _instancedStaticPairVec;
		//위의 인덱스와 대응, 내부의 SV_InstanceID와 연동해서 이렇게 갈 것이다.
		std::vector<SingleLightMapSet> _instancedLightMapSetVec;
	};
}