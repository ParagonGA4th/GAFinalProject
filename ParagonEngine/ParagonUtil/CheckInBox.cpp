#include "CheckInBox.h"

Pg::Util::CheckInBox::CheckInBox()
{

}

bool Pg::Util::CheckInBox::IsIn3DBox(Pg::Math::PGFLOAT3 col1Transform, float width1, float height1, float depth1, Pg::Math::PGFLOAT3 col2Transform, float width2, float height2, float depth2)
{
	float halfWidth1 = width1 / 2;
	float halfHeight1 = height1 / 2;
	float halfDepth1 = depth1 / 2;

	float halfWidth2 = width2 / 2;
	float halfHeight2 = height2 / 2;
	float halfDepth2 = depth2 / 2;

	return (
		col1Transform.x - halfWidth1 < col2Transform.x + halfWidth2 &&
		col1Transform.x + halfWidth1 > col2Transform.x - halfWidth2 &&
		col1Transform.y - halfHeight1 < col2Transform.y + halfHeight2 &&
		col1Transform.y + halfHeight1 > col2Transform.y - halfHeight2 &&
		col1Transform.z - halfDepth1 < col2Transform.z + halfDepth2 &&
		col1Transform.z + halfDepth1 > col2Transform.z - halfDepth2
		);
}

PARAGON_UTIL_DLL bool Pg::Util::CheckInBox::IsIn2DBox(Pg::Math::PGFLOAT3 col1Transform, float width1, float height1, Pg::Math::PGFLOAT3 col2Transform, float width2, float height2)
{
	float halfWidth1 = width1 / 2;
	float halfHeight1 = height1 / 2;

	float halfWidth2 = width2 / 2;
	float halfHeight2 = height2 / 2;

	return (
		col1Transform.x - halfWidth1 < col2Transform.x + halfWidth2 &&
		col1Transform.x + halfWidth1 > col2Transform.x - halfWidth2 &&
		col1Transform.y - halfHeight1 < col2Transform.y + halfHeight2 &&
		col1Transform.y + halfHeight1 > col2Transform.y - halfHeight2 
		);
}
