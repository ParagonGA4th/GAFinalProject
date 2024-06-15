#pragma once
#include "ScriptInterface.h"
#include "GameState.h"

#include "../ParagonData/Scene.h"

#include <string>
#include <vector>

/// <summary>
/// 기존 내용은 InGameManager로 이전, 얘는 Total GameFlow를 관리한다.
/// </summary>

namespace Pg::API::Time { class PgTime; }
namespace Pg::API { class PgScene; }

namespace Pg::DataScript
{
	class TotalGameManager : public ScriptInterface<TotalGameManager>
	{
		DEFINE_PARAGON_SCRIPT_SINGLETON(TotalGameManager);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		//Scene이 바뀔 때
		virtual void OnSceneChange_Global(Pg::Data::Scene* changedScene) override;
	private:
		Pg::API::PgScene* _pgScene{ nullptr };
	};
}

