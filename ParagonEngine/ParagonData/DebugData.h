#pragma once
#include "../ParagonMath/PgMath.h"
#include <vector>
#include <string>

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

}