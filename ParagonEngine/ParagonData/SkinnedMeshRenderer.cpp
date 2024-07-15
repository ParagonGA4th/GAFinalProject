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
		assert(!_initAnimName.empty() && "처음에 들어갈 Animation이 비어있으면 안된다");
		_setAnimationFunction(_initAnimName, true); //기본적으로 처음 애니메이션은 Loop한다고 가정한다.
		_currentAnim = _initAnimName;
	}

	void SkinnedMeshRenderer::ConvertPotentialUnrealValues()
	{
		//어쩔 수 없이 Property 복사를 위해 코드 중복.
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

	void SkinnedMeshRenderer::CheckForPathNameErrors()
	{
		//어쩔 수 없이 Property 복사를 위해 코드 중복.
		//결과적으로 Path만 있으면 작동하기 때문에 문제는 없지만, 
		//Name이 기록안되면 문제가 있다.
		using Pg::Util::Helper::ResourceHelper;
		if (_meshName.empty())
		{
			assert(!_meshFilePath.empty() && "이러면 애초에 동작하지 않는 값이 들어왔다는 소리.");
			assert(ResourceHelper::IsFileExist(_meshFilePath) && "유효한 파일 경로가 들어있어야 한다.");
			_meshName = ResourceHelper::GetNameFromPath(_meshFilePath);
		}

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

	std::string SkinnedMeshRenderer::GetAnimation()
	{
		return _currentAnim;
	}

	void SkinnedMeshRenderer::SetAnimation(const std::string& animName, bool isLoop)
	{
		//단순한 존재여부를 파악
		if (!_setAnimationFunction)
		{
			std::string tPrint = "SetAnimation Impl not yet bound in : ";
			tPrint.append(animName);

			PG_WARN(tPrint.c_str());
		}
		//위에 있으면 에러가 날 것. 
		_currentAnim = animName;
		//제대로 된 실행 여부를 확인.
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