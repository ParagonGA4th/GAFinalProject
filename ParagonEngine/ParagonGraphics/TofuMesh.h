#pragma once

//#include "TofuMath.h"
#include <cstdint>
#include <DirectXMath.h>

namespace tofu
{
	struct Bone
	{
		float		matrix[12];
		int32_t		parent;
		int32_t		firstChild;
		int32_t		nextSibling;
		int32_t		name;
	};

	struct Mesh
	{
		float		matrix[12];
		uint32_t	startVertex;
		uint32_t	startIndex;
		uint32_t	numVertices;
		uint32_t	numIndices;
	};

	struct VectorFrame
	{
		DirectX::XMFLOAT3		value;
		float					time;
	};

	struct QuaternionFrame
	{
		DirectX::XMFLOAT4		value;
		float		time;
	};

	struct Track
	{
		uint32_t	transFrames;
		uint32_t	numTransFrames;
		uint32_t	rotFrames;
		uint32_t	numRotFrames;
		uint32_t	scaleFrames;
		uint32_t	numScaleFrames;
		uint32_t	_reserved1, _reserved2;
	};

	struct Animation
	{
		float		frameRate;
		float		duration;
		uint32_t	tracks;
		uint32_t	numTracks;
	};

	struct TFModel
	{
		uint32_t	magic;
		uint32_t	version;
		uint32_t	flags;
		uint32_t	vertexStart;
		uint32_t	meshStart;
		uint32_t	boneStart;
		uint32_t	animStart;
		uint32_t	stringStart;
	};
}