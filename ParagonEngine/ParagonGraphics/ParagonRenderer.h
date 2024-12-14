#pragma once
#include "D3DCarrier.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/DebugData.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/EditorMode.h"

//SkinningMk.2
//#include "MultimaterialMesh.h"

#include <memory>
#include <vector>

/// <summary>
/// Paragon Renderer
/// 
/// 2023. 09. 12.
/// </summary>

namespace Pg::Data
{
	class Scene;
	class GameObject;
}

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;
	class DeferredRenderer;
	class CubemapRenderer;
	class Forward2DRenderer;
	class Forward3DRenderer;
	class DebugRenderer;
	class PPFinalRenderer;
	class GraphicsSceneParser;
	class LightmapManager;

	class SceneInformationList;
	class GraphicsApiExporter;
	class VisualEffectRenderer;
}

namespace Pg::Graphics
{
	class ParagonRenderer
	{
		friend class GraphicsApiExporter;
	public:
		ParagonRenderer();
		~ParagonRenderer();

		//Render ���� Component �����͸� �ٽ� �ε��Ѵ�. -> �� �Լ��� ������ �ߵ��Ǿ�� �ϴµ�..
		virtual void SyncComponentToGraphics(const Pg::Data::Scene* const newScene);
		
		//Scene�� �ٲ���� �� / ���� ������ ó�� ���۵Ǿ��� �� ȣ��Ǿ�� �Ѵ�.
		//�޸� �߰� �Ҵ��� ���� ����, Scene�� ����������Ʈ ���� ���� �ߺ��� ���ƾ� �Ѵ�!

	public:
		void Initialize(const Pg::Data::Enums::eEditorMode* const editorMode, const std::string& resourceListPath);
		void ConnectDefaultResources();
		//DebugRenderer�� Debug Geometry�� �Ѱ��.
		void PassBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec);
		void PassLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec);
		void PassCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec);
		void PassSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec);
		void PassPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec);
		void PassRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec);
		void PassBox2dGeometryData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec);
		void PassNavMeshGeometryData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec);
		void PassNavCylinderGeometryData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec);
		//Picking�� ���ӿ�����Ʈ�� ������.
		//[[deprecated("Not Using Picking Anymore")]]
		//Pg::Data::GameObject* GetPickedID_SetOutlineMode(int selectedWidthPixel, int selectedHeightPixel);
		
		//��ü������ �������� ���� ���ӿ�����Ʈ��� �ƿ����� �н��� �������� �ʴ´�.
		void SetOutlinedGameObject(Pg::Data::GameObject* outlinedObj);

		void BeginRender();
		void Render(Pg::Data::CameraData* camData);			// �̹� ������Ʈ �ܰ迡�� RenderObject��� �����Ǳ⿡, ������Ʈ ��ü�� ���� �ʿ䰡 ����.
		void DebugRender(Pg::Data::CameraData* camData);	// ������ Debug Render�� �Ѵ�.
		void UiRender(Pg::Data::CameraData* camData);		// 2D UI�� �������Ѵ�.
		void PostProcessingRender(Pg::Data::CameraData* camData); //PostProcessing�� �������Ѵ�.
		void DebugQuadsOverlayRender();
		void FadeInOutRender();
		void FinalRender(Pg::Data::CameraData* camData);	// MainRenderTarget���� �����ִ� ������ �Ѵ�.
		void EndRender();

		//������ SRV Quad�� ������.
		ID3D11ShaderResourceView* GetFinalQuadSRV();

		//DeltaTime�� �Ѱܹ޴´�.
		void SetDeltaTime(float dt);

		//������ ������ ����.
		void AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);
		void DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP);

		//������ �����ؼ� ó�� (������ Add/Modify/Delete�Ǿ��� ������Ʈ ��ȭ ���� �ݿ�)
		void HandleRenderObjectsRuntime();

	public:
		VisualEffectRenderer* GetVisualEffectRenderer();


	private:
		//�� ������ �޾Ƶ��̱�. (������ ������ ���·�)
		void ParseSceneData(const Pg::Data::Scene* const newScene);

	private:
		LowDX11Storage* _DXStorage = nullptr;
		LowDX11Logic* _DXLogic = nullptr;

		std::unique_ptr<GraphicsSceneParser> _sceneParser;
		std::unique_ptr<LightmapManager> _lightmapManager;
	
		std::unique_ptr<DeferredRenderer> _deferredRenderer;
		std::unique_ptr<CubemapRenderer> _cubemapRenderer;
		std::unique_ptr<Forward2DRenderer> _forward2dRenderer;
		std::unique_ptr<Forward3DRenderer> _forward3dRenderer;
		std::unique_ptr<DebugRenderer> _debugRenderer;
		std::unique_ptr<PPFinalRenderer> _finalRenderer;

	private:
		//����� X, ��ü�� �ӽ÷� ������ �� �ְ� �Ű������� �ڷ� ����.
		//�������� ParagonRenderer�� �����ϰ� �ִ�.
		std::unique_ptr<D3DCarrier> _gCarrier;
	private:
		//SkinningMk.2 ����.
		//MultimaterialMesh* _tempMultiMesh;
		std::string _resourcePath;
		
		const Pg::Data::Scene* _currentScene;
	};
}

	