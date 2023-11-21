#pragma once
#include "../ParagonMath/PgMath.h"
#include "../ParagonProcess/CoreSingleton.h"
#include "../ParagonData/DebugData.h"
#include <vector>
#include <string>

/// <summary>
/// 변지상의 DebugSystem.
/// 기본적으로 Collider의 Debug를 띄울 예정이다.
/// 2023.11.09
/// </summary>
namespace Pg::Engine
{
	using namespace Pg::Math;

	class DebugSystem
	{
	public:
		void Initialize();
		void Finalize();

	public:
		//디버그 정보를 벡터에 저장.
		void DrawBoxDebug(PGFLOAT4X4 worldTM, PGFLOAT3 scale, PGFLOAT4 color);
		void DrawLineDebug(PGFLOAT3 beginPoint, PGFLOAT3 endPoint, PGFLOAT4 color);

		void DeleteBoxDebug();

		void SetDebugMode(bool isdebug);

	public:
		std::vector<Pg::Data::BoxInfo>& GetBoxVector() const;
		std::vector<Pg::Data::LineInfo>& GetLineVector();

		//std::vector<Pg::Data::BoxInfo*>& GetDrawEnabledBoxVector() const;
		//std::vector<Pg::Data::LineInfo*>& GetDrawEnabledLineVector();
	private:
		std::vector<Pg::Data::BoxInfo> _boxVec;
		std::vector<Pg::Data::LineInfo> _lineVec;

		//std::vector<Pg::Data::BoxInfo*> _boxVecChosen;
		//std::vector<Pg::Data::LineInfo*> _lineVecChosen;

		//디버그 모드 플래그
		bool _isDebug = false;
	};

}

