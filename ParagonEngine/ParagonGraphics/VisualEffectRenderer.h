#pragma once
#include "../ParagonData/VisualEffectData.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonUtil/Tween.h"
#include "VisualEffectGraphicsSet.h"
#include "SystemVertexShader.h"

#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>

#include <dxtk/CommonStates.h>

namespace Pg::Graphics
{
	class LowDX11Storage;

	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

/// <summary>
/// Mesh관련된 이펙트를 하려면,
/// Renderer를 연결한 Script를 보여주는 형식으로 해야 한다.
/// 여기는 BillBoard 2D / 3D만 할당하게 하자.
/// </summary>

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

namespace Pg::Graphics
{
	struct VERenderingSet
	{
		VERenderingSet(Pg::Data::VisualEffectData veData) :
			_visualEffectData(veData)
		{
			_veGraphicsSet = new VisualEffectGraphicsSet();
		}
		~VERenderingSet()
		{
			if (_veGraphicsSet != nullptr) { delete _veGraphicsSet; }
		}
		Pg::Data::VisualEffectData _visualEffectData;

		VisualEffectGraphicsSet* _veGraphicsSet{ nullptr };
		//그 다음에 기타 정보가 필요하다면 투입될 것.
	};
}

namespace Pg::Graphics
{
	struct D3DCarrier;

	class VisualEffectRenderer
	{
		//Billboard Render Pass만 쓰일 수 있다.
		friend class VisualEffectRenderPass;
	public:
		VisualEffectRenderer();
		~VisualEffectRenderer();

	public:
		//CSV에서 로드된 구조체에서, 로드. 
		void LoadAllEffects(const std::vector<Pg::Data::VisualEffectData>& vec);

		//Visual Effect Object 싹 다 리셋.
		void ResetVisualEffectObjects();

		//Visual Effect Render Object는 다른 오브젝트와는 달리,
		//구조체를 만들어서, 이를 Rendering Engine에 등록하는 방식으로 되어 있다.
		//Tween등 값을 가능하게 하기 위해 다만, 값은 보관해야 한다.
		//수명은 Script에 보관된다. Tween 등 동작을 했을 때, Null Reference를 막기 위해서.
		//명시적으로 수명을 죽이고 싶으면, Remove를 호출해야 한다.
		//잠시 껐다 키고 싶으면, SetActive를 활용하면 된다.
		void RegisterVisualEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj);
		void RemoveVisualEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj);

		//Main Render Loop에서 돌아가는 효과이다.
		void Render(const D3DCarrier* carrier, Pg::Data::CameraData* camData);

		//인덱스를 설정할 수 있어야 하기에, 이펙트 유효 + ManualSwitchMode가 꺼져 있으면 nullptr 반환.
		unsigned int* GetEffectTextureIndexPointer(const std::string& effectName);
	private:
		void LoadSingleEffect(VERenderingSet* veSet);
		//void Load3DSpaceEffect(VERenderingSet* veSet);
		//void Render3dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj);
		//void Render2dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj);
		//추가로 더 추가될 수 있음.
	private:
		Manager::GraphicsResourceManager* _graphicsResourceManager{ nullptr };
		LowDX11Storage* _DXStorage{ nullptr };
		
		//리소스 저장소. EffectName // Data.
		std::unordered_map<std::string, std::unique_ptr<VERenderingSet>> _visualEffectsMap;
		//이펙트가 엄청 많은 것도 아니고, 그냥 1 이펙트당 Rendering State 바꿔준다고 생각하자.

		//현재 렌더되는 오브젝트 목록.
		std::unordered_map<VERenderingSet*, std::vector<Pg::Data::VisualEffectRenderObject*>> _currentRenderingMap;
		
		//CommonState 저장.
		std::unique_ptr<DirectX::CommonStates> _commonStates{ nullptr };
	
	private:
		Pg::Util::Time::TimeSystem* _timeSystem{ nullptr };
	};
}
