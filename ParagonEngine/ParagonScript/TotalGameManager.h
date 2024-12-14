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
/// ���� ������ InGameManager�� ����, ��� Total GameFlow�� �����Ѵ�.
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

		//Scene�� �ٲ� �� Global Object�� ������ ȣ���. ���� ȣ�� ������ ������ ������, ���� ���ص� ��.
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;

		//Manager�� �߿����� ���� ���� Update�� ���ƾ� �Ѵ�.
		virtual unsigned int GetPriorityIndex() override { return 0; }

	public:
		//���� ��ü���� ȣ���� �� �ִ� �Լ� ���:
		//�ϴ� �� �Լ��� �����ϴ�. ������ �� ����.
		//������ �Ű����� ü���� ������, ���Ŀ� �ٸ� �ɼ��� ȣ��Ǵ� ���� ���.
		void CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer);

		//2D�� ���, nullptr�� ��ȯ�� ��.
		HandlerBundle3D* GetCurrentHandlerBundle();

		//Scene �Ķ���Ϳ� ���� HandlerBundle3D ����.
		HandlerBundle3D* GetHandlerBundleByScene(Pg::Data::Scene* scene);


	private:
		//���������� ������ Ÿ�ֿ̹� ȣ���� �� �ְ�.
		void Internal_CallForEntireSceneReset(Pg::Data::Scene* targetScene, int potValue, void* potPointer);

		//Combat System Unit ����. RegisterXXX �ø��� ȣ��.
		void RegisterCombatSystemUnits();

		//���� ������Ʈ���� ȣ���� �� �ְ�, �Լ����� �����ϰ� �ִ�.
		std::vector<std::function<void()>> _subFunctionStorageVector;

	private:
		void Initialize(Pg::Data::Scene* changedScene);
		void SetupBundlesForAllScenes();
		void SetCurrentHandlerBundle(Pg::Data::Scene* changedScene);
		

	private:
		//�ϵ��ڵ��� ġƮ �Լ���. '\' + �޺���̼����� ȣ���� �� �ְ�.
		//��ư �ϳ��� ȣ������� ���� ���̴�.
		void SelectCheatCodeWithin();


	private:
		Pg::API::PgScene* _pgScene{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };

	private:
		//Flow ������ ���� �����ϴ� ����. (3D)
		std::unordered_map<Pg::Data::Scene*, std::unique_ptr<HandlerBundle3D>> _scene3dHandlerBundleMap;
		//Flow ������ ���� �����ϴ� ����. (2D)
		std::unordered_map<Pg::Data::Scene*, BaseGUIHandler*> _scene2dHandlerBundleMap;

		//2D/3D ������ ���� nullptr �Ҵ��� �޶�����.
		HandlerBundle3D* _currentHandlerBundle3d{ nullptr }; //3D�� ��� nullptr�� �ȵ�, 2D�� nullptr����.
		BaseGUIHandler* _currentGUIHander2d{ nullptr }; //3D�� ��� nullptr����, 2D�� nullptr�� �ȵ�..

		//��ü ���� �޾������� -> Initialize�� �ʱ⿡ �ϱ� ���ؼ�.
		bool _isManagingInitializeCalled{ false };

	private:
		//�������� �ٸ� �Ŵ�����.
		//InGameManager* _inGameManager{ nullptr };

		CombatSystem* _combatSystem{ nullptr };
	};
}

