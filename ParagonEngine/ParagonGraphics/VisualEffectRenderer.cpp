#include "VisualEffectRenderer.h"
#include "GraphicsResourceManager.h"
#include "GraphicsResourceHelper.h"
#include "LowDX11Storage.h"
#include "D3DCarrier.h"
#include "../ParagonData/AssetDefines.h"
#include <cassert>

//Custom DXTK Effects.
#include "OneTextureEffect3D.h"
#include "TwoTextureEffect3D.h"
#include "ThreeTextureEffect3D.h"

#include "../ParagonUtil/TimeSystem.h"
#include "../ParagonUtil/CustomAssert.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonData/GameConstantData.h"
#include "../ParagonHelper/ResourceHelper.h"

#include "GeometryGenerator.h"
#include "LayoutDefine.h"
#include "RenderTexture2DArray.h"

#include <singleton-cpp/singleton.h>
#include <dxtk/SimpleMath.h>

namespace Pg::Graphics
{
	VisualEffectRenderer::VisualEffectRenderer()
	{
		_graphicsResourceManager = Manager::GraphicsResourceManager::Instance();
		_DXStorage = LowDX11Storage::GetInstance();
		_commonStates = std::make_unique<DirectX::CommonStates>(_DXStorage->_device);

		_timeSystem = &singleton<Pg::Util::Time::TimeSystem>();
		//_systemSpriteInputLayoutPurposeVS = std::make_unique<SystemVertexShader>();
		//using Pg::Util::Helper::ResourceHelper;
		//std::make_unique<SystemVertexShader>(ResourceHelper::IfReleaseChangeDebugTextW(Pg::Defines::SPRITE_VERTEX_SHADER_TEST_DIRECTORY), ,
		//	LowDX11Storage::GetInstance()->_solidState, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
		VERenderingSet* tVeSet = _visualEffectsMap.at(effectName).get();
		_currentRenderingMap.at(tVeSet).push_back(vfxObj);

		//2D일 때 예외 처리.
		if (tVeSet->_veGraphicsSet->_effect3D != nullptr)
		{
			assert("");
		}
		else
		{
			//디폴트 렌더모드가 아닐 때
			auto& tSprite2D = tVeSet->_veGraphicsSet->_spriteEffect2D;
			if (!(tSprite2D->_isDefaultRenderMode))
			{
				if (tSprite2D->_animatedTexture != nullptr)
				{
					//Animated Reset.
					tSprite2D->_animatedTexture->Reset();
				}
				else
				{
					//Scrolling Background.
					tSprite2D->_scrollingBackground->Reset();
				}
			}
		}
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

	void VisualEffectRenderer::Render(const D3DCarrier* carrier, Pg::Data::CameraData* camData)
	{
		//Depth Stencil Setting.
		//Quad의 Depth랑 합쳐서 출력되어야 한다. 
		//이는 밖에서 처리된다. 

		for (auto& [bRenderSet, bEffectObjectVec] : _currentRenderingMap)
		{
			if (bEffectObjectVec.empty())
			{
				continue;
			}

			//Render Set & Effect Vector.
			//여기서 상태에 따라서 Render State를 다르게 지정하는 등 시행할 수 있을 것이다.
			DirectX::XMVECTOR tCamPos = PG2XM_FLOAT3_VECTOR(camData->_position);

			//X,Y,Z 모두를 써 E출력할지, 2차원으로 처리할 지를 판단한다.
			bool tIs3d = bRenderSet->_visualEffectData._is3dSpace;
			//3D의 경우 의미가 있는 설정이다. 3D공간에 있어도 자체 Rotation을 무시하고 카메라 바라보리를 결정.
			bool tIsFaceCamera = bRenderSet->_visualEffectData._isFaceCamera;
			//프레임을 받아들여 인덱스를 가지고 AnimatedTexture처럼 출력해야 하는지를 구분.
			Pg::Data::eSpriteMode tIsSpriteSheet = bRenderSet->_visualEffectData._spriteMode;

			//커스텀 셰이더 여부 + 등등을 해결해야 한다.
			if (tIs3d)
			{
				if (bRenderSet->_visualEffectData._ignoreDepth)
				{
					//설정.
					_DXStorage->_deviceContext->OMSetRenderTargets(1, &(carrier->_quadMainRT->GetRTV()), nullptr);
				}
				else
				{
					//설정.
					_DXStorage->_deviceContext->OMSetRenderTargets(1, &(carrier->_quadMainRT->GetRTV()), carrier->_gBufRequiredInfoDSV->GetDSV());
				}

				auto& bRenderEffect3D = bRenderSet->_veGraphicsSet->_effect3D;
				auto& bStoreMatForm = bRenderSet->_veGraphicsSet->_effectStoreMatrixForm;
				auto& bBasicEffectMaybe = bRenderSet->_veGraphicsSet->_dxtkBasicEffect;
				auto& bCustomMaybe = bRenderSet->_veGraphicsSet->_baseCustomEffect;

				DirectX::XMVECTOR tGlobalUp = PG2XM_FLOAT3_VECTOR(Pg::Math::PGFLOAT3::GlobalUp());
				DirectX::XMMATRIX tWorldMat = DirectX::XMMatrixIdentity();
				DirectX::XMMATRIX tViewMat = DirectX::XMMatrixIdentity();
				DirectX::XMMATRIX tProjMat = PG2XM_MATRIX4X4(camData->_projMatrix);
				//이걸로 View를 설정.
				//XMMatrixLookAtLH

				//모두 같은 레이아웃을 3D 기준으로 사용할 것이기에, 세팅.
				_DXStorage->_deviceContext->IASetInputLayout(bRenderSet->_veGraphicsSet->_inputLayout);

				if (bRenderSet->_visualEffectData._isAlphaBlended)
				{
					_DXStorage->_deviceContext->OMSetBlendState(_commonStates->AlphaBlend(), NULL, 0xffffffff);
				}
				else
				{
					_DXStorage->_deviceContext->OMSetBlendState(_commonStates->NonPremultiplied(), NULL, 0xffffffff);
				}

				//Texture 투입.
				//BasicEffect거나 / 우리가 만든 BaseCustomEffect의 자식일 수도 있다.
				if (bBasicEffectMaybe != nullptr)
				{
					//빌보드 애니메이션 여부에 따라서 달라진다.
					auto& bIsBillboardAnim = bRenderSet->_veGraphicsSet->_isBillboardAnimation;
					if (bIsBillboardAnim)
					{
						///여기 해야 한다.
						unsigned int tCurrentFrame = bRenderSet->_veGraphicsSet->_currentTextureFrame;
						bBasicEffectMaybe->SetTexture(bRenderSet->_veGraphicsSet->_renderTextureVec.at(tCurrentFrame)->GetSRV());

						//Animated Logic. 
						bRenderSet->_veGraphicsSet->_recordedTime += _timeSystem->GetDeltaTime();
						if (bRenderSet->_veGraphicsSet->_recordedTime >= bRenderSet->_veGraphicsSet->_changeBaseTime)
						{
							//RecordedTime Reset.
							bRenderSet->_veGraphicsSet->_recordedTime = 0.f;
							if (tCurrentFrame >= bRenderSet->_veGraphicsSet->_textureSize - 1)
							{
								tCurrentFrame = 0;
							}
							else
							{
								tCurrentFrame++;
							}
						}
					}
					else
					{
						//DXTK의 기본적인 이펙트를 가지고 만들어졌다. DirectX::BasicEffect.
						//무조건 한개일 것이다 이러면 Texture.
						unsigned int tCurrentFrame = 0;
						if (bRenderSet->_visualEffectData._manualSwitchingMode)
						{
							tCurrentFrame = bRenderSet->_veGraphicsSet->_currentTextureFrame;
						}
						bBasicEffectMaybe->SetTexture(bRenderSet->_veGraphicsSet->_renderTextureVec.at(tCurrentFrame)->GetSRV());
					}
					bBasicEffectMaybe->SetAlpha((bRenderSet->_visualEffectData._alphaPercentage / 100.f));
				}
				else
				{
					//BaseCustomEffect의 자식들이다.
					assert(bCustomMaybe != nullptr); //당연히 로직 상 이렇게 들어왔어야.
					for (unsigned int i = 0; i < bCustomMaybe->GetTextureCount(); i++)
					{
						//개수대로 값을 투입.
						bCustomMaybe->SetTexture(i, bRenderSet->_veGraphicsSet->_renderTextureVec.at(i)->GetSRV());
					}
				}

				// Bind Buffers
				UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
				UINT offset = 0;

				//VB / IB / Topology 세팅. 다 쿼드라서 매번 바꿀 필요 X.
				//누워있는지, 아닌지만 보자.
				if (bRenderSet->_visualEffectData._isQuadLyingDown)
				{
					//누워 있다.
					_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::LYING_DOWN_QUAD_VB), &stride, &offset);
					_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::LYING_DOWN_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
				}
				else
				{
					//서 있다, 디폴트처럼.
					_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
					_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
				}

				_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//3D Rendering 전담.
				for (auto& bEffectObject : bEffectObjectVec)
				{
					if (!(bEffectObject->GetActive())) { continue; }

					//Pg::Math::PGQuaternion tFaceDir = bEffectObject->_rotation;
					if (bRenderSet->_visualEffectData._isFaceCamera)
					{
						{
							//using namespace DirectX::SimpleMath;
							using namespace DirectX::SimpleMath;

							XMVECTOR playerPosition = PG2XM_FLOAT3_VECTOR(camData->_position);
							XMVECTOR objectPosition = PG2XM_FLOAT3_VECTOR(bEffectObject->_position);
							XMVECTOR v = objectPosition - playerPosition;
							Vector3 tSubVec = XMVectorSet(0.f, XMVectorGetY(v), 0.f, 0.f);
							

							Vector3 tObjPos = { bEffectObject->_position.x, bEffectObject->_position.y, bEffectObject->_position.z };
							Vector3 tCameraPos = { camData->_position.x, camData->_position.y, camData->_position.z };
							Vector3 tRotateAxis{ 0,1,0 };

							Pg::Math::PGFLOAT3 tFor = GetForwardVectorFromQuat(camData->_rotation);
							Vector3 tCameraForward = { tFor.x, tFor.y, tFor.z };
							Vector3 tObjectForward = tCameraPos + (tSubVec * 10.f);
							tObjectForward.Normalize();

							Matrix tTrans = Matrix::CreateConstrainedBillboard(tObjPos, tCameraPos, tRotateAxis, &tCameraForward, &tObjectForward);
							XMVECTOR tOutScale;
							XMVECTOR tOutQuat;
							XMVECTOR tOutPos;
							CustomAssert(XMMatrixDecompose(&tOutScale, &tOutQuat, &tOutPos, tTrans));

							bEffectObject->_position = Pg::Math::XM2PG_FLOAT3_VECTOR(tOutPos);
							bEffectObject->_rotation = Pg::Math::XM2PG_QUATERNION(tOutQuat);
							//bEffectObject->_scale = Pg::Math::XM2PG_FLOAT3_VECTOR(tOutScale);
							//Scale은 반영하지 않음.
						}

					}

					//개별적인 Render Object 단계.
					//tWorldMat = PG2XM_MATRIX4X4(Pg::Math::PGGetWorldMatrixFromValues(
					//	bEffectObject->_position, tFaceDir, bEffectObject->_scale));

					tWorldMat = PG2XM_MATRIX4X4(Pg::Math::PGGetWorldMatrixFromValues(
						bEffectObject->_position, bEffectObject->_rotation, bEffectObject->_scale));

					tViewMat = PG2XM_MATRIX4X4(camData->_viewMatrix);

					//Matrix Setting을 한다.
					bStoreMatForm->SetMatrices(tWorldMat, tViewMat, tProjMat);
					bRenderEffect3D->Apply(_DXStorage->_deviceContext);

					//여기서 Draw해야 한다. Quad를.
					_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
				}
			}
			else
			{
				//설정.
				_DXStorage->_deviceContext->OMSetRenderTargets(1, &(carrier->_quadMainRT->GetRTV()), carrier->_gBufRequiredInfoDSV->GetDSV());

				//2D.
				auto& bVeSet = bRenderSet->_veGraphicsSet;
				auto& bSpriteEffect2D = bVeSet->_spriteEffect2D;
				auto& tEffectData = bRenderSet->_visualEffectData;

				if (tEffectData._spriteMode == Pg::Data::_SCROLLING_BG)
				{
					bVeSet->_spriteBatch->Begin(
						DirectX::SpriteSortMode_BackToFront);

					for (auto& bEffectObject : bEffectObjectVec)
					{
						if (!(bEffectObject->GetActive())) { continue; }
						//Scroll 속도는 일단 FIX.
						bVeSet->_spriteEffect2D->_scrollingBackground->Update(_timeSystem->GetDeltaTime() * 10.f);
						bVeSet->_spriteEffect2D->_scrollingBackground->Draw(bVeSet->_spriteBatch.get());
					}

					bVeSet->_spriteBatch->End();
				}
				else
				{
					bVeSet->_spriteBatch->Begin(
						DirectX::SpriteSortMode_BackToFront, bVeSet->_customBlendState,
						nullptr, nullptr, nullptr, [&bVeSet]
						{
							bVeSet->_spriteEffect2D->SetCustomShaderInfo();
						}
					);

					if (tEffectData._spriteMode == Pg::Data::_DEFAULT)
					{
						for (auto& bEffectObject : bEffectObjectVec)
						{
							if (!(bEffectObject->GetActive())) { continue; }

							auto& tFirstTexture = bVeSet->_renderTextureVec.at(0);
							//RECT outRect = { 0,0, tFirstTexture->GetFileWidth(), tFirstTexture->GetFileHeight() };

							//기본 설정으로 그리기, Rotation / Scale 반영하지 않음.
							bVeSet->_spriteBatch->Draw(
								tFirstTexture->GetSRV(), DirectX::XMFLOAT2(bEffectObject->_position.x, bEffectObject->_position.y));
						}
					}
					else if (tEffectData._spriteMode == Pg::Data::_SPRITE_SHEET)
					{
						for (auto& bEffectObject : bEffectObjectVec)
						{
							if (!(bEffectObject->GetActive())) { continue; }

							const float ACCELERATE_FACTOR = 10.f;
							bVeSet->_spriteEffect2D->_animatedTexture->Update(_timeSystem->GetDeltaTime() * ACCELERATE_FACTOR);
							bVeSet->_spriteEffect2D->_animatedTexture->Draw(bVeSet->_spriteBatch.get(), DirectX::XMFLOAT2(bEffectObject->_position.x, bEffectObject->_position.y));
						}
					}
					else
					{
						assert(false && "미정의");
					}

					bVeSet->_spriteBatch->End();
				}
			}
			//bRenderSet->_spriteBatch->Draw();

			_DXStorage->_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			//BlendState Reset.
			_DXStorage->_deviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
		}
	}

	void VisualEffectRenderer::LoadSingleEffect(VERenderingSet* veSet)
	{
		auto& tVisualEffectGraphicsSet = veSet->_veGraphicsSet;
		auto& tEffectData = veSet->_visualEffectData;

		bool _IsPGT2ARR = false;
		//Texture : 하나인지, 여러 개인지를 알려야 한다.
		//ImageRenderer와 동일한 방식, 다만, 이건 이름으로 반복하는 것.
		{
			auto& bTexture2DVector = tVisualEffectGraphicsSet->_renderTextureVec;
			auto& bTextureInputNames = tEffectData._textureName;

			//PGT2ARR인지, 아닌지 구별해야 한다.
			//확장자 구별. -> PGT2ARR은 SpriteAnimation 용도 / 그냥 ^연속으로 기록시, One-Two-Three 시리즈.
			if (bTextureInputNames.find(".pgt2arr") != std::string::npos)
			{
				//Texture2DArray 형태로 돌아옴.
				auto tTemp = _graphicsResourceManager->GetResourceByName(bTextureInputNames, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
				RenderTexture2DArray* tTexture2dDataArray = static_cast<RenderTexture2DArray*>(tTemp.get());
				bTexture2DVector = tTexture2dDataArray->GetSingleRenderTexture2DArray(); //PGT2ARR에서 Texture2D 꺼내오기. 
				_IsPGT2ARR = true;
			}
			else
			{
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

				_IsPGT2ARR = false;
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

		//Alpha Blend 활용 <-> Alpha Clip 활용 BlendState. 
		if (tEffectData._isAlphaBlended) { tVisualEffectGraphicsSet->_customBlendState = _commonStates->AlphaBlend(); }
		else { tVisualEffectGraphicsSet->_customBlendState = _commonStates->NonPremultiplied(); }

		//3D 공간에서의 Plane.
		if (tEffectData._is3dSpace)
		{
			unsigned int tTextureSize = veSet->_veGraphicsSet->_textureSize;
			auto& bEffect = tVisualEffectGraphicsSet->_effect3D;
			auto& bStoreMatrixForm = tVisualEffectGraphicsSet->_effectStoreMatrixForm;

			//하나라도 Custom을 쓰는지 / 다 디폴트인지.
			if ((!tEffectData._isUseCustomVertexShader) && (!tEffectData._isUseCustomPixelShader))
			{
				//switch (tTextureSize)
				//{
				//	//Texture가 하나밖에 없다면, Basic Effect를 넣기. 그렇게	
				//	case 1:
				//	{
				//		
				//	}
				//	break;
				//	default:
				//	{
				//		//Dual도 안되게 해놓았다.
				//		//이제는 Basic Effect 스위칭 가능하게.
				//		//assert(false && "2개 이상은 DXTK 자체 이펙트 시리즈에서 불가.");
				//
				//
				//
				//
				//
				//
				//
				//	}
				//	break;
				//}
				bEffect = std::make_unique<DirectX::BasicEffect>(_DXStorage->_device);
				DirectX::BasicEffect* tBasicEffect = static_cast<DirectX::BasicEffect*>(bEffect.get());
				tBasicEffect->SetTextureEnabled(true);
				tBasicEffect->SetLightingEnabled(false);
				//나중에 여기다가 Emission Default로 설정할 수도 있다.

				bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(tBasicEffect);

				//BasicEffect이니, 이를 기록.
				tVisualEffectGraphicsSet->_dxtkBasicEffect = tBasicEffect;

				//PGT2ARR인지 기록 -> 유일하게 Billboard Sprite Animation을 서포트할 것이다.
				tVisualEffectGraphicsSet->_isBillboardAnimation = _IsPGT2ARR;
			}
			else
			{
				auto& bBaseCustomEffect = tVisualEffectGraphicsSet->_baseCustomEffect;

				if (_IsPGT2ARR)
				{
					//이건 Support 안 할 것이다.
					assert(false);
				}
				else
				{
					switch (tTextureSize)
					{
						//Texture가 하나밖에 없다면, Basic Effect를 넣기. 그렇게 
						case 1:
						{
							bEffect = std::make_unique<OneTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<OneTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//별도로 기록.
							bBaseCustomEffect = bCustom;
						}
						break;
						case 2:
						{
							bEffect = std::make_unique<TwoTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<TwoTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//별도로 기록.
							bBaseCustomEffect = bCustom;
						}
						break;
						case 3:
						{
							bEffect = std::make_unique<ThreeTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<ThreeTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//별도로 기록.
							bBaseCustomEffect = bCustom;
						}
						break;
						default:
						{
							assert(false && "4개 이상은 불가.");
						}
						break;
					}
				}
			}

			//InputLayout을 맞게 만들기 위해 - Custom을 사용해도 PosNormalTex는 유지해야 한다.
			//일단, 값 유지.
			void const* shaderByteCode;
			size_t byteCodeLength;
			bEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength); // 모든 Effect 이 코드 소화 가능.

			HR(_DXStorage->_device->CreateInputLayout(DirectX::VertexPositionNormalTexture::InputElements,
				DirectX::VertexPositionNormalTexture::InputElementCount,
				shaderByteCode, byteCodeLength,
				&(tVisualEffectGraphicsSet->_inputLayout)));

			//이제, 값 정하기.
			//Alpha Blend != Alpha Clip.
			//나중에 Apply 적용 시, 값은 밖에서 되는 것이 아니라, Effect 안에서 되어야 한다. (VEGraphicSet 접근을)
			// 또한, CustomVertexShader / CustomPixelShader는 둘 다 한꺼번에 쓰거나 / 말거나해야!

		}
		else //2D 공간에서 Plane.
		{
			//ScreenSpace에서 적용되는 공간.
			//적용할 BlendState 적용.	
			tVisualEffectGraphicsSet->_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);
			tVisualEffectGraphicsSet->_spriteEffect2D = std::make_unique<SpriteEffect2D>(veSet);
			//나머지는 렌더할 때 적용해야 할 것 같다.
			tVisualEffectGraphicsSet->_inputLayout = LayoutDefine::GetSpriteCustomLayout();

			//Custom 2D Shader 양식 :
			// https://raw.githubusercontent.com/wiki/Microsoft/DirectXTK/shaders/BloomExtract.hlsl
			// https://github.com/microsoft/DirectXTK/wiki/Writing-custom-shaders
			//Texture2D<float4> Texture : register(t0);
			//sampler TextureSampler : register(s0);
			//float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0

			//만약 SpriteSheet이라면, 다른 클래스로 등록.
			//다른 셰이더를 넣어주는 것으로 해결할 수 있을 것.

			auto& bSpriteEffect2D = tVisualEffectGraphicsSet->_spriteEffect2D;
			if (tEffectData._spriteMode == Pg::Data::_DEFAULT)
			{
				//디폴트 렌더 모드.
				bSpriteEffect2D->_isDefaultRenderMode = true;
			}
			else if (tEffectData._spriteMode == Pg::Data::_SPRITE_SHEET)
			{
				//SpriteSheet 모드.
				bSpriteEffect2D->_animatedTexture = std::make_unique<AnimatedTexture>();
				//일단 Default Frames Per Second = 2.
				bSpriteEffect2D->_animatedTexture->Load(tVisualEffectGraphicsSet->_renderTextureVec.at(0)->GetSRV(), tEffectData._frameCount, 2);
				//미리 로드.

			}
			else if (tEffectData._spriteMode == Pg::Data::_SCROLLING_BG)
			{
				//Scrolling Background 모드.
				bSpriteEffect2D->_scrollingBackground = std::make_unique<ScrollingBackground>();
				bSpriteEffect2D->_scrollingBackground->Load(tVisualEffectGraphicsSet->_renderTextureVec.at(0)->GetSRV());
				bSpriteEffect2D->_scrollingBackground->SetWindow(Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT);
			}
			else
			{
				assert(false && "미정의");
			}
		}
	}

	unsigned int* VisualEffectRenderer::GetEffectTextureIndexPointer(const std::string& effectName)
	{
		if (_visualEffectsMap.contains(effectName))
		{
			auto& bSet = _visualEffectsMap.at(effectName);

			unsigned int* tRet = nullptr;
			if (bSet->_visualEffectData._manualSwitchingMode)
			{
				tRet = &(bSet->_veGraphicsSet->_currentTextureFrame);
			}
			return tRet;
		}
		else
		{
			return nullptr;
		}
	}

}
