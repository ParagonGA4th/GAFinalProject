#include "VisualEffectRenderer.h"
#include "GraphicsResourceManager.h"
#include "LowDX11Storage.h"
#include "../ParagonData/AssetDefines.h"
#include <cassert>

//Custom DXTK Effects.
#include "OneTextureEffect3D.h"
#include "TwoTextureEffect3D.h"
#include "ThreeTextureEffect3D.h"

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
			auto tReturnVal = _visualEffectsMap.insert(std::make_pair(it._effectName, std::make_unique<VERenderingSet>(it)));
			auto& bVESetPtr = tReturnVal.first->second;
			const auto& tEffectData = tReturnVal.first->second->_visualEffectData;

			LoadSingleEffect(bVESetPtr.get());
			////3D Space 소속여부로 다른 로드.
			//if (tEffectData._is3dSpace) { Load3DSpaceEffect(bVESetPtr.get()); }
			//else { Load2DSpaceEffect(bVESetPtr.get()); }

			
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
			//bRenderSet->_spriteBatch->Draw();
			for (auto& bEffectObject : bEffectObjectVec)
			{
				//개별적인 Render Object 단계.
			}



		}
	}

	void VisualEffectRenderer::LoadSingleEffect(VERenderingSet* veSet)
	{
		auto& tVisualEffectGraphicsSet = veSet->_veGraphicsSet;
		auto& tEffectData = veSet->_visualEffectData;

		//Texture : 하나인지, 여러 개인지를 알려야 한다.
		//ImageRenderer와 동일한 방식, 다만, 이건 이름으로 반복하는 것.
		{
			auto& bTexture2DVector = tVisualEffectGraphicsSet->_renderTextureVec;
			auto& bTextureInputNames = tEffectData._textureName;

			//HOTFIX. 이제 Image Path Data는 우선 '^'이 들어 있는지부터 확인.
			if (bTextureInputNames.find('^') == std::string::npos)
			{
				//캐럿 못 찾음 - 1개만 들어있는 것.
				//Image 데이터를 받기.
				auto tTexture2dData = _graphicsResourceManager->GetResourceByName(bTextureInputNames, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
				bTexture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
			}
			else
			{
				std::string token;
				std::stringstream ss(bTextureInputNames);
				std::vector<std::string> outStringVector;
				//Ex. "^asd.png^ase.png^asf.png" 이런 식으로 path가 존재해야 한다.
				//전부 다 크기가 같아야 동작.
				//캐럿을 기준으로 이미지 받아들이는 거 나누기.
				while (std::getline(ss, token, '^'))
				{
					outStringVector.push_back(token);
				}

				for (auto& it : outStringVector)
				{
					if (it.empty())
					{
						continue;
					}
					//Image 데이터를 받기.
					auto tTexture2dData = _graphicsResourceManager->GetResourceByName(it, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
					bTexture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
				}
			}

			//사이즈 기록.
			veSet->_veGraphicsSet->_textureSize = bTexture2DVector.size();
			assert(veSet->_veGraphicsSet->_textureSize != 0);
		}

		//이 두개, 만약 Nullptr면 기본을 사용하는 것.
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

		//3D 공간에서의 Plane.
		if (tEffectData._is3dSpace)
		{
			unsigned int tTextureSize = veSet->_veGraphicsSet->_textureSize;
			auto& bEffect = tVisualEffectGraphicsSet->_effect3D;
			
			//하나라도 Custom을 쓰는지 / 다 디폴트인지.
			if ((!tEffectData._isUseCustomVertexShader) && (!tEffectData._isUseCustomPixelShader))
			{
				switch (tTextureSize)
				{
					//Texture가 하나밖에 없다면, Basic Effect를 넣기. 그렇게 
					case 1: { bEffect = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device); } break;
					case 2: { bEffect = std::make_unique<DirectX::DualTextureEffect>(_DXStorage->_device); } break;
					default: { assert(false && "3개 이상은 DXTK 자체 이펙트 시리즈에서 불가."); } break;
				}
			}
			else
			{
				switch (tTextureSize)
				{
					//Texture가 하나밖에 없다면, Basic Effect를 넣기. 그렇게 
					case 1: { bEffect = std::make_unique<OneTextureEffect3D>(_DXStorage->_device); } break;
					case 2: { bEffect = std::make_unique<TwoTextureEffect3D>(_DXStorage->_device); } break;
					case 3: { bEffect = std::make_unique<ThreeTextureEffect3D>(_DXStorage->_device); } break;
					default: { assert(false && "4개 이상은 불가."); } break;
				}
			}
			
			//InputLayout을 맞게 만들기 위해 - Custom을 사용해도 PosNormalTex는 유지해야 한다.
			void const* shaderByteCode;
			size_t byteCodeLength;
			bEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			HR(_DXStorage->_device->CreateInputLayout(DirectX::VertexPositionNormalTexture::InputElements,
				DirectX::VertexPositionNormalTexture::InputElementCount,
				shaderByteCode, byteCodeLength,
				&(tVisualEffectGraphicsSet->_inputLayout3D)));
		}
		else //2D 공간에서 Plane.
		{

		}



		





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
