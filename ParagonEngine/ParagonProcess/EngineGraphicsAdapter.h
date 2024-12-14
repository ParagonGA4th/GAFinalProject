#pragma once
#include "IEngine.h"
#include "IGraphics.h"
#include "../ParagonData/EditorMode.h"

#include <memory>
#include <vector>
#include <string>
#include <d3d11.h>
/// <summary>
/// Process���� Engine�� Graphics�� ������ �� �ִ� ������� ������ �����Ѵ�.
/// </summary>

namespace Pg::Core
{
	class ProcessMain;

	namespace Manager
	{
		class AssetManager;
	}
}

namespace Pg::Data
{
	class Scene;
	class GameObject;
	class RendererChangeList;
}

namespace Pg::Core
{
	class EngineGraphicsAdapter
	{
	public:
		EngineGraphicsAdapter(ProcessMain* core);
		~EngineGraphicsAdapter();

		//���� �ʱ�ȭ.
		void InitializeEngine(void* hwnd, int screenWidth, int screenHeight, const std::string& resourceListPath);

		//�׷��Ƚ� �ʱ�ȭ.
		void InitializeGraphics(void* hwnd, int screenWidth, int screenHeight, const std::string& resourceListPath);

		//�׷��Ƚ� ���ҽ�, �ּ� �Ŵ����� ����.
		void SyncLoadGraphicsResources();
		
		//Engine Update.
		void UpdateEngine();

		//Graphics Update.
		void UpdateGraphics(const Pg::Data::Scene* const scene, float deltaTime, Pg::Data::CameraData* cameraData);

		//AssetManager Update.
		void UpdateAssetManager(Manager::AssetManager* assetManager);

		//������ ������ ��.
		void BeginRender();

		//������ ������ ������ ��.
		void Render();
		
		////��ŷ�� ������Ʈ �޾ƾ� ��. (0-1 ���� ���� // 0-1 ���� ����)
		//Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio);
		
		//�ƿ����εǾ�� �� ���ӿ�����Ʈ ����.
		void SetOutlinedGameObject(Pg::Data::GameObject* obj);

		//Quad-Based ������, Main RenderTarget����.
		void FinalRender();
		
		//������ ���� ��.
		void EndRender();

		//Finalize.
		void Finalize();

		//�������� ����� ���� ��.
		void OnWindowResized(int screenWidth, int screenHeight);

		//�׷��Ƚ� ����̽� ��ȯ.
		ID3D11Device* GetGraphicsDevice();

		//�׷��Ƚ� ����̽� ���ؽ�Ʈ ��ȯ.
		ID3D11DeviceContext* GetGraphicsDeviceContext();

		//�׷��Ƚ� �����̶� Renderer ������Ʈ�� ������Ų��.
		void UpdateRendererChangeList();

		//Editor ���� : Editor Camera ���� SRV�� ����.
		void* GetEditorCameraViewSRV();

		//Editor ���� : Game Camera ���� SRV�� ����.
		void* GetGameCameraViewSRV();

		void SetSceneList(std::vector<Pg::Data::Scene*> sceneList);
		std::vector<Pg::Data::Scene*> GetSceneList();
		void SetCurrentScene(Pg::Data::Scene* currentScene);
		Pg::Data::Scene* GetCurrentScene();

		Pg::Data::CameraData* GetCameraData();

		//�׷��Ƚ� �������� ������ ����� �����͸� �����Ѵ�.
		void PassDebugRenderData();

		//����� �ý��� ������Ʈ�� ������ Ŭ����.
		void ClearDebugVectorData();

		//��Ÿ Ÿ�� �ѱ��.
		float GetDeltaTime();

		//�����Ͱ� ��������� EndRender ���� ��밡��.  ������/���� ���� ���ؼ� ��Ÿ�ӿ� �߰� / ���� / ���� ���� ������.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);

		//�����Ϳ� ���� ���� ȣ�� X, ���μ����� ���� �ϰ������� Add/Modify/Delete�� ������Ʈ ������ �ݿ��ϴµ� ���� ��.
		void HandleRenderObjectsRuntime();

		//�̹� ���޵� 

		// ���� �������� ���¸� �޴� �Լ�
		void SetEditorMode(Pg::Data::Enums::eEditorMode mode);

		//����Ʈ ���ҽ� �ε�� ��, �׷��Ƚ����� ����.
		void GraphicsConnectDefaultResources();

		//Paragon Project�� �ε��Ǿ��� ��.
		void OnParagonProjectLoad(const std::vector<Pg::Data::Scene*>& sceneVec);

	private:
		std::unique_ptr<IEngine> _engine;					//���� ����
		std::unique_ptr<IGraphics> _graphics;				//�׷��Ƚ� ����

		ProcessMain* _processMain; //�ھ� ���� �޾ƿͼ� ����.

		//������ ü���� ����Ʈ (���� ����)
		Pg::Data::RendererChangeList* _rendererChangeList = nullptr;
	};
}


