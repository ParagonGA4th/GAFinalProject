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
/// �������� ���� �������̽�. �ھ �� ����
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

		//���� : Scene�� �ٲ�ų� // ������Ʈ ���� ���ܳ��ų�/������ �� �̺�Ʈ�� �߻����� ��, �׷��� ������ ���� ���� ������Ʈ���� �����Ѵ�.
		virtual void SyncComponentToGraphics(const Pg::Data::Scene* const scene) abstract;

		virtual void OnWindowResized(int screenWidth, int screenHeight) abstract;

		//AssetManager���� ������ ����, �׷��Ƚ� ���ҽ� �Ŵ����� ���ҽ��� �ε��ϴ� �Լ�.
		virtual void LoadResource(const std::string& filePath, Pg::Data::Enums::eAssetDefine define) abstract;

		//AssetManager���� ������ ����, �׷��Ƚ� ���ҽ� �Ŵ����� ���ҽ��� ��ε��ϴ� �Լ�.
		virtual void UnloadResource(const std::string& filePath) abstract;

		//�׷��Ƚ� ����, Ư�� ���ҽ��� �ε����� �� ���� �Ļ� 2�� ���ҽ� (Ex. Embedded Textures) ����� ���� ����.
		//�̸� ���� ��ϰ� ȣȯ�ϱ� ���ؼ� ���̴� �Լ�.
		virtual std::map<std::string, Pg::Data::Enums::eAssetDefine>* SendAddedSecondaryResources() abstract;

		//�Ӻ���� �ؽ��ķ� ���� ������ ���ҽ����� ����� ����. (To Asset Manager)
		virtual void ClearSecondaryResourcesList() abstract;

		//Editor ���� : Editor Camera ���� SRV�� ����.
		virtual ID3D11ShaderResourceView* GetEditorCameraViewSRV() abstract;

		//Editor ���� : Game Camera ���� SRV�� ����.
		virtual ID3D11ShaderResourceView* GetGameCameraViewSRV() abstract;

		//Engine���� DebugData�� �޴´�.
		virtual void SetBoxDebugRenderData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec) abstract;
		virtual void SetLineDebugRenderData(const std::vector<Pg::Data::LineInfo>& const lineColVec) abstract;
		virtual void SetCapsuleDebugRenderData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec) abstract;
		virtual void SetSphereDebugRenderData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec) abstract;
		virtual void SetPlaneDebugRenderData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec) abstract;
		virtual void SetRayCastDebugRenderData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec) abstract;
		virtual void SetBox2dDebugRenderData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec) abstract;
		virtual void SetNavMeshDebugRenderData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec) abstract;
		virtual void SetNavCylinderDebugRenderData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec) abstract;
	
		//Picking�� ������� / ������� ������, ������ ���������� ����� ON/OFF �� �� �ִ�.
		virtual void SetPickingEnableMode(bool val) abstract;
		//� ������Ʈ�� �ƿ����̴����� ���� �� �ִ�.
		virtual void SetOutlinedObject(Pg::Data::GameObject* obj) abstract;

		//Picking : Screen Space Coord�� ������ ���� GameObject*�� ��ȯ�Ѵ�. ������ Nullptr ����.
		//virtual Pg::Data::GameObject* GetPickedGameObjectWithRatios(float widthRatio, float heightRatio) abstract;
	
		//�����͸� ���ؼ� ��Ÿ�ӿ� �߰� / ���� / ���� ���� ������.
		virtual void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract; 
		virtual void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;
		virtual void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP) abstract;

		//���� ȣ�� X, ���μ����� ���� �ϰ������� Add/Modify/Delete�� ������Ʈ ������ �ݿ��ϴµ� ���� ��.
		virtual void HandleRenderObjectsRuntime() abstract;
	};

}