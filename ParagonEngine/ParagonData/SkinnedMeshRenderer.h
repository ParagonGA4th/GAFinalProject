#pragma once

#include "RendererBase3D.h"
#include <string>
#include <functional>
#include <dxtk/SimpleMath.h>
#include "data_factory.h" // Auto Register를 위한 필수요건.

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

	class SkinnedMeshRenderer : public RendererBase3D, Pg::Factory::Data::RegisteredInFactory<RendererBase3D, SkinnedMeshRenderer, GameObject*>
	{
		friend class Pg::Graphics::RenderObjectSkinnedMesh3D;
		friend class Pg::Graphics::GraphicsSceneParser;
	public:
		SkinnedMeshRenderer(GameObject* obj);

		virtual void ConvertPotentialUnrealValues() override;
		//혹시 Path만 있고 Name은 없는 상황을 막기 위해.
		virtual void CheckForPathNameErrors() override;

		//Animation Path - SetAnimation()
		void SetAnimation(const std::string& animName, bool isLoop);

	public:
		//Scene이 시작할 때 일괄적으로 호출하기 위해서.
		virtual void Start() override;

	public:
		//Serializers.
		virtual void OnSerialize(SerializeVector& sv) override;
		virtual void OnDeserialize(SerializeVector& sv) override;

	public:
		BEGIN_VISITABLES(SkinnedMeshRenderer);
		VISITABLE(bool, _isInstanced);
		VISITABLE(std::string, _meshName);
		VISITABLE(std::string, _materialName);
		VISITABLE(std::string, _initAnimName);
		END_VISITABLES;

	private:
		//RenderObjectSkinnedMesh가 등록한다.
		//return bool == 해당 함수가 성공했는지. (GraphicsSceneParser가 별도로 연결해준다)
		//일단 직접 호출 금지.
		std::function<void(const std::string&, bool)> _setAnimationFunction;


	

	};
}

