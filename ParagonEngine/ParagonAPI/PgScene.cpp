#include "PgScene.h"
#include "../ParagonGameEngine/SceneSystem.h"
#include "../ParagonUtil/Log.h"
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
		PG_WARN("Setting Current Scene");
		//�ٲ� Scene �̸� �̸� �����س��� �ֱ�.
		_toChangeSceneName = sceneName;
		_shouldChangeScene = true;
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
		if (_shouldChangeScene)
		{
			if (!_inMidstTransition)
			{
				PG_WARN("IsInvokedFadeOut");

				//FadeOut
				_pgGraphics->ScreenSpace_FadeOut();

				//Ȱ��ȭ
				_inMidstTransition = true;
				//�� FadeOut ��������.
				_inMidstFadingOut = true;
				//���� FadeIn�ϸ� �ȵ�.
				_inMidstFadingIn = false;
			}
			else
			{
				PG_WARN("SceneChange: �ּ� ȣ�� �ð� ������ ��ȣ��Ǿ���.�ش� ���� ����");
			}

			//�ٽ� �ߵ��� ������ OFF.
			_shouldChangeScene = false;
		}
		
		if (_inMidstTransition)
		{
			//������ ���� ������ Fade Out �ϰ� ���� ���̴�.
			//�Ϸ�Ǿ������� �˻��ؾ� �Ѵ�.
			//Fade Out�� �Ǵ� �� + �� �����ٸ�?
			if (_inMidstFadingOut && (!(_pgGraphics->GetIsFadingOut())))
			{
				PG_WARN("IsInvokedFadeIn");
				//���� FadeOut�� ��.
				assert(!_toChangeSceneName.empty());
				_sceneSystem->SetCurrentScene(_toChangeSceneName);
				_pgGraphics->ScreenSpace_FadeIn();
				_inMidstFadingOut = false;
				_inMidstFadingIn = true;
			}

			if (_inMidstFadingIn && (!(_pgGraphics->GetIsFadingIn())))
			{
				//�� �����ٴ� ��.	
				ResetSceneBoolValues();
			}
		}

	}

	void PgScene::ResetSceneBoolValues()
	{
		_shouldChangeScene = false;
		_inMidstTransition = false;
		_inMidstFadingOut = false;
		_inMidstFadingIn = false;
		_toChangeSceneName = "";
	}

	Pg::Data::Enums::eEditorMode PgScene::GetEditorMode()
	{
		return _sceneSystem->GetEditorMode();
	}

	//void PgScene::AssignFunctionToSceneSystem()
	//{
	//	//std::function<void()> tFadeInFunction = std::bind(&Graphics::PgGraphics::ScreenSpace_FadeIn, _pgGraphics);
	//	//std::function<void()> tFadeOutFunction = std::bind(&Graphics::PgGraphics::ScreenSpace_FadeOut, _pgGraphics);
	//	//
	//	//_sceneSystem->AssignFunctionToSceneSystem(tFadeInFunction, tFadeOutFunction);
	//}

}



