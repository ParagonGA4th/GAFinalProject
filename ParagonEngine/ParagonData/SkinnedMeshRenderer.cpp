#include "SkinnedMeshRenderer.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonHelper/ResourceHelper.h"

#include <cassert>
#include <sstream>
#include <vector>

namespace Pg::Data
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(GameObject* obj) : RendererBase3D(obj)
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
			_materialName.clear();
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

	void SkinnedMeshRenderer::SetAnimation(const std::string& animName)
	{
		//단순한 존재여부를 파악
		if (!_setAnimationFunction)
		{
			std::string tPrint = "SetAnimation Impl not yet bound in : ";
			tPrint.append(animName);
			PG_WARN(tPrint.c_str());
		}
		//제대로 된 실행 여부를 확인.
		_setAnimationFunction(animName);
	}

	void SkinnedMeshRenderer::Start()
	{
		assert(!_initAnimName.empty() && "처음에 들어갈 Animation이 비어있으면 안된다");
		_setAnimationFunction(_initAnimName);
	}

}