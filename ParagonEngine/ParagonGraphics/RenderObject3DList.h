#pragma once
#include "RenderObject3D.h"
#include "RenderObjectStaticMesh3D.h"
#include "RenderObjectSkinnedMesh3D.h"
#include "RenderMaterial.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/CameraData.h"

#include <unordered_map>
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include <limits>

/// <summary>
/// Paragon Renderer가 Render되는 오브젝트들을 들고 있을 용도로,
/// 만들어진 컨테이너용 클래스 (3D)
/// </summary>

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
		const RenderMaterial*  _renderMat;
		bool _isSkinned; //이 값을 따라서 어떤 값을 렌더해야 할지가 달라진다.

		std::unique_ptr<RenderObjectStaticMesh3D> _eitherStaticMesh;
		std::unique_ptr<RenderObjectSkinnedMesh3D> _eitherSkinnedMesh;
		//</불변 데이터>

		//매 프레임 변하는 데이터: BACK_TO_FRONT 되어야 한다. (>) 오버로드해야. 
		float _cameraRelativeDistSquared{ std::numeric_limits<float>::max() };
	};
}

namespace Pg::Graphics
{
	struct RenderObject3DList
	{
	public:

		std::vector<RenderObject3D*> GetRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		void DeleteRenderObjectWithGameObject(Pg::Data::GameObject* obj);
		//더 큰 거일수록 뒤에 가야 한다. 
		//먼저 그려져야 하니!
		void SortBlendedByDepth_BackToFront(Pg::Data::CameraData* camData);


		//일단은 Material Index와 관계는 없을 것이다. -> 이는 현재로서는 Opaque에 한정.
		std::unordered_map<std::string, RenderMaterial*> _materialPathSet;

		//FirstPath가 지나면 Static, Skinned의 차이가 없다.
		//FirstStatic/Skinned에 한정해서 차이나게 기록하고, 나머지는 일괄적으로 렌더해버리자.

		//Static - Opaque.
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectStaticMesh3D>>>>> _staticList; /* Vector */

		//Skinned - Opaque.
		std::unordered_map<RenderMaterial*, /* Material Path */
			std::unique_ptr<std::vector<std::pair<Pg::Data::GameObject*, std::unique_ptr<RenderObjectSkinnedMesh3D>>>>> _skinnedList; /* Vector */

		//얘네는 이제 소팅이 되어야 한다.
		//얘는 Forward로 렌더링해야 한다.
		//더 이상 RenderMaterial 단위는 안됨.
		//이제 덮어씌운다! -> 스킨드도 따로 떨어져 있으면 안됨.
		//전에 바인딩된 Material을 보관한 다음에, => 전에 바인딩이 안되었으면 바인딩하는 방식을 쓰자.
		//물리적으로 GameObject의 순서를 바꿔서 소팅하기보다는, 이들의 인덱스를 받아서 인덱스 순서대로 출력하는 방식을 사용하자.
		//중요한게, 오브젝트가 섞여 있다면, 어느 애들은 기존 BlendState로 렌더링하고, 어느 애들은 AlphaBlending 스테이트 대로. 

		//Static & Skinned - AlphaBlended.
		std::vector<std::unique_ptr<AlphaBlendedTuple>> _allAlphaBlendedList;
		
		//Index를 따로 보관할 것이기에.
		std::vector<int> _sortedIndexBlendedVec;
		///같은 인덱스의 수가 가진 벡터와 대응 순열이 있다. 

	};

}