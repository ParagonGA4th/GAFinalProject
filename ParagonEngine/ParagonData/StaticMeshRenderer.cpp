#include "StaticMeshRenderer.h"
#include "GameObject.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "../ParagonUtil/InstancingException.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/GameConstantData.h"

#include <sstream>
#include <vector>
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
		//후에 호출될 예정.
		//일단은 언리얼 식 FBX 기록 형식인지, 자체적으로 값을 넣었는지를 파악한다.
		std::string segment;
		std::vector<std::string> seglist;
		std::stringstream test(_meshName);

		//값을 나눠서 받는다.
		while (std::getline(test, segment, '.'))
		{
			seglist.push_back(segment);
		}

		//언리얼 식인지 / 자체적으로 가능한 양식인지 확인한다.
		assert(seglist.size() == 2 && "언리얼/자체 어느쪽에도 유효하지 않는 포맷이다.");
		if (seglist[0].compare(seglist[1]) == 0)
		{
			// 언리얼 양식.
			std::string tRes = seglist[0];
			//FBX만 엔진 내부에서 사용할 예정이기에.
			tRes.append(".fbx");

			//일단 값을 활용해 _meshName에 넣는다.
			_meshName = tRes;
		}

		//"fromUnrealExample"은 언리얼이라는 증거.
		//만약 materialName이 해당 값이었으면 materialName을 비우자. 이를 기반으로 판단할 것.
		if (_materialName.compare("fromUnrealExample") == 0)
		{
			{
				_materialName.clear();

				//Unreal Coordinate System에 대한 Solution. (왼손 / 오른손 좌표계 혼용)
				Pg::Math::PGQuaternion tTemp = _object->_transform._rotation;

				//트랜스폼 돌리기.
				_object->_transform._rotation.x = tTemp.x * -1.0f;
				_object->_transform._rotation.y = tTemp.z;
				_object->_transform._rotation.z = tTemp.y;
				_object->_transform._rotation.w = tTemp.w;
			}
			{
				//언리얼에서 왔다 익스포터를 받는 수정을 여기서 해야. (마지막 겹치지 않는 레이블링 반복)
				std::string str = _object->GetName();
				std::vector<std::size_t> positions;

				// 끝부터 _의 존재를 찾는다.
				std::size_t pos = str.length();
				while ((pos = str.rfind('_', pos - 1)) != std::string::npos)
				{
					positions.push_back(pos);
				}

				if (!positions.empty())
				{
					//positions만큼 위치를 찾은 것.
					if (positions.size() >= 4)
					{
						std::string tFirst = str.substr(positions[1]);
						std::string tSecond = str.substr(positions[3], positions[1] - positions[3]);

						if (tFirst.compare(tSecond) == 0)
						{
							//이러면 값이 일치한다는 것. 
							str.erase(positions[1]);
						}
					}
				}

				//바뀔 수도 있는 이름을 집어넣는다.
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
		//결과적으로 Path만 있으면 작동하기 때문에 문제는 없지만, 
		//Name이 기록안되면 문제가 있다.
		using Pg::Util::Helper::ResourceHelper;
		if (_meshName.empty())
		{
			assert(!_meshFilePath.empty() && "이러면 애초에 동작하지 않는 값이 들어왔다는 소리.");
			assert(ResourceHelper::IsFileExist(_meshFilePath) && "유효한 파일 경로가 들어있어야 한다.");
			_meshName = ResourceHelper::GetNameFromPath(_meshFilePath);
		}

		//인스턴싱 : XML에서 기록.
		std::string tPrefixFromName = _meshName.substr(0, 5);
		//Mesh Path Set / 만약 Default Material이 아닌 경우 MaterialPath까지 배치 완료.
		bool tIsPartOfInstanceException = singleton<Pg::Util::InstancingException>().IsExceptionFromInstance(_meshFilePath);
		if (tPrefixFromName.compare(Pg::Defines::NON_INSTANCED_3DMODEL_PREFIX) == 0 || tIsPartOfInstanceException)
		{
			_isInstanced = false;
			_object->_transform._isCanMove = true;
		}
		else
		{
			//norm_으로 시작하지 않기 때문에, 인스턴스된 렌더링이 적용됨!
			_isInstanced = true;
			_object->_transform._isCanMove = false;
		}
		//여기까지 인스턴싱

		if (_materialName.empty())
		{
			if (_renderMaterialPath.empty())
			{
				return;
			}
			//여기서는 유효한 Material Path 검사를 할 수 없다. Default Material의 경우 Path가 없기 때문!
			_materialName = ResourceHelper::GetNameFromPath(_renderMaterialPath);
		}
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