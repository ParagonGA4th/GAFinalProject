#pragma once
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <dxtk/SimpleMath.h>

/// <summary>
/// 리팩토링된, 새로 필요한 만큼만 Animation의 데이터를 들고 있을
/// 구조체들의 선언부.
/// 기존의 선언부를 결국 대체할 용도이다!
/// </summary>

namespace Pg::Graphics
{
	//NodeAnim에 활용, Vector Key (Animation)
	struct VectorKey_AssetData
	{
	public:
		//해당 Key의 발동 Keyframe.
		double _time;

		//Vector Key의 값.
		DirectX::SimpleMath::Vector3 _value;

		VectorKey_AssetData() noexcept : _time(0.0), _value() {}
		VectorKey_AssetData(double time, const DirectX::SimpleMath::Vector3& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Vector3;

		// std::find 사용 가능 오버로드 (일치 여부)
		bool operator==(const VectorKey_AssetData& rhs) const
		{
			return rhs._value == this->_value;
		}
		bool operator!=(const VectorKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort 사용 가능 오버로드 (대소 관계)
		bool operator<(const VectorKey_AssetData& rhs) const {
			return _time < rhs._time;
		}

		bool operator>(const VectorKey_AssetData& rhs) const {
			return _time > rhs._time;
		}
	};

	//NodeAnim에 활용, Quaternion Key (Animation)
	struct QuatKey_AssetData
	{
	public:
		//해당 Key 발동 Keyframe.
		double _time;

		//해당 Key의 값.
		DirectX::SimpleMath::Quaternion _value;

		QuatKey_AssetData() noexcept : _time(0.0), _value() {}
		QuatKey_AssetData(double time, const DirectX::SimpleMath::Quaternion& value) :
			_time(time), _value(value) {}

		using elem_type = DirectX::SimpleMath::Quaternion;

		// std::find 사용 가능 오버로드 (일치 여부)
		bool operator==(const QuatKey_AssetData& rhs) const {
			return rhs._value == this->_value;
		}

		bool operator!=(const QuatKey_AssetData& rhs) const {
			return rhs._value != this->_value;
		}

		// std::sort 사용 가능 오버로드 (대소 관계)
		bool operator<(const QuatKey_AssetData& rhs) const {
			return _time < rhs._time;
		}

		bool operator>(const QuatKey_AssetData& rhs) const {
			return _time > rhs._time;
		}
	};

	struct NodeAnim_AssetData
	{
	public:
		NodeAnim_AssetData();
		~NodeAnim_AssetData();

		//자신이 매핑될 실제 Node의 이름.
		std::string _nodeName;

		//Position Key들의 개수.
		unsigned int _numPositionKeys = 0;

		//Position Key들의 리스트.
		std::vector<VectorKey_AssetData> _positionKeyList;

		//Rotation Key들의 개수.
		unsigned int _numRotationKeys = 0;

		//Rotation Key들의 리스트.
		std::vector<QuatKey_AssetData> _rotationKeyList;
	};

	//Animtion을 보관하기 위해 사용되는 구조체.
	struct Animation_AssetData
	{
	public:
		//Animation 이름.
		std::string _name;

		//Tick 기준 Animation의 길이.
		double _duration;

		//초 당 Tick의 개수.
		double _ticksPerSecond;

		//각각 하나의 Node에 영향을 미치는, NodeAnim Channel의 개수.
		unsigned int _numChannels;

		//각각 하나의 Node에 영향을 미치는, NodeAnim Channel들의 리스트.
		std::vector<std::unique_ptr<NodeAnim_AssetData>> _channelList;
	};

	//렌더될 때, Shader에 Bone들의 목록 활용 구조체.
	struct BoneInfo_AssetData
	{
		BoneInfo_AssetData();
		DirectX::SimpleMath::Matrix _finalTransformation; // Final transformation to apply to vertices 
		DirectX::SimpleMath::Matrix _boneOffset; // Initial offset from local to bone space. 
	};
}
