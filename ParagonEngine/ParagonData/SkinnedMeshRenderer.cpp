#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonData/AnimTransform.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/Transform.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include <cassert>
#include <sstream>
#include <vector>
#include <algorithm>

namespace Pg::Data
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* obj) 
		: RendererBase3D(obj)
		//_animBlendFactor(10.f), _alphaPercentage(50.f)
	{
		SetRendererTypeName(typeid(this).name());
	}

	void SkinnedMeshRenderer::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<SkinnedMeshRenderer>(this, sv);
	}

	void SkinnedMeshRenderer::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<SkinnedMeshRenderer>(this, sv);
	}

	void SkinnedMeshRenderer::Internal_EngineAwake()
	{
		assert(!_initAnimName.empty() && "ó���� �� Animation�� ��������� �ȵȴ�");
		_setAnimationFunction(_initAnimName, true); //�⺻������ ó�� �ִϸ��̼��� Loop�Ѵٰ� �����Ѵ�.
		_currentAnim = _initAnimName;
	}

	void SkinnedMeshRenderer::ConvertPotentialUnrealValues()
	{
		//��¿ �� ���� Property ���縦 ���� �ڵ� �ߺ�.
		//�Ŀ� ȣ��� ����.
		//�ϴ��� �𸮾� �� FBX ��� ��������, ��ü������ ���� �־������� �ľ��Ѵ�.
		std::string segment;
		std::vector<std::string> seglist;
		std::stringstream test(_meshName);

		//���� ������ �޴´�.
		while (std::getline(test, segment, '.'))
		{
			seglist.push_back(segment);
		}

		//�𸮾� ������ / ��ü������ ������ ������� Ȯ���Ѵ�.
		assert(seglist.size() == 2 && "�𸮾�/��ü ����ʿ��� ��ȿ���� �ʴ� �����̴�.");
		if (seglist[0].compare(seglist[1]) == 0)
		{
			// �𸮾� ���.
			std::string tRes = seglist[0];
			//FBX�� ���� ���ο��� ����� �����̱⿡.
			tRes.append(".fbx");

			//�ϴ� ���� Ȱ���� _meshName�� �ִ´�.
			_meshName = tRes;
		}

		//"fromUnrealExample"�� �𸮾��̶�� ����.
		//���� materialName�� �ش� ���̾����� materialName�� �����. �̸� ������� �Ǵ��� ��.
		if (_materialName.compare("fromUnrealExample") == 0)
		{
			{
				_materialName.clear();

				//Unreal Coordinate System�� ���� Solution. (�޼� / ������ ��ǥ�� ȥ��)
				Pg::Math::PGQuaternion tTemp = _object->_transform._rotation;

				//Ʈ������ ������.
				_object->_transform._rotation.x = tTemp.x * -1.0f;
				_object->_transform._rotation.y = tTemp.z;
				_object->_transform._rotation.z = tTemp.y;
				_object->_transform._rotation.w = tTemp.w;
			}
			{
				//�𸮾󿡼� �Դ� �ͽ����͸� �޴� ������ ���⼭ �ؾ�. (������ ��ġ�� �ʴ� ���̺� �ݺ�)
				std::string str = _object->GetName();
				std::vector<std::size_t> positions;

				// ������ _�� ���縦 ã�´�.
				std::size_t pos = str.length();
				while ((pos = str.rfind('_', pos - 1)) != std::string::npos)
				{
					positions.push_back(pos);
				}

				if (!positions.empty())
				{
					//positions��ŭ ��ġ�� ã�� ��.
					if (positions.size() >= 4)
					{
						std::string tFirst = str.substr(positions[1]);
						std::string tSecond = str.substr(positions[3], positions[1] - positions[3]);

						if (tFirst.compare(tSecond) == 0)
						{
							//�̷��� ���� ��ġ�Ѵٴ� ��. 
							str.erase(positions[1]);
						}
					}
				}

				//�ٲ� ���� �ִ� �̸��� ����ִ´�.
				_object->SetName(str);
			}
		}
	}

	void SkinnedMeshRenderer::CheckForPathNameErrors()
	{
		//��¿ �� ���� Property ���縦 ���� �ڵ� �ߺ�.
		//��������� Path�� ������ �۵��ϱ� ������ ������ ������, 
		//Name�� ��ϾȵǸ� ������ �ִ�.
		using Pg::Util::Helper::ResourceHelper;
		if (_meshName.empty())
		{
			assert(!_meshFilePath.empty() && "�̷��� ���ʿ� �������� �ʴ� ���� ���Դٴ� �Ҹ�.");
			assert(ResourceHelper::IsFileExist(_meshFilePath) && "��ȿ�� ���� ��ΰ� ����־�� �Ѵ�.");
			_meshName = ResourceHelper::GetNameFromPath(_meshFilePath);
		}

		if (_materialName.empty())
		{
			if (_renderMaterialPath.empty())
			{
				return;
			}
			//���⼭�� ��ȿ�� Material Path �˻縦 �� �� ����. Default Material�� ��� Path�� ���� ����!
			_materialName = ResourceHelper::GetNameFromPath(_renderMaterialPath);
		}
	}

	std::string SkinnedMeshRenderer::GetAnimation()
	{
		return _currentAnim;
	}

	void SkinnedMeshRenderer::SetAnimation(const std::string& animName, bool isLoop)
	{
		//�ܼ��� ���翩�θ� �ľ�
		if (!_setAnimationFunction)
		{
			std::string tPrint = "SetAnimation Impl not yet bound in : ";
			tPrint.append(animName);

			PG_WARN(tPrint.c_str());
		}
		//���� ������ ������ �� ��. 
		_currentAnim = animName;
		//����� �� ���� ���θ� Ȯ��.
		_setAnimationFunction(animName, isLoop);
	}

	Pg::Data::AnimTransform* SkinnedMeshRenderer::FindAnimTransform(const std::string& animNodeName)
	{
		if (!_findAnimTransformFunction)
		{
			std::string tPrint = "FindAnimTransfrom Impl not yet bound with : ";
			tPrint.append(animNodeName);
			PG_WARN(tPrint.c_str());
		}

		return _findAnimTransformFunction(animNodeName);
	}

	void SkinnedMeshRenderer::SetAlphaPercentage(float per)
	{
		_alphaPercentage = std::clamp<float>(per, 0.f, 100.f);
	}

	float SkinnedMeshRenderer::GetAlphaPercentage()
	{
		return _alphaPercentage;
	}

	void SkinnedMeshRenderer::PauseAnim()
	{
		_playVariable = 0;
	}

	void SkinnedMeshRenderer::PlayAnim()
	{
		_playVariable = 1;
	}

	void SkinnedMeshRenderer::SetanimBlendFactor(float per)
	{
		_animBlendFactor = per;
	}

	float SkinnedMeshRenderer::GetanimBlendFactor()
	{
		return _animBlendFactor;
	}
}