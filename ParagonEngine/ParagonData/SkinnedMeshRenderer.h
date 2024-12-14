#pragma once

#include "RendererBase3D.h"
#include <string>
#include <functional>
#include <dxtk/SimpleMath.h>
#include <visit_struct/visit_struct.hpp>

/// <summary>
/// ������ 3D Skinned Mesh ����� ����� ������.
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
		//Ȥ�� Path�� �ְ� Name�� ���� ��Ȳ�� ���� ����.
		virtual void CheckForPathNameErrors() override;

		//Animation Path - SetAnimation()
		std::string GetAnimation();
		void SetAnimation(const std::string& animName, bool isLoop);

		void SetAlphaPercentage(float per);
		float GetAlphaPercentage();

		void SetanimBlendFactor(float per);
		float GetanimBlendFactor();

		//�� �̸��� ������� Transform�� ã�� �� �ִ�. (��, �翬�� RootNode�� ������� ��, World ���� ��� ���� ���ؾ�.)
		Pg::Data::AnimTransform* FindAnimTransform(const std::string& animNodeName);

		void PauseAnim();
		void PlayAnim();

	public:
		//Scene�� ������ �� �ϰ������� ȣ���ϱ� ���ؼ�.
		//Internal Logic���� ����.
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

		//�̰͵� ���� VISITABLE�� �߰��Ǿ��! �⺻������ 10 ������ �����ϳ�, ���� �� �Ϻ� �������� �� ������.
		//float _animBlendFactor{ 10.f }; 
		//float _alphaPercentage{ 50.f }; //0 - 100 ����. Alpha Blending�� ����� ���� ���δ�.
	private:
		//RenderObjectSkinnedMesh�� ����Ѵ�.
		//return bool == �ش� �Լ��� �����ߴ���. (GraphicsSceneParser�� ������ �������ش�)
		//�ϴ� ���� ȣ�� ����.
		std::function<void(const std::string&, bool)> _setAnimationFunction;
		std::function<Pg::Data::AnimTransform*(const std::string&)> _findAnimTransformFunction;

		std::string _currentAnim;

		//�̴� Pause / Play�� ���ؼ�.
		int _playVariable{ 1 };
	};
}

