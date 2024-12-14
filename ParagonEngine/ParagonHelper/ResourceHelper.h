#pragma once
#include "../ParagonData/AssetDefines.h"
#include <map>
#include <string>

/// <summary>
/// AssetManager�� �����ִ� ���ҽ� ����, ���ҽ� ������ ������ �ش�.
/// ���ҽ��� �߰��Ǹ鼭, ������ ��� ������Ʈ�Ǿ�� �Ѵ�.
/// </summary>

namespace Pg::Util::Helper
{
	class ResourceHelper
	{
	public:
		ResourceHelper();
		~ResourceHelper();

		//���ҽ��� �ش� ��ġ�� �����ϴ��� �˷��ִ� Wrapper �Լ�.
		 static bool IsFileExist(const std::string& filePath);

		//�پ��� ����� ��� ��� ����� �ϳ��� ������� ������ ������ �������ִ� �Լ��̴�. 
		 static std::string ForcePathUniform(const std::string& filePath);

		//ForcePathUniform + Relative Path�� ��� Full Path�� ������ش�.
		 static std::string ForcePathUniformFull(const std::string& filePath);

		//eAssetDefine�� Graphics ���ҽ� �Ҽ�����, Engine ���ҽ� �Ҽ����� �������ش�. (���ҽ��� �߰��Ǹ鼭 ���� ������Ʈ�Ǿ�� �� �Լ�)
		 static bool IsGraphicsResource(Pg::Data::Enums::eAssetDefine define);

		//�� ��ü�� : ������ �� ������ <-1> / 2D <0> / 3D <1> ������ �������ش�.
		//IsGraphicsResource�� �����Ѵ�.
		 static short IsPlainRenderable(Pg::Data::Enums::eAssetDefine define);

		 //���ҽ��� Ȯ���ڰ� DDS����, �ƴ����� �˷��ش�.
		 static bool IsResourceDDS(const std::string& filePath);
		 static bool IsResourceDDS(const std::wstring& filePath);

		 //���ҽ��� Ȯ���ڰ� TGA����, �ƴ����� �˷��ش�.
		 static bool IsResourceTGA(const std::string& filePath);
		 static bool IsResourceTGA(const std::wstring& filePath);

		 //���ҽ��� Ȯ���ڰ� EXR���� (Unity Lightmap ȣȯ ����), �ƴ����� �˷��ش�.
		 static bool IsResourceEXR(const std::string& filePath);
		 static bool IsResourceEXR(const std::wstring& filePath);

		 static std::string GetNameFromPath(const std::string& filePath);

		 //����� / ������ ��带 �����ؼ�, ���� ������ ����� ��� Debug->Release�� String ���� �ٲ۴�.
		 static std::string IfReleaseChangeDebugText(const std::string& filePath);
		 static std::wstring IfReleaseChangeDebugTextW(const std::wstring& filePath);
	private:

	};
}


