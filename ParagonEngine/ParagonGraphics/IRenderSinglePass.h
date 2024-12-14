#pragma once
#include "RenderObject3DList.h"
#include "../ParagonData/CameraData.h"
#include "../ParagonData/ParagonDefines.h"
#include "../ParagonHelper/ResourceHelper.h"
#include "D3DCarrier.h"
#include <vector>
/// <summary>
/// ��� Single Render Pass���� �θ� Ŭ�����̴�.
/// �̸� ��ӹ޾Ƽ� �������� �н����� Ȱ��ȴ�.
/// �������� RenderPass�� ���� ������ �ٸ��Ƿ�, �������̽� ���·� ����.
/// </summary>

namespace Pg::Graphics
{
	class IRenderSinglePass
	{
	public:
		virtual void Initialize() abstract;

		//�̸� ������ �� �ִ� �Լ����� �����ϰ�, �Ѱܾ� �ϴ� �ڷ���� �ѱ��.
		
		//�� �н����� �ʿ��� ��ҵ��� �н� ������� �޴� �ڵ��̴�. (���� ���� �Ұ�)
		virtual void ReceiveRequiredElements(const D3DCarrier& gCarrier) abstract;
		virtual void BindPass() abstract;
		virtual void RenderPass(void* renderObjectList, Pg::Data::CameraData* camData) abstract;
		virtual void UnbindPass() abstract;
		//���� �н��� �ʿ��� ���� ���� �Լ����� �����Ѵ�.
		virtual void ExecuteNextRenderRequirements() abstract;
		//���� �н��� �Ѱ��� ����Ʈ�� (���� ���� ���) �Ѱ��ش�.		
		virtual void PassNextRequirements(D3DCarrier& gCarrier) abstract;
	
		//�̰� ���������� �ƴ�. 
		//�ʿ�� ȣ���.
		virtual void ConnectDefaultResources() {}
	
	};
}

