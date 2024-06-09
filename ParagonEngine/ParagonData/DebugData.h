#pragma once
#pragma warning(disable:4267) 
#include "../ParagonMath/PgMath.h"
#include <vector>
#include <string>
#include <algorithm>

/// <summary>
/// 변지상의 Collider의 디버그 정보를 담은 구조체...
/// 그래픽스로 전달하여 렌더링 할 예정.
/// </summary>
namespace Pg::Data
{
	using namespace Pg::Math;

	//World TM에는 Scale 관련 정보가 들어오지 않는다.
	struct BoxInfo
	{
		PGFLOAT4X4 worldTM;
		PGFLOAT3 scale;
		PGFLOAT4 color;
	};

	//버튼 디버그.
	struct Box2DInfo
	{
		PGFLOAT2 LT;
		PGFLOAT2 RB;
		PGFLOAT4 color;
	};

	struct LineInfo
	{
		PGFLOAT3 beginPoint;
		PGFLOAT3 endPoint;
		PGFLOAT4 color;
	};

	//똑같은 Line의 형태이지만, 필요한 변수와 활용도가 다르기에 구분. 
	struct RayCastInfo
	{
		//맞았을 때
		bool isHit;
		PGFLOAT3 hitPoint;

		//안 맞았을 때
		PGFLOAT3 origin;
		PGFLOAT3 dir;
		float length;
	};

	struct SphereInfo
	{
		PGFLOAT4X4 worldTM;
		PGFLOAT3 scale;
		PGFLOAT4 color;
	};

	struct CapsuleInfo
	{
		PGFLOAT4X4 worldTM;
		PGFLOAT3 scale;
		PGFLOAT4 color;
	};

	struct PlaneInfo
	{
		PGFLOAT4X4 worldTM;
		PGFLOAT3 scale;
		PGFLOAT4 color;
	};

	///Recast에서 빌드되는 Mesh의 정보들.
	struct NavMeshInfo
	{
		std::string path;
		std::vector<PGFLOAT3>* vertices;
		std::vector<unsigned int>* indices;
	};

	//나중에 NavMeshAgent 디버깅에 쓰일 애. 좌표계 바꿔서 들어와야.
	struct NavCylinderInfo
	{
	public:
		NavCylinderInfo(const std::vector<std::pair< PGFLOAT3, PGFLOAT3>>& seVec)
			: _seVec(seVec), _totalLineSize(seVec.size()), _currentlyLocatedPosition(seVec.at(0).first)
		{}

		//Start End Vector. Lines.
		std::vector<std::pair<PGFLOAT3, PGFLOAT3>> _seVec;

		float _radius{ 1.0f };
		float _height{ 1.0f };

		//DebugRenderer를 위한 Getter / Setter.
		unsigned int GetMovingIndex() { return _currentMovingIndex; }
		PGFLOAT3 GetCurrentPosition() { return _currentlyLocatedPosition; }

		void IncrementMovingIndex() 
		{
			if (_currentMovingIndex + 1 > _totalLineSize - 1)
			{
				_currentMovingIndex = 0;
			}
			else
			{
				_currentMovingIndex++;
			}
		}
		void SetCurrentPosition(PGFLOAT3 val) { _currentlyLocatedPosition = val; }
		void SetCurrentRatio(float ratio)
		{
			_currentRatio = std::clamp<float>(ratio, 0.f, 1.f);
		}
		float GetCurrentRatio() { return _currentRatio; }


		//정보 입력용이 아니다. 내부적으로 어디까지 도달했는지를 보기 위해서이다.
	private:
		float _currentRatio{ 0.f };
		unsigned int _currentMovingIndex{ 0 };
		PGFLOAT3 _currentlyLocatedPosition;
		unsigned int _totalLineSize;
	};

}