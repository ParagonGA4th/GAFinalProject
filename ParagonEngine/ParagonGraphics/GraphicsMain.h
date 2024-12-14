#pragma once
#include "../ParagonGraphics/GraphicsDLLExporter.h"
#include "../ParagonProcess/IGraphics.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonMath/PgMath.h"

#include "TempCamera.h"
#include "TestCube.h"

#include <windows.h>
#include <memory>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonData.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonData.lib")
#endif // _DEBUG

/// <summary>
/// 
/// �׷��Ƚ� ������ ���� 
/// 
/// </summary>

class ID3D11ShaderResourceView;

namespace Pg::Util
{
	namespace Input
	{
		class InputSystem;
	}
}

namespace Pg::Core
{
	class ProcessMain;

	namespace Time
	{
		class TimeManager;
	}
}

namespace Pg::Graphics
{
	namespace Manager
	{
		class GraphicsResourceManager;
	}

	class GraphicsApiExporter;
}

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;
	class ParagonRenderer;
	class VisualEffectRenderer;

	class GraphicsMain : public Pg::Core::IGraphics
	{
		friend class GraphicsApiExporter;
	public:
		PARAGON_GRAPHICS_DLL GraphicsMain(Pg::Core::ProcessMain* core);
		virtual ~GraphicsMain();

	public:
		PARAGON_GRAPHICS_DLL virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight, const std::string& resourceListPath) override;
		PARAGON_GRAPHICS_DLL virtual void GraphicsConnectDefaultResources() override;
		//PARAGON_GRAPHICS_DLL virtual void SyncLoadGraphicsResources() override;
		PARAGON_GRAPHICS_DLL virtual void SetEditorMode(Pg::Data::Enums::eEditorMode editorMode) override;

		PARAGON_GRAPHICS_DLL virtual void Update(const Pg::Data::Scene* const scene, float deltaTime, Pg::Data::CameraData* cameraData) override;
		PARAGON_GRAPHICS_DLL virtual void BeginRender() override;
		PARAGON_GRAPHICS_DLL virtual void Render(Pg::Data::Scene* scene) override;

		//Picking : Screen Space Coord�� ������ ���� GameObject*�� ��ȯ�Ѵ�. �� �Լ� ȣ�� ���ϸ� �׷��� ���ҽ� �Ƴ� �� ����.
		//PARAGON_GRAPHICS_DLL virtual Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio) override;

		PARAGON_GRAPHICS_DLL virtual void FinalRender() override;
		PARAGON_GRAPHICS_DLL virtual void EndRender() override;
		PARAGON_GRAPHICS_DLL virtual void Finalize() override;

	public:
		PARAGON_GRAPHICS_DLL virtual ID3D11Device* GetDevice() override;
		PARAGON_GRAPHICS_DLL virtual ID3D11DeviceContext* GetDeviceContext() override;

		//Scene�� �ٲ�ų� // ������Ʈ ���� ���ܳ��ų�/������ �� �̺�Ʈ�� �߻����� ��, �׷��� ������ ���� ���� ������Ʈ���� �����Ѵ�.
		PARAGON_GRAPHICS_DLL virtual void SyncComponentToGraphics(const Pg::Data::Scene* const scene) override;

		//AssetManager���� ������ ����, �׷��Ƚ� ���ҽ� �Ŵ����� ���ҽ��� �ε��ϴ� �Լ�.
		PARAGON_GRAPHICS_DLL virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) override;

		//AssetManager���� ������ ����, �׷��Ƚ� ���ҽ� �Ŵ����� ���ҽ��� ��ε��ϴ� �Լ�.
		PARAGON_GRAPHICS_DLL virtual void UnloadResource(const std::string& filePath) override;

		// TODO: Load(Scene* )
		// 

		//�׷��Ƚ� ����, Ư�� ���ҽ��� �ε����� �� ���� �Ļ� 2�� ���ҽ� (Ex. Embedded Textures) ����� ���� ����.
		//�̸� ���� ��ϰ� ȣȯ�ϱ� ���ؼ� ���̴� �Լ�.
		PARAGON_GRAPHICS_DLL virtual std::map<std::string, Pg::Data::Enums::eAssetDefine>* SendAddedSecondaryResources() override;

		PARAGON_GRAPHICS_DLL virtual void ClearSecondaryResourcesList() override;

		//Editor ���� : Editor Camera ���� SRV�� ����.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetEditorCameraViewSRV() override;

		//Editor ���� : Game Camera ���� SRV�� ����.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetGameCameraViewSRV() override;

		//Box Render : Debug Data �ޱ�.
		PARAGON_GRAPHICS_DLL virtual void SetBoxDebugRenderData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetLineDebugRenderData(const std::vector<Pg::Data::LineInfo>& const lineColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetCapsuleDebugRenderData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetSphereDebugRenderData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetPlaneDebugRenderData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetRayCastDebugRenderData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetBox2dDebugRenderData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetNavMeshDebugRenderData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetNavCylinderDebugRenderData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec) override;

		//Picking�� ������� / ������� ������, ������ ���������� ����� ON/OFF �� �� �ִ�.
		PARAGON_GRAPHICS_DLL virtual void SetPickingEnableMode(bool val) override;

		//�ƿ����̴��� ���ӿ�����Ʈ�� ����.
		PARAGON_GRAPHICS_DLL virtual void SetOutlinedObject(Pg::Data::GameObject* obj) override;

		//�����͸� ���ؼ� / Instantiate ���� ���� ��Ÿ�ӿ� �߰� / ���� / ���� ���� ������.
		PARAGON_GRAPHICS_DLL virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_GRAPHICS_DLL virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_GRAPHICS_DLL virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;

		//���� ȣ�� X, ���μ����� ���� �ϰ������� Add/Modify/Delete�� ������Ʈ ������ �ݿ��ϴµ� ���� ��.
		PARAGON_GRAPHICS_DLL virtual void HandleRenderObjectsRuntime() override;
	public:
		PARAGON_GRAPHICS_DLL virtual void OnWindowResized(int screenWidth, int screenHeight) override;
	
	public:
		VisualEffectRenderer* GetVisualEffectRenderer();
		Pg::Data::CameraData* GetCamData();
	private:
		//�ҿ����ϰ� ���޵� CamData�� ���� ����� �ִ´�.
		void FillCamDataProjection(Pg::Data::CameraData* camData);

		//��� �߽� ���� �䱸 ���� �Űܳ��Ҵ�. (Ex. Cubemap / Box.. etc..)
		//void BasicRendersInitialize();
		//void BasicRendersConstantBufferLoad();
		//void BasicRendersDraw();

		//������ Engine���� �ʿ��� ���ҽ����� ���� ����/��� ���� �ε��ؿԾ�� ������, ������ �׷� �� ������ �ӽ�.
		void TempResourceMeshLoad();

		

	private:
		HRESULT hr;
		Pg::Core::ProcessMain* _coreMain{ nullptr };
		Pg::Graphics::Manager::GraphicsResourceManager* _graphicsResourceManager{ nullptr };
		Pg::Graphics::GraphicsApiExporter* _graphicsApiExporter{ nullptr };
	private:
		LowDX11Logic* _DXLogic;
		LowDX11Storage* _DXStorage;

	private:
		// Editor ���� & ���߿� �� SRV���� ���� �����Ǵ� Quad�� SRV���� �Ѵ�.
		ID3D11ShaderResourceView* _gameCameraSRV = nullptr;

	private:
		Pg::Util::Input::InputSystem* _input;
		Pg::Data::CameraData* _camData;

	private:
		std::unique_ptr<ParagonRenderer> _renderer;

	private:
		bool _internalPickingMode = true;

		//�ش� ���������� �����Ͱ� Skinned ���� / �� ���࿡ ������ ��ģ��.
		Pg::Data::Enums::eEditorMode _prevRecordedEditMode{ Data::Enums::eEditorMode::_NONE };
	};
}


