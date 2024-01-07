#pragma once
#include "RenderObject3DList.h"
#include "../ParagonData/CameraData.h"

#include <vector>
/// <summary>
/// 모든 Render Pass들의 부모 클래스이다.
/// 이를 상속받아서 세부적인 패스들은 활용된다.
/// 개별적인 RenderPass당 세부 사항이 다르므로, 인터페이스 형태로 존재.
/// </summary>

namespace Pg::Graphics
{
	class IRenderPass
	{
	public:
		virtual void Initialize() abstract;

		//미리 실행할 수 있는 함수들은 실행하고, 넘겨야 하는 자료들은 넘기기.
		
		//전 패스에서 필요한 요소들을 패스 나름대로 받는 코드이다. (원본 변형 불가)
		virtual void ReceiveRequiredElements(const std::vector<ID3D11RenderTargetView*>* rtvArray, unsigned int rtvCount, 
			const std::vector<ID3D11ShaderResourceView*>* srvArray, unsigned int srvCount, ID3D11DepthStencilView* dsv) abstract;
		virtual void BindPass() abstract;
		virtual void RenderPass(RenderObject3DList* renderObjectList, Pg::Data::CameraData* camData) abstract;
		virtual void UnbindPass() abstract;
		//다음 패스에 필요할 렌더 관련 함수들을 실행한다.
		virtual void ExecuteNextRenderRequirements() abstract;
		//다음 패스에 넘겨줄 리스트를 (만약 있을 경우) 넘겨준다.		
		virtual void PassNextRequirements(std::vector<ID3D11RenderTargetView*>*& rtvArray, unsigned int& rtvCount, 
			std::vector<ID3D11ShaderResourceView*>*& srvArray, unsigned int& srvCount, ID3D11DepthStencilView*& dsv) abstract;
	};
}

