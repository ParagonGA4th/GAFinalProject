#include "DebugSystem.h"

namespace Pg::Engine
{

	void DebugSystem::Initialize()
	{

	}

	void DebugSystem::Finalize()
	{

	}

	void DebugSystem::SetDebugMode(bool isdebug)
	{
		_isDebug = isdebug;
	}

	void DebugSystem::DrawBoxDebug(PGFLOAT4X4 worldTM, PGFLOAT3 scale, PGFLOAT4 color)
	{
		if (!_isDebug)
		{
			return;
		}

		_boxVec.push_back({ worldTM, scale, color });
	}

	void DebugSystem::DrawLineDebug(PGFLOAT3 beginPoint, PGFLOAT3 endPoint, PGFLOAT4 color)
	{

	}

	void DebugSystem::DeleteBoxDebug()
	{
		_boxVec.clear();
	}

	std::vector<Pg::Data::BoxInfo>& DebugSystem::GetBoxVector()
	{
		return _boxVec;
	}

	std::vector<Pg::Data::LineInfo>& DebugSystem::GetLineVector()
	{
		return _lineVec;
	}

}