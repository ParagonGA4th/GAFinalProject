#pragma once

#include "RendererBase3D.h"
#include <string>
#include <functional>
#include <dxtk/SimpleMath.h>

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
		void SetAnimation(const std::string& animName);


	//private:
		//RenderObjectSkinnedMesh가 등록한다.
		//return bool == 해당 함수가 성공했는지. (GraphicsSceneParser가 별도로 연결해준다)
		//일단 직접 호출 금지.
		std::function<void(const std::string&)> _setAnimationFunction;


	private:
		//각각의 Node와 대응, 자신의 부모 노드의 인덱스가 어디인지를 알고 (first)
		//second는 이미 계산이 된 행렬이 기록된다.
		//매 프레임, 전체는 Matrix::Identity();로 채워진다.
		std::vector<std::pair<int, DirectX::SimpleMath::Matrix>> _linearizedFinalTransformList;

	};
}

