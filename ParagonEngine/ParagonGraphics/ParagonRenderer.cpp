#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"

#include "GraphicsSceneParser.h"
#include "DeferredRenderer.h"
#include "CubemapRenderer.h"
#include "Forward2DRenderer.h"
#include "DebugRenderer.h"
#include "FinalRenderer.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/GameObject.h"

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

	void ParagonRenderer::Initialize()
	{
		//SceneParser 만들고 Initialize();
		_sceneParser = std::make_unique<GraphicsSceneParser>();
		_sceneParser->Initialize();

		//렌더러들 내부에서 오고 갈 GraphicsCarrier 객체 생성.
		_gCarrier = std::make_unique<D3DCarrier>();

		_deferredRenderer = std::make_unique<DeferredRenderer>(_gCarrier.get());
		_deferredRenderer->Initialize();

		_cubemapRenderer = std::make_unique<CubemapRenderer>(_gCarrier.get());
		_cubemapRenderer->Initialize();

		_forward2dRenderer = std::make_unique<Forward2DRenderer>(_gCarrier.get());
		_forward2dRenderer->Initialize();

		_debugRenderer = std::make_unique<DebugRenderer>(_gCarrier.get());
		_debugRenderer->Initialize();

		_finalRenderer = std::make_unique<FinalRenderer>(_gCarrier.get());
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
		_deferredRenderer->RenderContents(_sceneParser->GetRenderObject3DList(), camData);
		_deferredRenderer->ConfirmCarrierData();

		// Cubemap Renderer.
		_cubemapRenderer->RenderContents(_sceneParser->GetRenderObjectCubemapList(), camData);
		_cubemapRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::DebugRender(Pg::Data::CameraData* camData)
	{
		_debugRenderer->RenderContents(_sceneParser->GetRenderObjectWireframeList(), camData);
		_debugRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::UiRender(Pg::Data::CameraData* camData)
	{
		// Forward 2D
		_forward2dRenderer->RenderContents(_sceneParser->GetRenderObject2DList(), camData);
		_forward2dRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::FinalRender(Pg::Data::CameraData* camData)
	{
		_finalRenderer->RenderContents(nullptr, camData);
		_finalRenderer->ConfirmCarrierData();
	}

	void ParagonRenderer::EndRender()
	{
		_DXLogic->Present();
	}

	void ParagonRenderer::SyncComponentToGraphics(const Pg::Data::Scene* const newScene)
	{

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


	//void ParagonRenderer::SyncDebugGeometryToGraphics(const Pg::Data::Scene* const newScene)
	//{
	//
	//}

	void ParagonRenderer::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//ParseSceneData는 브랜치 합치기 전에 SyncComponent로 분리 불가.
		_sceneParser->ParseSceneData(newScene);

		//모든 RenderPass들 셋업하기.
		_deferredRenderer->SetupRenderPasses();
		_cubemapRenderer->SetupRenderPasses();
		_forward2dRenderer->SetupRenderPasses();
		_debugRenderer->SetupRenderPasses();
		_finalRenderer->SetupRenderPasses();
	}


	

}