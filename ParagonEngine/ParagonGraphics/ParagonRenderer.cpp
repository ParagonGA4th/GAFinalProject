#include "ParagonRenderer.h"

#include "LowDX11Logic.h"
#include "LowDX11Storage.h"
#include "GraphicsResourceHelper.h"
#include "GraphicsResourceManager.h"
#include "MathHelper.h"

#include "GraphicsSceneParser.h"
#include "DeferredRenderer.h"
#include "Forward3DRenderer.h"
#include "Forward2DRenderer.h"
#include "DebugRenderer.h"

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
		
		_deferredRenderer = std::make_unique<DeferredRenderer>();
		_deferredRenderer->Initialize();

		_forward3dRenderer = std::make_unique<Forward3DRenderer>();
		_forward3dRenderer->Initialize();

		_forward2dRenderer = std::make_unique<Forward2DRenderer>();
		_forward2dRenderer->Initialize();

		_debugRenderer = std::make_unique<DebugRenderer>();
		_debugRenderer->Initialize();

		//SkinningMk.2
		//_tempMultiMesh = new MultimaterialMesh("tFilePath");
	}

	void ParagonRenderer::BeginRender()
	{
		//Depth Stencil State 디폴트 상태로 바꾸기. 
		_DXStorage->_deviceContext->OMSetDepthStencilState(_DXStorage->_depthStencilState, 0);

		//ClearDepthStencilView. Depth Buffer // Stencil Buffer 지우기.
		_DXStorage->_deviceContext->ClearDepthStencilView(_DXStorage->_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

		//Render Target을 디폴트로 설정.
		_DXStorage->_deviceContext->OMSetRenderTargets(1, &(_DXStorage->_mainRTV), _DXStorage->_depthStencilView);
	}

	void ParagonRenderer::Render(Pg::Data::CameraData* camData)
	{	
		// Deferred w/ Pass
		_deferredRenderer->Render(_sceneParser->GetRenderObject3DList(), camData);

		// Forward 3D
		_forward3dRenderer->Render(_sceneParser->GetRenderObjectCubemapList(), 0, camData);

		// Forward 2D
		_forward2dRenderer->Render(_sceneParser->GetRenderObject2DList(), camData);
	}

	void ParagonRenderer::DebugRender(Pg::Data::CameraData* camData)
	{
		_debugRenderer->Render(_sceneParser->GetRenderObjectWireframeList(), camData);
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

	void ParagonRenderer::ParseSceneData(const Pg::Data::Scene* const newScene)
	{
		//ParseSceneData는 브랜치 합치기 전에 SyncComponent로 분리 불가.
		_sceneParser->ParseSceneData(newScene);
	}
}