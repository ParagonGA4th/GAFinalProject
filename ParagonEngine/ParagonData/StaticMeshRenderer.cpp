#include "StaticMeshRenderer.h"
#include "GameObject.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonUtil/InstancingException.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameConstantData.h"

#include <sstream>
#include <vector>
#include <algorithm>
#include <DirectXMath.h>
#include <singleton-cpp/singleton.h>

namespace Pg::Data
{
	StaticMeshRenderer::StaticMeshRenderer(GameObject* obj) : RendererBase3D(obj)
	{
		FACTORY_INIT;

		SetRendererTypeName(typeid(this).name());
	}

	void StaticMeshRenderer::ConvertPotentialUnrealValues()
	{
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
							str.erase(positions[3]);	// �̰� ���� ������Ʈ �̸� ȸ���ϴ� ��
							//str.erase(positions[1]); 
						}
					}
				}

				//�ٲ� ���� �ִ� �̸��� ����ִ´�.
				_object->SetName(str);
			}
		}
	}

	void StaticMeshRenderer::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<StaticMeshRenderer>(this, sv);
	}

	void StaticMeshRenderer::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<StaticMeshRenderer>(this, sv);
	}

	void StaticMeshRenderer::CheckForPathNameErrors()
	{
		//��������� Path�� ������ �۵��ϱ� ������ ������ ������, 
		//Name�� ��ϾȵǸ� ������ �ִ�.
		using Pg::Util::Helper::ResourceHelper;
		if (_meshName.empty())
		{
			assert(!_meshFilePath.empty() && "�̷��� ���ʿ� �������� �ʴ� ���� ���Դٴ� �Ҹ�.");
			assert(ResourceHelper::IsFileExist(_meshFilePath) && "��ȿ�� ���� ��ΰ� ����־�� �Ѵ�.");
			_meshName = ResourceHelper::GetNameFromPath(_meshFilePath);
		}

		//�ν��Ͻ� : XML���� ���.
		std::string tPrefixFromName = _meshName.substr(0, 5);
		std::string tPrefixFromNameOneLonger = _meshName.substr(0, 6);
		//Mesh Path Set / ���� Default Material�� �ƴ� ��� MaterialPath���� ��ġ �Ϸ�.
		bool tIsPartOfInstanceException = singleton<Pg::Util::InstancingException>().IsExceptionFromInstance(_meshFilePath);
		if (tPrefixFromName.compare(Pg::Defines::NON_INSTANCED_3DMODEL_PREFIX) == 0 || 
			tPrefixFromNameOneLonger.compare(Pg::Defines::BLENDED_OPTIONAL_3DMODEL_PREFIX) == 0 ||
			tIsPartOfInstanceException)
		{
			_isInstanced = false;
			_object->_transform._isCanMove = true;
		}
		else
		{
			//norm_�̳� blend_�� �������� �ʱ� ������, �ν��Ͻ��� �������� �����!
			_isInstanced = true;
			_object->_transform._isCanMove = false;

			//Clipping�Ǵ� �ֵ��� ��� �ν��Ͻ̿� ����� ���̴�. -> 
			if (tPrefixFromName.compare(Pg::Defines::CLIPPED_3DMODEL_PREFIX) == 0)
			{
				_isAlphaClipped = true;
			}
			else
			{
				_isAlphaClipped = false;
			}
		}
		//������� �ν��Ͻ�

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

	void StaticMeshRenderer::SetAlphaPercentage(float per)
	{
		_alphaPercentage = std::clamp<float>(per, 0.f, 100.f);
	}

	float StaticMeshRenderer::GetAlphaPercentage()
	{
		return _alphaPercentage;
	}


	//void StaticMeshRenderer::Update()
	//{
	//	using namespace DirectX;
	//
	//	Pg::Util::Input::InputSystem* tInput = (Pg::Util::Input::InputSystem*)_input;
	//	if (tInput->GetKeyDown(Pg::API::Input::KeyB))
	//	{
	//		DirectX::XMFLOAT4 tQuat = { _object->_transform._rotation.x, _object->_transform._rotation.y,
	//									_object->_transform._rotation.z, _object->_transform._rotation.w };
	//		DirectX::XMVECTOR tQuatVec = DirectX::XMLoadFloat4(&tQuat);
	//		XMVECTOR q = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XM_PI / 2.0f); // Rotation quaternion around X-axis by 90 degrees
	//		XMVECTOR tAns = XMQuaternionMultiply(tQuatVec, q);
	//		XMFLOAT4 tAnsFF;
	//		XMStoreFloat4(&tAnsFF, tAns);
	//		_object->_transform._rotation = { tAnsFF.w, tAnsFF.x, tAnsFF.y, tAnsFF.z };
	//	}
	//	if (tInput->GetKeyDown(Pg::API::Input::KeyN))
	//	{
	//		DirectX::XMFLOAT4 tQuat = { _object->_transform._rotation.x, _object->_transform._rotation.y,
	//									_object->_transform._rotation.z, _object->_transform._rotation.w };
	//		DirectX::XMVECTOR tQuatVec = DirectX::XMLoadFloat4(&tQuat);
	//		XMVECTOR q = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XM_PI / 2.0f); // Rotation quaternion around Y-axis by 90 degrees
	//		XMVECTOR tAns = XMQuaternionMultiply(tQuatVec, q);
	//		XMFLOAT4 tAnsFF;
	//		XMStoreFloat4(&tAnsFF, tAns);
	//		_object->_transform._rotation = { tAnsFF.w, tAnsFF.x, tAnsFF.y, tAnsFF.z };
	//	}
	//	if (tInput->GetKeyDown(Pg::API::Input::KeyM))
	//	{
	//		DirectX::XMFLOAT4 tQuat = { _object->_transform._rotation.x, _object->_transform._rotation.y,
	//									_object->_transform._rotation.z, _object->_transform._rotation.w };
	//		DirectX::XMVECTOR tQuatVec = DirectX::XMLoadFloat4(&tQuat);
	//		XMVECTOR q = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XM_PI / 2.0f); // Rotation quaternion around Z-axis by 90 degrees
	//		XMVECTOR tAns = XMQuaternionMultiply(tQuatVec, q);
	//		XMFLOAT4 tAnsFF;
	//		XMStoreFloat4(&tAnsFF, tAns);
	//		_object->_transform._rotation = { tAnsFF.w, tAnsFF.x, tAnsFF.y, tAnsFF.z };
	//	}
	//}

}