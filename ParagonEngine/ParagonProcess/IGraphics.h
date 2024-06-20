#pragma once

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/AssetDefines.h"
#include "../ParagonData/EditorMode.h"
#include <string>
#include <vector>
#include <map>
#include <windows.h>

/// <summary>
/// 변지상의 엔진 인터페이스. 코어에 들어갈 예정
/// 2023.09.11
/// </summary>
/// 
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace Pg::Core
{
	class IGraphics abstract
	{
	public:
		virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight, const std::string& resourceListPath) abstract;
		virtual void GraphicsConnectDefaultResources() abstract;
		virtual void SetEditorMode(Pg::Data::Enums::eEditorMode editorMode) abstract;

		virtual void Update(const Pg::Data::Scene* const scene, float deltaTime, Pg::Data::CameraData* cameraData) abstract;
		virtual void BeginRender() abstract;
		virtual void Render(Pg::Data::Scene* scene) abstract;
		virtual void FinalRender() abstract;
		virtual void EndRender() abstract;

		virtual void Finalize() abstract;

		virtual ID3D11Device* GetDevice() abstract;
		virtual ID3D11DeviceContext* GetDeviceContext() abstract;

		//예정 : Scene이 바뀌거나 // 컴포넌트 등이 생겨나거나/빠지는 등 이벤트가 발생했을 때, 그래픽 엔진과 렌더 관련 컴포넌트들을 연동한다.
		virtual void SyncComponentToGraphics(const Pg::Data::Scene* const scene) abstract;

		virtual void OnWindowResized(int screenWidth, int screenHeight) abstract;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 로드하는 함수.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager와의 교류를 위해, 그래픽스 리소스 매니저에 리소스를 언로드하는 함수.
		virtual void UnloadResource(const std::string& filePath) abstract;

		//그래픽스 한정, 특정 리소스를 로드했을 시 내부 파생 2차 리소스 (Ex. Embedded Textures) 등등이 존재 가능.
		//이를 메인 목록과 호환하기 위해서 쓰이는 함수.
		virtual std::map<std::string, Pg::Data::Enums::eAssetDefine>* SendAddedSecondaryResources() abstract;

		//임베디드 텍스쳐로 인해 생성된 리소스들의 목록을 연동. (To Asset Manager)
		virtual void ClearSecondaryResourcesList() abstract;

		//Editor 연동 : Editor Camera 시점 SRV를 리턴.
		virtual ID3D11ShaderResourceView* GetEditorCameraViewSRV() abstract;

		//Editor 연동 : Game Camera 시점 SRV를 리턴.
		virtual ID3D11ShaderResourceView* GetGameCameraViewSRV() abstract;

		//Engine에서 DebugData를 받는다.
		virtual void SetBoxDebugRenderData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec) abstract;
		virtual void SetLineDebugRenderData(const std::vector<Pg::Data::LineInfo>& const lineColVec) abstract;
		virtual void SetCapsuleDebugRenderData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec) abstract;
		virtual void SetSphereDebugRenderData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec) abstract;
		virtual void SetPlaneDebugRenderData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec) abstract;
		virtual void SetRayCastDebugRenderData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec) abstract;
		virtual void SetBox2dDebugRenderData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec) abstract;
		virtual void SetNavMeshDebugRenderData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec) abstract;
		virtual void SetNavCylinderDebugRenderData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec) abstract;
	
		//Picking을 허용할지 / 허용하지 않을지, 렌더링 파이프라인 비용을 ON/OFF 할 수 있다.
		virtual void SetPickingEnableMode(bool val) abstract;
		//어떤 오브젝트를 아웃라이닝할지 정할 수 있다.
		virtual void SetOutlinedObject(Pg::Data::GameObject* obj) abstract;

		//Picking : Screen Space Coord의 비율에 따라서 GameObject*를 반환한다. 없으면 Nullptr 전달.
		//virtual Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio) abstract;
	
		//에디터를 위해서 런타임에 추가 / 수정 / 삭제 로직 열었다.
		virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract; 
		virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;
		virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;

		//직접 호출 X, 프로세스에 의해 일괄적으로 Add/Modify/Delete된 오브젝트 실제로 반영하는데 쓰일 것.
		virtual void HandleRenderObjectsRuntime() abstract;
	};

}