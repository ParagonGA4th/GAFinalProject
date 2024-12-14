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
/// 그래픽스 엔진의 메인 
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

		//Picking : Screen Space Coord의 비율에 따라서 GameObject*를 반환한다. 이 함수 호출 안하면 그래픽 리소스 아낄 수 있음.
		//PARAGON_GRAPHICS_DLL virtual Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio) override;

		PARAGON_GRAPHICS_DLL virtual void FinalRender() override;
		PARAGON_GRAPHICS_DLL virtual void EndRender() override;
		PARAGON_GRAPHICS_DLL virtual void Finalize() override;

	public:
		PARAGON_GRAPHICS_DLL virtual ID3D11Device* GetDevice() override;
		PARAGON_GRAPHICS_DLL virtual ID3D11DeviceContext* GetDeviceContext() override;

		//Scene이 바뀌거나 // 컴포넌트 등이 생겨나거나/빠지는 등 이벤트가 발생했을 때, 그래픽 엔진과 렌더 관련 컴포넌트들을 연동한다.
		PARAGON_GRAPHICS_DLL virtual void SyncComponentToGraphics(const Pg::Data::Scene* const scene) override;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		PARAGON_GRAPHICS_DLL virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) override;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 언로드하는 함수.
		PARAGON_GRAPHICS_DLL virtual void UnloadResource(const std::string& filePath) override;

		// TODO: Load(Scene* )
		// 

		//그래픽스 한정, 특정 리소스를 로드했을 시 내부 파생 2차 리소스 (Ex. Embedded Textures) 등등이 존재 가능.
		//이를 메인 목록과 호환하기 위해서 쓰이는 함수.
		PARAGON_GRAPHICS_DLL virtual std::map<std::string, Pg::Data::Enums::eAssetDefine>* SendAddedSecondaryResources() override;

		PARAGON_GRAPHICS_DLL virtual void ClearSecondaryResourcesList() override;

		//Editor 연동 : Editor Camera 시점 SRV를 리턴.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetEditorCameraViewSRV() override;

		//Editor 연동 : Game Camera 시점 SRV를 리턴.
		PARAGON_GRAPHICS_DLL virtual ID3D11ShaderResourceView* GetGameCameraViewSRV() override;

		//Box Render : Debug Data 받기.
		PARAGON_GRAPHICS_DLL virtual void SetBoxDebugRenderData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetLineDebugRenderData(const std::vector<Pg::Data::LineInfo>& const lineColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetCapsuleDebugRenderData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetSphereDebugRenderData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetPlaneDebugRenderData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetRayCastDebugRenderData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetBox2dDebugRenderData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetNavMeshDebugRenderData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec) override;
		PARAGON_GRAPHICS_DLL virtual void SetNavCylinderDebugRenderData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec) override;

		//Picking을 허용할지 / 허용하지 않을지, 렌더링 파이프라인 비용을 ON/OFF 할 수 있다.
		PARAGON_GRAPHICS_DLL virtual void SetPickingEnableMode(bool val) override;

		//아웃라이닝할 게임오브젝트를 고른다.
		PARAGON_GRAPHICS_DLL virtual void SetOutlinedObject(Pg::Data::GameObject* obj) override;

		//에디터를 위해서 / Instantiate 등을 위해 런타임에 추가 / 수정 / 삭제 로직 열었다.
		PARAGON_GRAPHICS_DLL virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_GRAPHICS_DLL virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;
		PARAGON_GRAPHICS_DLL virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) override;

		//직접 호출 X, 프로세스에 의해 일괄적으로 Add/Modify/Delete된 오브젝트 실제로 반영하는데 쓰일 것.
		PARAGON_GRAPHICS_DLL virtual void HandleRenderObjectsRuntime() override;
	public:
		PARAGON_GRAPHICS_DLL virtual void OnWindowResized(int screenWidth, int screenHeight) override;
	
	public:
		VisualEffectRenderer* GetVisualEffectRenderer();
		Pg::Data::CameraData* GetCamData();
	private:
		//불완전하게 전달된 CamData에 투영 행렬을 넣는다.
		void FillCamDataProjection(Pg::Data::CameraData* camData);

		//기능 중심 구현 요구 사항 옮겨놓았다. (Ex. Cubemap / Box.. etc..)
		//void BasicRendersInitialize();
		//void BasicRendersConstantBufferLoad();
		//void BasicRendersDraw();

		//원래는 Engine에서 필요한 리소스들을 씬의 상태/양상에 따라 로드해왔어야 하지만, 지금은 그럴 수 없으니 임시.
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
		// Editor 연동 & 나중에 이 SRV들이 최종 렌더되는 Quad의 SRV여야 한다.
		ID3D11ShaderResourceView* _gameCameraSRV = nullptr;

	private:
		Pg::Util::Input::InputSystem* _input;
		Pg::Data::CameraData* _camData;

	private:
		std::unique_ptr<ParagonRenderer> _renderer;

	private:
		bool _internalPickingMode = true;

		//해당 지역변수의 포인터가 Skinned 진행 / 미 진행에 영향을 끼친다.
		Pg::Data::Enums::eEditorMode _prevRecordedEditMode{ Data::Enums::eEditorMode::_NONE };
	};
}


