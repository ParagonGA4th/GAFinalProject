#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"
#include "SceneInformationList.h"
#include "LightmapManager.h"

#include "GraphicsSceneParser.h"
#include "LightmapManager.h"
#include "DeferredRenderer.h"
#include "CubemapRenderer.h"
#include "Forward2DRenderer.h"
#include "Forward3DRenderer.h"
#include "DebugRenderer.h"
#include "PPFinalRenderer.h"

#include "VisualEffectRenderer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/RendererBase3D.h"

#include <utility>
#include <singleton-cpp/singleton.h>

namespace Pg::Graphics
{
	using Pg::Graphics::Helper::MathHelper;

	ParagonRenderer::ParagonRenderer() :
		_DXStorage(LowDX11Storage::GetInstance()), _DXLogic(LowDX11Logic::GetInstance())
	{
		
	}

	ParagonRenderer::~ParagonRenderer()
	{

	}

	void ParagonRenderer::Initialize(const Pg::Data::Enums::eEditorMode* const editorMode, const std::string& resourceListPath)
	{
		_resourcePath = resourceListPath;

		//SceneParser ����� Initialize();
		_sceneParser = std::make_unique<GraphicsSceneParser>();
		_sceneParser->Initialize();

		//�������� ���ο��� ���� �� GraphicsCarrier ��ü ����.
		_gCarrier = std::make_unique<D3DCarrier>();

		_deferredRenderer = std::make_unique<DeferredRenderer>(_gCarrier.get(), editorMode);
		_deferredRenderer->Initialize();

		_cubemapRenderer = std::make_unique<CubemapRenderer>(_gCarrier.get());
		_cubemapRenderer->Initialize();

		_forward2dRenderer = std::make_unique<Forward2DRenderer>(_gCarrier.get());
		_forward2dRenderer->Initialize();

		_forward3dRenderer = std::make_unique<Forward3DRenderer>(_gCarrier.get(), editorMode, resourceListPath);
		_forward3dRenderer->Initialize();

		_debugRenderer = std::make_unique<DebugRenderer>(_gCarrier.get());
		_debugRenderer->Initialize();

		_finalRenderer = std::make_unique<PPFinalRenderer>(_gCarrier.get());
		_finalRenderer->Initialize();
		//SkinningMk.F
		//_tempMultiMesh = new MultimaterialMesh("tFilePath);
	}

	void ParagonRenderer::ConnectDefaultResources()
	{
		_deferredRenderer->ConnectDefaultResources();
		_finalRenderer->ConnectDefaultResources();

		_lightmapManager = std::make_unique<LightmapManager>();
		_lightmapManager->Initialize(_resourcePath);
	}

	void ParagonRenderer::BeginRender()
	{
		
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{
		_deferredRenderer->ClearPlaceResources();
		//�̰Ŵ� ������.
		_lightmapManager->Temp(_currentScene);

		// Deferred w/ Pass
		_deferredRenderer->RenderContents(_sceneParser->GetRenderObject3DList(), _sceneParser->GetSceneInformationList(), camData);
		_deferredRenderer->ConfirmCarrierData();

		// Cubemap Renderer.
		_cubemapRenderer->RenderContents(_sceneParser->GetRenderObjectCubemapList(), nullptr, camData);
		_cubemapRenderer->ConfirmCarrierData();

		_forward3dRenderer->RenderContents(_sceneParser->GetRenderObject3DList(), nullptr, camData);
		_forward3dRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::DebugRender(Pg::Data::CameraData* camData)
	{
		_debugRenderer->RenderContents(_sceneParser->GetRenderObjectWireframeList(), nullptr, camData);
		_debugRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::UiRender(Pg::Data::CameraData* camData)
	{
		// Forward 2D
		_forward2dRenderer->RenderContents(_sceneParser->GetRenderObject2DList(), nullptr, camData);
		_forward2dRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::PostProcessingRender(Pg::Data::CameraData* camData)
	{
		_finalRenderer->RenderPostProcessingStages(nullptr, camData);
	}

	void ParagonRenderer::DebugQuadsOverlayRender()
	{
		_finalRenderer->RenderDebugQuadsOverlay();
	}

	void ParagonRenderer::FinalRender(Pg::Data::CameraData* camData)
	{
		//���������� �˾Ƽ� �������� ��. (Picking�� ���� Outline ��尡 ���������� ȣ��� ���̱� ������!
		_finalRenderer->RenderOutlineStencil(camData);
		_finalRenderer->RenderContents(nullptr, nullptr, camData);
		_finalRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

	void ParagonRenderer::SyncComponentToGraphics(const Pg::Data::Scene* const newScene)
	{
		_currentScene = newScene;

		PG_TRACE("Started Parsing SceneData...");
		//���� ParseSceneData ���� ����ü�� �� �Ź� Graphics ��ü�� �ٽ� ������� �𸣰�����..
		//�ϴ� ���ϴ� ���߿� TODO.
		ParseSceneData(newScene);
		PG_TRACE("...Ended Parsing SceneData");

		//GPU Lightmap Data ����. ���� �ٲ� ����.
		PG_TRACE("Started GPU Lightmapping Setup...");
		_lightmapManager->SetGPULightmapDataWithScene(newScene, _sceneParser->GetRenderObject3DList());
		_sceneParser->GetSceneInformationList()->_isUseLightmap = _lightmapManager->GetIsSceneUseLightmap();
		PG_TRACE("...Ended GPU Lightmapping Setup");
	}

	void ParagonRenderer::PassBoxGeometryData(const std::vector<Pg::Data::BoxInfo*>& const boxColVec)
	{
		_debugRenderer->GetDebugBoxGeometryData(boxColVec);
	}

	void ParagonRenderer::PassLineGeometryData(const std::vector<Pg::Data::LineInfo>& const lineColVec)
	{
		_debugRenderer->GetDebugLineGeometryData(lineColVec);
	}

	void ParagonRenderer::PassCapsuleGeometryData(const std::vector<Pg::Data::CapsuleInfo*>& const capsuleColVec)
	{
		_debugRenderer->GetDebugCapsuleGeometryData(capsuleColVec);
	}

	void ParagonRenderer::PassSphereGeometryData(const std::vector<Pg::Data::SphereInfo*>& const sphereColVec)
	{
		_debugRenderer->GetDebugSphereGeometryData(sphereColVec);
	}

	void ParagonRenderer::PassPlaneGeometryData(const std::vector<Pg::Data::PlaneInfo*>& const planeColVec)
	{
		_debugRenderer->GetDebugPlaneGeometryData(planeColVec);
	}

	void ParagonRenderer::PassRayCastGeometryData(const std::vector<Pg::Data::RayCastInfo>& const rayCastColVec)
	{
		_debugRenderer->GetDebugRayCastGeometryData(rayCastColVec);
	}

	void ParagonRenderer::PassBox2dGeometryData(const std::vector<Pg::Data::Box2DInfo>& const box2dColVec)
	{
		_debugRenderer->GetDebugBox2dGeometryData(box2dColVec);
	}

	void ParagonRenderer::PassNavMeshGeometryData(const std::vector<Pg::Data::NavMeshInfo*>& const navMeshVec)
	{
		_debugRenderer->GetDebugNavMeshGeometryData(navMeshVec);
	}

	void ParagonRenderer::PassNavCylinderGeometryData(const std::vector<Pg::Data::NavCylinderInfo*>& const navCylinderVec)
	{
		_debugRenderer->GetDebugNavCylinderGeometryData(navCylinderVec);
	}

	//void ParagonRenderer::SyncDebugGeometryToGraphics(const Pg::Data::Scene* const newScene)
	//{
	//
	//}

	void ParagonRenderer::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//ParseSceneData�� �귣ġ ��ġ�� ���� SyncComponent�� �и� �Ұ�.
		_sceneParser->ParseSceneData(newScene);


		static bool tDebouncer = true;

		if (tDebouncer)
		{
			//��� RenderPass�� �¾��ϱ�.
			_deferredRenderer->SetupRenderPasses();
			_cubemapRenderer->SetupRenderPasses();
			_forward3dRenderer->SetupRenderPasses(); //�굵 Deferredó�� �и��Ǿ�� �ϴ°�?
			_forward2dRenderer->SetupRenderPasses();
			_debugRenderer->SetupRenderPasses();
			_finalRenderer->SetupRenderPasses();

			tDebouncer = false;
		}

		//�Ź� �ٸ� ���͸����� �ε��ؾ� �ϴ� Deferred�� �ణ �ٸ� �� �ִ�.
		_deferredRenderer->SetupOpaqueQuadRenderPasses();
		_deferredRenderer->InitializeResettablePasses();
	}

	ID3D11ShaderResourceView* ParagonRenderer::GetFinalQuadSRV()
	{
		assert(_gCarrier->_toSendSRVToEngine != nullptr);

		return _gCarrier->_toSendSRVToEngine;
	}

	//Pg::Data::GameObject* ParagonRenderer::GetPickedID_SetOutlineMode(int selectedWidthPixel, int selectedHeightPixel)
	//{
	//	unsigned int tFoundID = _finalRenderer->GetPickingObjectID(selectedWidthPixel, selectedHeightPixel);
	//
	//	if (tFoundID == 0)
	//	{
	//		//������, Outline Buffer�� ���� �׷��� ������ ����.
	//		_finalRenderer->SetOutlineRenderingMode(false);
	//		return nullptr;
	//	}
	//	
	//	//�ش� Material ���� ���δ� Outline Shader Rendering ����. -> �׷��ְ� Bool ������ ����.
	//	_finalRenderer->SetOutlineRenderingMode(true);
	//	_finalRenderer->SetObjectIDSelected(tFoundID);
	//
	//	//������ ���������� ���� ã�Ƽ� �����ؾ� �Ѵ�. �׷��� ���� ��츦 �ܺο��� ����ó����.
	//	return _sceneParser->GetObjectWithObjID(tFoundID);
	//}

	void ParagonRenderer::SetOutlinedGameObject(Pg::Data::GameObject* outlinedObj)
	{
		if ((outlinedObj == nullptr) || (outlinedObj->GetComponent<Pg::Data::RendererBase3D>() == nullptr))
		{
			_finalRenderer->SetOutlineRenderingMode(false);
			return;
		}

		//Object ID�� ã������, ������ �ִ� GraphicsSceneParser ���θ� ã�ƾ� �Ѵ�...
		UINT tSelectedID = _sceneParser->GetObjIDWithObject(outlinedObj);
		if (tSelectedID != NULL)
		{
			_finalRenderer->SetOutlineRenderingMode(true);
			_finalRenderer->SetObjectIDSelected(tSelectedID);
		}
	}

	void ParagonRenderer::SetDeltaTime(float dt)
	{
		//�̸� Animation�� ���� ������ : ��ŸŸ�� �Ѱ��.
		_deferredRenderer->SetDeltaTime(dt);
		_forward3dRenderer->SetDeltaTime(dt);
	}

	void ParagonRenderer::AddRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_sceneParser->AddRenderObject_Runtime(objVecP);
	}

	void ParagonRenderer::ModifyRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_sceneParser->ModifyRenderObject_Runtime(objVecP);
	}

	void ParagonRenderer::DeleteRenderObject_Runtime(const std::vector<Pg::Data::GameObject*>* objVecP)
	{
		_sceneParser->DeleteRenderObject_Runtime(objVecP);
	}

	void ParagonRenderer::HandleRenderObjectsRuntime()
	{
		_sceneParser->HandleRenderObjectsRuntime();
	}

	Pg::Graphics::VisualEffectRenderer* ParagonRenderer::GetVisualEffectRenderer()
	{
		return _forward3dRenderer->GetVisualEffectRenderer();
	}

	void ParagonRenderer::FadeInOutRender()
	{
		_finalRenderer->RenderFadeInOut();
	}


}