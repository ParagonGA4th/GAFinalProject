#include <string>

struct PositionData
{
	float _x;
	float _y;
	float _z;
};

struct RotationData
{
	float _w;
	float _x;
	float _y;
	float _z;
};

struct ScaleData
{
	float _x;
	float _y;
	float _z;
};

struct TransformData
{
	PositionData _position;
	RotationData _rotation;
	ScaleData _scale;
}; 

struct GameObjectData
{
	int _objectNumber;
	std::string _name;
	std::string _tag;
	bool _active;
	int _parent;
	std::string _type;

	TransformData _transform;
};