#pragma once
#include "../ParagonMath/PgMath.h"
#include "AssetDefines.h"
#include <string>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

/// <summary>
/// IRenderInfo�� ������Ʈ�ϱ� ����, 
/// �������� ������Ʈ�� �� �ʿ��� ���� ��Ʈ�� ��� ���� �����ϴ� ����ü.
/// ������ �� �ʿ��� ������ �����Ѵ�.
/// (*) Shader, Material, 3DModelData�� ������ ��.
/// </summary>

namespace Pg::Data
{
	using Pg::Data::Enums::eAssetDefine;

	struct RenderTextData
	{
		RenderTextData();

		//� ������ ���ҽ������� ����Ѵ�.
		eAssetDefine _assetDefine;

		//FLOAT3, ���� ������Ʈ�� ��ġ�� ������.
		Pg::Math::PGFLOAT3 position;
		//���ʹϾ����� ȸ�� ���� ����.
		Pg::Math::PGQuaternion rotation;
		//FLOAT3, ���� ������Ʈ�� ��ġ�� ������.
		Pg::Math::PGFLOAT3 scale;
		
		//���� ����ϴ� Visual Resource�� ���.
		std::string resourcePath;
		//std::string materialPath; //TBA
	};

	inline RenderTextData::RenderTextData() :
		_assetDefine(eAssetDefine::_NONE), position(0.f, 0.f, 0.f),
		rotation(0.f, 0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), resourcePath()
	{
		
	}

}
