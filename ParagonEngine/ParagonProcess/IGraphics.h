#pragma once

#include "../ParagonData/Scene.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/AssetDefines.h"
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

namespace Pg::Data
{
	class Scene;
}

namespace Pg::Core
{
	class IGraphics abstract
	{
	public:
		virtual void Initialize(HWND hWnd, int screenWidth, int screenHeight) abstract;
		
		//그래픽스 리소스, 애셋 매니저와 연동.
		virtual void SyncLoadGraphicsResources() abstract; 

		virtual void Update(const Pg::Data::Scene* const scene, Pg::Data::CameraData* cameraData, float deltaTime) abstract;
		virtual void BeginRender() abstract;
		virtual void Render(Pg::Data::Scene* scene) abstract;
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
	};
}