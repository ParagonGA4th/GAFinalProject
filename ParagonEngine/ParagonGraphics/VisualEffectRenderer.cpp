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
		//��� ����Ʈ �ε�.
		for (auto& it : vec)
		{
			auto tReturnVal = _visualEffectsMap.insert(std::make_pair(it._effectName, std::make_unique<VERenderingSet>(it)));
			auto& bVESetPtr = tReturnVal.first->second;
			const auto& tEffectData = tReturnVal.first->second->_visualEffectData;

			LoadSingleEffect(bVESetPtr.get());
			////3D Space �Ҽӿ��η� �ٸ� �ε�.
			//if (tEffectData._is3dSpace) { Load3DSpaceEffect(bVESetPtr.get()); }
			//else { Load2DSpaceEffect(bVESetPtr.get()); }


			//������� VisualEffectGraphicsSet ����.

			///SpriteBatch / GeometricPrimitive �����. ���Ŀ� ���� ���׿� ���� �����Ǿ�� �Ѵ�.
			//bVESetPtr->_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);

			///���⼭, �������� SpriteBatch ������ ���� ������ ����� �� ���̴�.
		}

		//����, ������� current rendering map ���� ���� ����� �޸� ����.
		for (auto& it : _visualEffectsMap)
		{
			_currentRenderingMap.insert(std::make_pair(it.second.get(), std::vector<Pg::Data::VisualEffectRenderObject*>()));
		}

		//�׳� 1 ����Ʈ�� 1 SpriteBatch ���°� �ݿ��ȴٰ� ��������.
	}

	void VisualEffectRenderer::RegisterVisualEffectObject(const std::string& effectName, Pg::Data::VisualEffectRenderObject* vfxObj)
	{
		assert(_visualEffectsMap.contains(effectName) && "������ �̹� �ε�� EffectName�� ������ �ε��ؾ� �Ѵ�");
		//Current Rendering Map�� Visual Effect Map �߰�.
		VERenderingSet* tVeSet = _visualEffectsMap.at(effectName).get();
		_currentRenderingMap.at(tVeSet).push_back(vfxObj);

		//2D�� �� ���� ó��.
		if (tVeSet->_veGraphicsSet->_effect3D != nullptr)
		{
			assert("");
		}
		else
		{
			//����Ʈ ������尡 �ƴ� ��
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
		//��ġ�� �����ϱ�.
		for (auto& [bSetPtr, bVec] : _currentRenderingMap)
		{
			//������ ����.
			bVec.erase(std::remove_if(bVec.begin(), bVec.end(), [vfxObj](Pg::Data::VisualEffectRenderObject* dat) {
				return dat == vfxObj; }), bVec.end());
		}
	}

	void VisualEffectRenderer::ResetVisualEffectObjects()
	{
		//������ Logic �� Rendering�̴�, ������ ���� ������ �߻����� �ʴ´�.
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
		//Quad�� Depth�� ���ļ� ��µǾ�� �Ѵ�. 
		//�̴� �ۿ��� ó���ȴ�. 

		for (auto& [bRenderSet, bEffectObjectVec] : _currentRenderingMap)
		{
			if (bEffectObjectVec.empty())
			{
				continue;
			}

			//Render Set & Effect Vector.
			//���⼭ ���¿� ���� Render State�� �ٸ��� �����ϴ� �� ������ �� ���� ���̴�.
			DirectX::XMVECTOR tCamPos = PG2XM_FLOAT3_VECTOR(camData->_position);

			//X,Y,Z ��θ� �� E�������, 2�������� ó���� ���� �Ǵ��Ѵ�.
			bool tIs3d = bRenderSet->_visualEffectData._is3dSpace;
			//3D�� ��� �ǹ̰� �ִ� �����̴�. 3D������ �־ ��ü Rotation�� �����ϰ� ī�޶� �ٶ󺸸��� ����.
			bool tIsFaceCamera = bRenderSet->_visualEffectData._isFaceCamera;
			//�������� �޾Ƶ鿩 �ε����� ������ AnimatedTextureó�� ����ؾ� �ϴ����� ����.
			Pg::Data::eSpriteMode tIsSpriteSheet = bRenderSet->_visualEffectData._spriteMode;

			//Ŀ���� ���̴� ���� + ����� �ذ��ؾ� �Ѵ�.
			if (tIs3d)
			{
				if (bRenderSet->_visualEffectData._ignoreDepth)
				{
					//����.
					_DXStorage->_deviceContext->OMSetRenderTargets(1, &(carrier->_quadMainRT->GetRTV()), nullptr);
				}
				else
				{
					//����.
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
				//�̰ɷ� View�� ����.
				//XMMatrixLookAtLH

				//��� ���� ���̾ƿ��� 3D �������� ����� ���̱⿡, ����.
				_DXStorage->_deviceContext->IASetInputLayout(bRenderSet->_veGraphicsSet->_inputLayout);

				if (bRenderSet->_visualEffectData._isAlphaBlended)
				{
					_DXStorage->_deviceContext->OMSetBlendState(_commonStates->AlphaBlend(), NULL, 0xffffffff);
				}
				else
				{
					_DXStorage->_deviceContext->OMSetBlendState(_commonStates->NonPremultiplied(), NULL, 0xffffffff);
				}

				//Texture ����.
				//BasicEffect�ų� / �츮�� ���� BaseCustomEffect�� �ڽ��� ���� �ִ�.
				if (bBasicEffectMaybe != nullptr)
				{
					//������ �ִϸ��̼� ���ο� ���� �޶�����.
					auto& bIsBillboardAnim = bRenderSet->_veGraphicsSet->_isBillboardAnimation;
					if (bIsBillboardAnim)
					{
						///���� �ؾ� �Ѵ�.
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
						//DXTK�� �⺻���� ����Ʈ�� ������ ���������. DirectX::BasicEffect.
						//������ �Ѱ��� ���̴� �̷��� Texture.
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
					//BaseCustomEffect�� �ڽĵ��̴�.
					assert(bCustomMaybe != nullptr); //�翬�� ���� �� �̷��� ���Ծ��.
					for (unsigned int i = 0; i < bCustomMaybe->GetTextureCount(); i++)
					{
						//������� ���� ����.
						bCustomMaybe->SetTexture(i, bRenderSet->_veGraphicsSet->_renderTextureVec.at(i)->GetSRV());
					}
				}

				// Bind Buffers
				UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
				UINT offset = 0;

				//VB / IB / Topology ����. �� ����� �Ź� �ٲ� �ʿ� X.
				//�����ִ���, �ƴ����� ����.
				if (bRenderSet->_visualEffectData._isQuadLyingDown)
				{
					//���� �ִ�.
					_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::LYING_DOWN_QUAD_VB), &stride, &offset);
					_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::LYING_DOWN_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
				}
				else
				{
					//�� �ִ�, ����Ʈó��.
					_DXStorage->_deviceContext->IASetVertexBuffers(0, 1, &(GeometryGenerator::_QUAD_VB), &stride, &offset);
					_DXStorage->_deviceContext->IASetIndexBuffer(GeometryGenerator::_QUAD_IB, DXGI_FORMAT_R32_UINT, 0);
				}

				_DXStorage->_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//3D Rendering ����.
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
							//Scale�� �ݿ����� ����.
						}

					}

					//�������� Render Object �ܰ�.
					//tWorldMat = PG2XM_MATRIX4X4(Pg::Math::PGGetWorldMatrixFromValues(
					//	bEffectObject->_position, tFaceDir, bEffectObject->_scale));

					tWorldMat = PG2XM_MATRIX4X4(Pg::Math::PGGetWorldMatrixFromValues(
						bEffectObject->_position, bEffectObject->_rotation, bEffectObject->_scale));

					tViewMat = PG2XM_MATRIX4X4(camData->_viewMatrix);

					//Matrix Setting�� �Ѵ�.
					bStoreMatForm->SetMatrices(tWorldMat, tViewMat, tProjMat);
					bRenderEffect3D->Apply(_DXStorage->_deviceContext);

					//���⼭ Draw�ؾ� �Ѵ�. Quad��.
					_DXStorage->_deviceContext->DrawIndexed(GeometryGenerator::QUAD_INDICE_COUNT, 0, 0);
				}
			}
			else
			{
				//����.
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
						//Scroll �ӵ��� �ϴ� FIX.
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

							//�⺻ �������� �׸���, Rotation / Scale �ݿ����� ����.
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
						assert(false && "������");
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
		//Texture : �ϳ�����, ���� �������� �˷��� �Ѵ�.
		//ImageRenderer�� ������ ���, �ٸ�, �̰� �̸����� �ݺ��ϴ� ��.
		{
			auto& bTexture2DVector = tVisualEffectGraphicsSet->_renderTextureVec;
			auto& bTextureInputNames = tEffectData._textureName;

			//PGT2ARR����, �ƴ��� �����ؾ� �Ѵ�.
			//Ȯ���� ����. -> PGT2ARR�� SpriteAnimation �뵵 / �׳� ^�������� ��Ͻ�, One-Two-Three �ø���.
			if (bTextureInputNames.find(".pgt2arr") != std::string::npos)
			{
				//Texture2DArray ���·� ���ƿ�.
				auto tTemp = _graphicsResourceManager->GetResourceByName(bTextureInputNames, Pg::Data::Enums::eAssetDefine::_TEXTURE2DARRAY);
				RenderTexture2DArray* tTexture2dDataArray = static_cast<RenderTexture2DArray*>(tTemp.get());
				bTexture2DVector = tTexture2dDataArray->GetSingleRenderTexture2DArray(); //PGT2ARR���� Texture2D ��������. 
				_IsPGT2ARR = true;
			}
			else
			{
				//HOTFIX. ���� Image Path Data�� �켱 '^'�� ��� �ִ������� Ȯ��.
				if (bTextureInputNames.find('^') == std::string::npos)
				{
					//ĳ�� �� ã�� - 1���� ����ִ� ��.
					//Image �����͸� �ޱ�.
					auto tTexture2dData = _graphicsResourceManager->GetResourceByName(bTextureInputNames, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
					bTexture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
				}
				else
				{
					std::string token;
					std::stringstream ss(bTextureInputNames);
					std::vector<std::string> outStringVector;
					//Ex. "^asd.png^ase.png^asf.png" �̷� ������ path�� �����ؾ� �Ѵ�.
					//���� �� ũ�Ⱑ ���ƾ� ����.
					//ĳ���� �������� �̹��� �޾Ƶ��̴� �� ������.
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
						//Image �����͸� �ޱ�.
						auto tTexture2dData = _graphicsResourceManager->GetResourceByName(it, Pg::Data::Enums::eAssetDefine::_TEXTURE2D);
						bTexture2DVector.push_back(static_cast<RenderTexture2D*>(tTexture2dData.get()));
					}
				}

				_IsPGT2ARR = false;
			}

			//������ ���.
			veSet->_veGraphicsSet->_textureSize = bTexture2DVector.size();
			assert(veSet->_veGraphicsSet->_textureSize != 0);
		}

		//�� �ΰ�, ���� Nullptr�� �⺻�� ����ϴ� ��.
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

		//Alpha Blend Ȱ�� <-> Alpha Clip Ȱ�� BlendState. 
		if (tEffectData._isAlphaBlended) { tVisualEffectGraphicsSet->_customBlendState = _commonStates->AlphaBlend(); }
		else { tVisualEffectGraphicsSet->_customBlendState = _commonStates->NonPremultiplied(); }

		//3D ���������� Plane.
		if (tEffectData._is3dSpace)
		{
			unsigned int tTextureSize = veSet->_veGraphicsSet->_textureSize;
			auto& bEffect = tVisualEffectGraphicsSet->_effect3D;
			auto& bStoreMatrixForm = tVisualEffectGraphicsSet->_effectStoreMatrixForm;

			//�ϳ��� Custom�� ������ / �� ����Ʈ����.
			if ((!tEffectData._isUseCustomVertexShader) && (!tEffectData._isUseCustomPixelShader))
			{
				//switch (tTextureSize)
				//{
				//	//Texture�� �ϳ��ۿ� ���ٸ�, Basic Effect�� �ֱ�. �׷���	
				//	case 1:
				//	{
				//		
				//	}
				//	break;
				//	default:
				//	{
				//		//Dual�� �ȵǰ� �س��Ҵ�.
				//		//������ Basic Effect ����Ī �����ϰ�.
				//		//assert(false && "2�� �̻��� DXTK ��ü ����Ʈ �ø���� �Ұ�.");
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
				//���߿� ����ٰ� Emission Default�� ������ ���� �ִ�.

				bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(tBasicEffect);

				//BasicEffect�̴�, �̸� ���.
				tVisualEffectGraphicsSet->_dxtkBasicEffect = tBasicEffect;

				//PGT2ARR���� ��� -> �����ϰ� Billboard Sprite Animation�� ����Ʈ�� ���̴�.
				tVisualEffectGraphicsSet->_isBillboardAnimation = _IsPGT2ARR;
			}
			else
			{
				auto& bBaseCustomEffect = tVisualEffectGraphicsSet->_baseCustomEffect;

				if (_IsPGT2ARR)
				{
					//�̰� Support �� �� ���̴�.
					assert(false);
				}
				else
				{
					switch (tTextureSize)
					{
						//Texture�� �ϳ��ۿ� ���ٸ�, Basic Effect�� �ֱ�. �׷��� 
						case 1:
						{
							bEffect = std::make_unique<OneTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<OneTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//������ ���.
							bBaseCustomEffect = bCustom;
						}
						break;
						case 2:
						{
							bEffect = std::make_unique<TwoTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<TwoTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//������ ���.
							bBaseCustomEffect = bCustom;
						}
						break;
						case 3:
						{
							bEffect = std::make_unique<ThreeTextureEffect3D>(_DXStorage->_device, veSet);
							auto bCustom = static_cast<ThreeTextureEffect3D*>(bEffect.get());
							bStoreMatrixForm = static_cast<DirectX::IEffectMatrices*>(bCustom);

							//������ ���.
							bBaseCustomEffect = bCustom;
						}
						break;
						default:
						{
							assert(false && "4�� �̻��� �Ұ�.");
						}
						break;
					}
				}
			}

			//InputLayout�� �°� ����� ���� - Custom�� ����ص� PosNormalTex�� �����ؾ� �Ѵ�.
			//�ϴ�, �� ����.
			void const* shaderByteCode;
			size_t byteCodeLength;
			bEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength); // ��� Effect �� �ڵ� ��ȭ ����.

			HR(_DXStorage->_device->CreateInputLayout(DirectX::VertexPositionNormalTexture::InputElements,
				DirectX::VertexPositionNormalTexture::InputElementCount,
				shaderByteCode, byteCodeLength,
				&(tVisualEffectGraphicsSet->_inputLayout)));

			//����, �� ���ϱ�.
			//Alpha Blend != Alpha Clip.
			//���߿� Apply ���� ��, ���� �ۿ��� �Ǵ� ���� �ƴ϶�, Effect �ȿ��� �Ǿ�� �Ѵ�. (VEGraphicSet ������)
			// ����, CustomVertexShader / CustomPixelShader�� �� �� �Ѳ����� ���ų� / ���ų��ؾ�!

		}
		else //2D �������� Plane.
		{
			//ScreenSpace���� ����Ǵ� ����.
			//������ BlendState ����.	
			tVisualEffectGraphicsSet->_spriteBatch = std::make_unique<DirectX::SpriteBatch>(_DXStorage->_deviceContext);
			tVisualEffectGraphicsSet->_spriteEffect2D = std::make_unique<SpriteEffect2D>(veSet);
			//�������� ������ �� �����ؾ� �� �� ����.
			tVisualEffectGraphicsSet->_inputLayout = LayoutDefine::GetSpriteCustomLayout();

			//Custom 2D Shader ��� :
			// https://raw.githubusercontent.com/wiki/Microsoft/DirectXTK/shaders/BloomExtract.hlsl
			// https://github.com/microsoft/DirectXTK/wiki/Writing-custom-shaders
			//Texture2D<float4> Texture : register(t0);
			//sampler TextureSampler : register(s0);
			//float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0

			//���� SpriteSheet�̶��, �ٸ� Ŭ������ ���.
			//�ٸ� ���̴��� �־��ִ� ������ �ذ��� �� ���� ��.

			auto& bSpriteEffect2D = tVisualEffectGraphicsSet->_spriteEffect2D;
			if (tEffectData._spriteMode == Pg::Data::_DEFAULT)
			{
				//����Ʈ ���� ���.
				bSpriteEffect2D->_isDefaultRenderMode = true;
			}
			else if (tEffectData._spriteMode == Pg::Data::_SPRITE_SHEET)
			{
				//SpriteSheet ���.
				bSpriteEffect2D->_animatedTexture = std::make_unique<AnimatedTexture>();
				//�ϴ� Default Frames Per Second = 2.
				bSpriteEffect2D->_animatedTexture->Load(tVisualEffectGraphicsSet->_renderTextureVec.at(0)->GetSRV(), tEffectData._frameCount, 2);
				//�̸� �ε�.

			}
			else if (tEffectData._spriteMode == Pg::Data::_SCROLLING_BG)
			{
				//Scrolling Background ���.
				bSpriteEffect2D->_scrollingBackground = std::make_unique<ScrollingBackground>();
				bSpriteEffect2D->_scrollingBackground->Load(tVisualEffectGraphicsSet->_renderTextureVec.at(0)->GetSRV());
				bSpriteEffect2D->_scrollingBackground->SetWindow(Pg::Data::GameConstantData::WIDTH, Pg::Data::GameConstantData::HEIGHT);
			}
			else
			{
				assert(false && "������");
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
