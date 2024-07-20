#include "PgScene.h"
#include "../ParagonGameEngine/SceneSystem.h"
#include "PgGraphics.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

namespace Pg::API
{
	void PgScene::Initialize()
	{
		auto& tSceneSystem = singleton<Pg::Engine::SceneSystem>();
		_sceneSystem = &tSceneSystem;
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();

		//AssignFunctionToSceneSystem();
		ResetSceneBoolValues();
	}

	void PgScene::SetCurrentScene(const std::string& sceneName)
	{
		//FadeOut
		_pgGraphics->ScreenSpace_FadeOut();

		//활성화
		_inMidstTransition = true;
		//막 FadeOut 시작했음.
		_inMidstFadingOut = true;
		//아직 FadeIn하면 안됨.
		_inMidstFadingIn = false;
		//바뀔 Scene 이름 미리 저장해놓고 있기.
		_toChangeSceneName = sceneName;

		//if ()

		//_pgGraphics->ScreenSpace_FadeOut();
		//_sceneSystem->SetCurrentScene(sceneName);
		//_pgGraphics->ScreenSpace_FadeIn();
		//Fade In
	}

	Pg::Data::Scene* PgScene::GetCurrentScene()
	{
		return _sceneSystem->GetCurrentScene();
	}

	std::vector<Pg::Data::Scene*> PgScene::GetProjectSceneList()
	{
		return _sceneSystem->GetProjectSceneList();
	}

	void PgScene::UpdateLoop()
	{
		if (_inMidstTransition)
		{
			//들어오기 전에 무조건 Fade Out 하고 있을 것이다.
			//완료되었는지를 검사해야 한다.
			//Fade Out이 되는 중 + 다 끝났다면?
			if (_inMidstFadingOut && (!(_pgGraphics->GetIsFadingOut())))
			{
				//드디어 FadeOut이 끝.
				assert(!_toChangeSceneName.empty());
				_sceneSystem->SetCurrentScene(_toChangeSceneName);
				_pgGraphics->ScreenSpace_FadeIn();
				_inMidstFadingOut = false;
				_inMidstFadingIn = true;
			}

			if (_inMidstFadingIn && (!(_pgGraphics->GetIsFadingIn())))
			{
				//다 끝났다는 말.
				ResetSceneBoolValues();
			}
		}

	}

	void PgScene::ResetSceneBoolValues()
	{
		_inMidstTransition = false;
		_inMidstFadingOut = false;
		_inMidstFadingIn = false;
		_toChangeSceneName = "";
	}

	//void PgScene::AssignFunctionToSceneSystem()
	//{
	//	//std::function<void()> tFadeInFunction = std::bind(&Graphics::PgGraphics::ScreenSpace_FadeIn, _pgGraphics);
	//	//std::function<void()> tFadeOutFunction = std::bind(&Graphics::PgGraphics::ScreenSpace_FadeOut, _pgGraphics);
	//	//
	//	//_sceneSystem->AssignFunctionToSceneSystem(tFadeInFunction, tFadeOutFunction);
	//}

}



