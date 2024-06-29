#include "VisualEffectRenderer.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include <cassert>

namespace Pg::Graphics
{
	VisualEffectRenderer::VisualEffectRenderer()
	{
		_graphicsResourceManager = Manager::GraphicsResourceManager::Instance();
		_DXStorage = LowDX11Storage::GetInstance();
	}

	VisualEffectRenderer::~VisualEffectRenderer()
	{

	}

	void VisualEffectRenderer::LoadAllEffects(const std::vector<Pg::Data::VisualEffectData>& vec)
	{
		//모든 이펙트 로딩.
		for (auto& it : vec)
		{
			auto tReturnVal = _visualEffectsMap.insert(std::make_pair(it._effectName, std::make_unique<VERenderingSet>(it, it._is3dSpace)));
			auto& bVESetPtr = tReturnVal.first->second;
			const auto& tEffectData = tReturnVal.first->second->_visualEffectData;

			//3D Space 소속여부로 다른 로드.
			if (tEffectData._is3dSpace) { Load3DSpaceEffect(bVESetPtr.get()); }
			else { Load2DSpaceEffect(bVESetPtr.get()); }

			
			//여기까지 VisualEffectGraphicsSet 세팅.

			///SpriteBatch / GeometricPrimitive 만들기. 추후에 변경 사항에 따라서 수정되어야 한다.
			//bVESetPtr->_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);

			///여기서, 개별적인 SpriteBatch 설정에 따른 설정을 해줘야 할 것이다.
		}

		//또한, 만들어진 current rendering map 역시 위에 기반한 메모리 마련.
		for (auto& it : _visualEffectsMap)
		{
			_currentRenderingMap.insert(std::make_pair(it.second.get(), std::vector<Pg::Data::VisualEffectRenderObject*>()));
		}

		//그냥 1 이펙트당 1 SpriteBatch 상태가 반영된다고 생각하자.
	}

	void VisualEffectRenderer::RegisterVisualEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		assert(_visualEffectsMap.contains(effectName) && "무조건 이미 로드된 EffectName을 가지고 로드해야 한다");
		//Current Rendering Map에 Visual Effect Map 추가.
		_currentRenderingMap.at(_visualEffectsMap.at(effectName).get()).push_back(vfxObj);
	}

	void VisualEffectRenderer::RemoveVisualEffectObject(Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		//겹치면 삭제하기.
		for (auto& [bSetPtr, bVec] : _currentRenderingMap)
		{
			//같으면 삭제.
			bVec.erase(std::remove_if(bVec.begin(), bVec.end(), [vfxObj](Pg::Data::VisualEffectRenderObject* dat) {
				return dat == vfxObj; }), bVec.end());
		}
	}

	void VisualEffectRenderer::ResetVisualEffectObjects()
	{
		//어차피 Logic 후 Rendering이니, 스코프 관련 문제가 발생하지 않는다.
		for (auto& [bSetPtr, bVec] : _currentRenderingMap)
		{
			if (!bVec.empty())
			{
				bVec.clear();
			}
		}
	}

	void VisualEffectRenderer::Render()
	{
		for (auto& [bRenderSet, bEffectObjectVec] : _currentRenderingMap)
		{
			//Render Set & Effect Vector.
			//여기서 상태에 따라서 Render State를 다르게 지정하는 등 시행할 수 있을 것이다.
			
			//X,Y,Z 모두를 써 E출력할지, 2차원으로 처리할 지를 판단한다.
			bool tIs3d = bRenderSet->_visualEffectData._is3dSpace; 
			//3D의 경우 의미가 있는 설정이다. 3D공간에 있어도 자체 Rotation을 무시하고 카메라 바라보리를 결정.
			bool tIsFaceCamera = bRenderSet->_visualEffectData._isFaceCamera; 
			//프레임을 받아들여 인덱스를 가지고 AnimatedTexture처럼 출력해야 하는지를 구분.
			bool tIsSpriteSheet = bRenderSet->_visualEffectData._isSpriteSheet;

			//커스텀 셰이더 여부 + 등등을 해결해야 한다.
			if (tIs3d)
			{


			}
			else
			{

			}
			bRenderSet->_spriteBatch->Draw();
			for (auto& bEffectObject : bEffectObjectVec)
			{
				//개별적인 Render Object 단계.
			}



		}
	}

	void VisualEffectRenderer::Load2DSpaceEffect(VERenderingSet* veSet)
	{
		auto& tVisualEffectGraphicsSet = veSet->_veGraphicsSet2D;
		auto& tEffectData = veSet->_visualEffectData;

		//여기부터 VisualEffectGraphicsSet 세팅.
		tVisualEffectGraphicsSet->_renderTexture = static_cast<RenderTexture2D*>(_graphicsResourceManager->GetResourceByName
		(tEffectData._textureName, Pg::Data::Enums::eAssetDefine::_TEXTURE2D).get());
		assert(tVisualEffectGraphicsSet->_renderTexture != nullptr);

		if (tEffectData._isUseCustomVertexShader)
		{
			tVisualEffectGraphicsSet->_customRenderVertexShader = static_cast<RenderVertexShader*>(_graphicsResourceManager->GetResourceByName
			(tEffectData._customVertexShaderName, Pg::Data::Enums::eAssetDefine::_RENDER_VERTEXSHADER).get());
		}

		if (tEffectData._isUseCustomPixelShader)
		{
			tVisualEffectGraphicsSet->_customRenderPixelShader = static_cast<RenderPixelShader*>(_graphicsResourceManager->GetResourceByName
			(tEffectData._customPixelShaderName, Pg::Data::Enums::eAssetDefine::_RENDER_PIXELSHADER).get());
		}
	}

	void VisualEffectRenderer::Load3DSpaceEffect(VERenderingSet* veSet)
	{
		auto& tVisualEffectGraphicsSet = veSet->_veGraphicsSet3D;
		auto& tEffectData = veSet->_visualEffectData;
		//내부적으로 Mesh인지, 아닌지 역시 구분해야 한다. 
		//Mesh라면, Asset3DModelData 가져와서 렌더해야 한다.
		//해당 정보 가져와서 렌더하게 하면 끝나는 문제.
		if(tEffectData._
	}

	//void VisualEffectRenderer::Render3dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj)
	//{
	//
	//}
	//
	//void VisualEffectRenderer::Render2dSpaceQuadEffect(VERenderingSet* veSet, Pg::Data::VisualEffectRenderObject* veObj)
	//{
	//
	//}

}
