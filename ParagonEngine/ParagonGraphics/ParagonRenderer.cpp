#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"
#include "SceneInformationList.h"

#include "GraphicsSceneParser.h"
#include "DeferredRenderer.h"
#include "CubemapRenderer.h"
#include "Forward2DRenderer.h"
#include "Forward3DRenderer.h"
#include "DebugRenderer.h"
#include "PPFinalRenderer.h"


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

	void ParagonRenderer::Initialize(const Pg::Data::Enums::eEditorMode* const editorMode)
	{
		//SceneParser 만들고 Initialize();
		_sceneParser = std::make_unique<GraphicsSceneParser>();
		_sceneParser->Initialize();

		//렌더러들 내부에서 오고 갈 GraphicsCarrier 객체 생성.
		_gCarrier = std::make_unique<D3DCarrier>();

		_deferredRenderer = std::make_unique<DeferredRenderer>(_gCarrier.get(), editorMode);
		_deferredRenderer->Initialize();

		_cubemapRenderer = std::make_unique<CubemapRenderer>(_gCarrier.get());
		_cubemapRenderer->Initialize();

		_forward2dRenderer = std::make_unique<Forward2DRenderer>(_gCarrier.get());
		_forward2dRenderer->Initialize();

		_forward3dRenderer = std::make_unique<Forward3DRenderer>(_gCarrier.get(), editorMode);
		_forward3dRenderer->Initialize();

		_debugRenderer = std::make_unique<DebugRenderer>(_gCarrier.get());
		_debugRenderer->Initialize();

		_finalRenderer = std::make_unique<PPFinalRenderer>(_gCarrier.get());
		_finalRenderer->Initialize();
		//SkinningMk.F
		//_tempMultiMesh = new MultimaterialMesh("tFilePath");


	}

	void ParagonRenderer::BeginRender()
	{
		
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{
		// Deferred w/ Pass
		_deferredRenderer->RenderContents(_sceneParser->GetRenderObject3DList(), _sceneParser->GetSceneInformationList(), camData);
		_deferredRenderer->ConfirmCarrierData();

		_forward3dRenderer->RenderContents(_sceneParser->GetRenderObject3DList(), nullptr, camData);
		_forward3dRenderer->ConfirmCarrierData();

		// Cubemap Renderer.
		_cubemapRenderer->RenderContents(_sceneParser->GetRenderObjectCubemapList(), nullptr, camData);
		_cubemapRenderer->ConfirmCarrierData();
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

	void ParagonRenderer::FinalRender(Pg::Data::CameraData* camData)
	{
		//내부적으로 알아서 구분해줄 것. (Picking에 따라 Outline 모드가 내부적으로 호출될 것이기 때문에!
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
		//현재 ParseSceneData 내부 구현체에 왜 매번 Graphics 객체를 다시 만드는지 모르겠지만..
		//일단 급하니 나중에 TODO.
		ParseSceneData(newScene);
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

	//void ParagonRenderer::SyncDebugGeometryToGraphics(const Pg::Data::Scene* const newScene)
	//{
	//
	//}

	void ParagonRenderer::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//ParseSceneData는 브랜치 합치기 전에 SyncComponent로 분리 불가.
		_sceneParser->ParseSceneData(newScene);


		static bool tDebouncer = true;

		if (tDebouncer)
		{
			//모든 RenderPass들 셋업하기.
			_deferredRenderer->SetupRenderPasses();
			_cubemapRenderer->SetupRenderPasses();
			_forward3dRenderer->SetupRenderPasses(); //얘도 Deferred처럼 분리되어야 하는가?
			_forward2dRenderer->SetupRenderPasses();
			_debugRenderer->SetupRenderPasses();
			_finalRenderer->SetupRenderPasses();

			tDebouncer = false;
		}

		//매번 다른 매터리얼을 로드해야 하는 Deferred는 약간 다를 수 있다.
		_deferredRenderer->SetupOpaqueQuadRenderPasses();
		_deferredRenderer->InitializeOpaqueQuadRenderPasses();

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
	//		//없으니, Outline Buffer를 따로 그려줄 이유도 없다.
	//		_finalRenderer->SetOutlineRenderingMode(false);
	//		return nullptr;
	//	}
	//	
	//	//해당 Material 값을 감싸는 Outline Shader Rendering 과정. -> 그려주게 Bool 값으로 설정.
	//	_finalRenderer->SetOutlineRenderingMode(true);
	//	_finalRenderer->SetObjectIDSelected(tFoundID);
	//
	//	//무조건 내부적으로 값을 찾아서 리턴해야 한다. 그렇지 않은 경우를 외부에서 예외처리함.
	//	return _sceneParser->GetObjectWithObjID(tFoundID);
	//}

	void ParagonRenderer::SetOutlinedGameObject(Pg::Data::GameObject* outlinedObj)
	{
		if ((outlinedObj == nullptr) || (outlinedObj->GetComponent<Pg::Data::RendererBase3D>() == nullptr))
		{
			_finalRenderer->SetOutlineRenderingMode(false);
			return;
		}

		//Object ID를 찾으려면, 기존에 있는 GraphicsSceneParser 내부를 찾아야 한다...
		UINT tSelectedID = _sceneParser->GetObjIDWithObject(outlinedObj);
		if (tSelectedID != NULL)
		{
			_finalRenderer->SetOutlineRenderingMode(true);
			_finalRenderer->SetObjectIDSelected(tSelectedID);
		}
	}

	void ParagonRenderer::SetDeltaTime(float dt)
	{
		//미리 Animation을 돌릴 렌더러 : 델타타임 넘겼다.
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

	
	
	

}