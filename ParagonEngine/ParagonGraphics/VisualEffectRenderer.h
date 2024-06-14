#pragma once
#include "../ParagonData/VisualEffectData.h"
#include "../ParagonData/VisualEffectRenderObject.h"
#include "../ParagonUtil/Tween.h"
#include "VisualEffectGraphicsSet.h"
#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <dxtk/SpriteBatch.h>

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}
}

namespace Pg::Graphics
{
	struct VERenderingSet
	{
		VERenderingSet(Pg::Data::VisualEffectData veData, VisualEffectGraphicsSet veGraphicsSet) :
			_visualEffectData(veData), _visualEffectGraphicsSet(veGraphicsSet) {}
		Pg::Data::VisualEffectData _visualEffectData;
		VisualEffectGraphicsSet _visualEffectGraphicsSet;
		//그 다음에 기타 정보가 필요하다면 투입될 것.
	};
}

namespace Pg::Graphics
{
	class VisualEffectRenderer
	{
		//Billboard Render Pass만 쓰일 수 있다.
		friend class BillboardRenderPass;
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
		void Render();
	private:
		Manager::GraphicsResourceManager* _graphicsResourceManager{ nullptr };
		
		//리소스 저장소. EffectName // Data.
		std::unordered_map<std::string, std::unique_ptr<VERenderingSet>> _visualEffectsMap;
		//이펙트가 엄청 많은 것도 아니고, 그냥 1 이펙트당 Rendering State 바꿔준다고 생각하자.

		//현재 렌더되는 오브젝트 목록.
		std::unordered_map<VERenderingSet*, std::vector<Pg::Data::VisualEffectRenderObject*>> _currentRenderingMap;
		
		
	};
}
