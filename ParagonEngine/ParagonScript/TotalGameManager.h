#pragma once
#include "ScriptInterface.h"
#include "GameState.h"
#include "HandleBundle3D.h"

#include "../ParagonData/ISortableGlobalObject.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

/// <summary>
/// 기존 내용은 InGameManager로 이전, 얘는 Total GameFlow를 관리한다.
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API::Input { class PgInput; }
namespace Pg::API { class PgScene; }
namespace Pg::API::Tween { class PgTween; }

namespace Pg::DataScript
{
	class InGameManager;
	class CombatSystem;

	class TotalGameManager : public ScriptInterface<TotalGameManager>, public Pg::Data::ISortableGlobalObject
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(TotalGameManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		//Scene이 바뀔 때 Global Object만 루프로 호출됨. 이제 호출 순서도 정해져 있으니, 걱정 안해도 됨.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;

		//Manager들 중에서도 가장 먼저 Update가 돌아야 한다.
		virtual unsigned int GetPriorityIndex() override { return 0; }

	public:
		//하위 객체들이 호출할 수 있는 함수 목록:
		//일단 이 함수가 유일하다. 리셋은 이 정도.
		//동일한 매개변수 체제를 가져서, 추후에 다른 옵션이 호출되는 것을 대비.
		void CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer);

		//2D일 경우, nullptr를 반환할 것.
		HandlerBundle3D* GetCurrentHandlerBundle();

		//Scene 파라미터에 따라 HandlerBundle3D 리턴.
		HandlerBundle3D* GetHandlerBundleByScene(Pg::Data::Scene* scene);


	private:
		//내부적으로 정해진 타이밍에 호출할 수 있게.
		void Internal_CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer);

		//Combat System Unit 구성. RegisterXXX 시리즈 호출.
		void RegisterCombatSystemUnits();

		//하위 컴포넌트들이 호출할 수 있게, 함수들을 보관하고 있다.
		std::vector<std::function<void()>> _subFunctionStorageVector;

	private:
		void Initialize(Pg::Data::Scene* changedScene);
		void SetupBundlesForAllScenes();
		void SetCurrentHandlerBundle(Pg::Data::Scene* changedScene);
		

	private:
		//하드코딩된 치트 함수들. '\' + 콤비네이션으로 호출할 수 있게.
		//버튼 하나로 호출되지는 않을 것이다.
		void SelectCheatCodeWithin();


	private:
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };

	private:
		//Flow 관리를 위해 보관하는 정보. (3D)
		std::unordered_map<Pg::Data::Scene*, std::unique_ptr<HandlerBundle3D>> _scene3dHandlerBundleMap;
		//Flow 관리를 위해 보관하는 정보. (2D)
		std::unordered_map<Pg::Data::Scene*, BaseGUIHandler*> _scene2dHandlerBundleMap;

		//2D/3D 보관에 따라 nullptr 할당이 달라진다.
		HandlerBundle3D* _currentHandlerBundle3d{ nullptr }; //3D의 경우 nullptr면 안됨, 2D면 nullptr여야.
		BaseGUIHandler* _currentGUIHander2d{ nullptr }; //3D의 경우 nullptr여야, 2D면 nullptr면 안됨..

		//전체 값이 받아졌는지 -> Initialize를 초기에 하기 위해서.
		bool _isManagingInitializeCalled{ false };

	private:
		//보관중인 다른 매니저들.
		//InGameManager* _inGameManager{ nullptr };

		CombatSystem* _combatSystem{ nullptr };
	};
}

