#pragma once
// #GraphicsTBA : 메모리 추가 할당을 막기 위해, Scene당 렌더오브젝트 생성 로직 중복을 막아야 한다! 또한, 한 오브젝트 안에 여러 개의 렌더러가 들어가는 로직 역시 막아야 할 것.

#include "RenderObject2D.h"
#include "RenderObject3D.h"

#include "../ParagonData/CameraData.h"
#include "../ParagonData/GameObject.h"

#include <memory>

/// <summary>
/// Paragon Renderer
/// 
/// 2023. 09. 12.
/// </summary>

namespace Pg::Data
{
	class Scene;
	class GameObject;
	class RendererChangeList;
}

namespace Pg::Graphics
{
	class LowDX11Logic;
	class LowDX11Storage;

	class ParagonRenderer
	{
	public:
		ParagonRenderer();
		~ParagonRenderer();

		//Scene이 바뀌었을 때 / 게임 엔진이 처음 시작되었을 때 호출되어야 한다.
		//메모리 추가 할당을 막기 위해, Scene당 렌더오브젝트 생성 로직 중복을 막아야 한다!
		void OnNewSceneStart(Pg::Data::Scene* newScene);

		//ParagonRenderer에 연동 처리를 맡겼다.
		void SyncComponentToGraphics();

		void BeginRender();
		void Render(Pg::Data::CameraData camData); //이미 컴포넌트 단계에서 RenderObject들과 연동되기에, 오브젝트 자체를 받을 필요가 없음.
		void EndRender();

	private:
		LowDX11Storage* _DXStorage = nullptr;
		LowDX11Logic* _DXLogic = nullptr;

		//저장된 GameObject <=> RenderObject들.
		std::unordered_map<Pg::Data::GameObject*, std::unique_ptr<RenderObject2D>> _renderObject2DList;
		std::unordered_map<Pg::Data::GameObject*, std::unique_ptr<RenderObject3D>> _renderObject3DList;
		
		Pg::Data::RendererChangeList* _rendererChangeList = nullptr;
	};
}

