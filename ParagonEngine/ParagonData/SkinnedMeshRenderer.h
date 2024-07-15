#pragma once

#include "RendererBase3D.h"
#include <string>
#include <functional>
#include <dxtk/SimpleMath.h>
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// 앞으로 3D Skinned Mesh 출력을 담당할 렌더러.
/// </summary>

namespace Pg::Graphics
{
	class RenderObjectSkinnedMesh3D;
	class GraphicsSceneParser;
}

namespace Pg::Data
{
	class GameObject;
	class AnimTransform;
}

namespace Pg::Data
{
	class SkinnedMeshRenderer : public RendererBase3D
	{
		friend class Pg::Graphics::RenderObjectSkinnedMesh3D;
		friend class Pg::Graphics::GraphicsSceneParser;
	public:
		SkinnedMeshRenderer(GameObject* obj);

		virtual void ConvertPotentialUnrealValues() override;
		//혹시 Path만 있고 Name은 없는 상황을 막기 위해.
		virtual void CheckForPathNameErrors() override;

		//Animation Path - SetAnimation()
		std::string GetAnimation();
		void SetAnimation(const std::string& animName, bool isLoop);

		void SetAlphaPercentage(float per);
		float GetAlphaPercentage();

		void SetanimBlendFactor(float per);
		float GetanimBlendFactor();

		//본 이름을 기반으로 Transform을 찾을 수 있다. (단, 당연히 RootNode에 상대적일 것, World 관련 행렬 따로 곱해야.)
		Pg::Data::AnimTransform* FindAnimTransform(const std::string& animNodeName);

		void PauseAnim();
		void PlayAnim();

	public:
		//Scene이 시작할 때 일괄적으로 호출하기 위해서.
		//Internal Logic으로 연관.
		virtual void Internal_EngineAwake() override;
		
	public:
		//Serializers.
		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;

	public:
		BEGIN_VISITABLES(SkinnedMeshRenderer);
		VISITABLE(std::string, _meshName);
		VISITABLE(std::string, _materialName);
		VISITABLE(std::string, _initAnimName);
		VISITABLE(float, _animBlendFactor);
		VISITABLE(float, _alphaPercentage);
		END_VISITABLES;

		//이것도 이제 VISITABLE에 추가되어야! 기본적으로 10 정도가 적당하나, 보스 등 일부 기준으로 더 느려야.
		//float _animBlendFactor{ 10.f }; 
		//float _alphaPercentage{ 50.f }; //0 - 100 범위. Alpha Blending을 사용할 때만 쓰인다.
	private:
		//RenderObjectSkinnedMesh가 등록한다.
		//return bool == 해당 함수가 성공했는지. (GraphicsSceneParser가 별도로 연결해준다)
		//일단 직접 호출 금지.
		std::function<void(const std::string&, bool)> _setAnimationFunction;
		std::function<Pg::Data::AnimTransform*(const std::string&)> _findAnimTransformFunction;

		std::string _currentAnim;

		//이는 Pause / Play를 위해서.
		int _playVariable{ 1 };
	};
}

