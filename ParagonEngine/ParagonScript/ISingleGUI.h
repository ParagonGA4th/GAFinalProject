#pragma once

///��� �������� GUI ��Ұ� ��ӹ��� �������̽�.

namespace Pg::DataScript
{
	class ISingleGUI
	{
	public:
		//�ʿ��� ��ŭ ���� �޾Ƽ� ���� ����̴�.
		//Ex. Player�� Health�����, �ʿ��� ��Ҹ� Pointer�� �޾Ƽ� �� �� �ְ� �Ѵ�.
		//�̴� Handler���� �ϰ������� Setup �������� �����ϰ� �� ���̴�.
		//�ش� ������ �ؼ��ϴ� ����� ���� Ŭ�������� �޶����� �� ���̴�.
		//�ش� Script�� Renderer2D�� �����ؼ�, FillRatio, Index ����� �ٲٰ� �ȴ�. 
		virtual void ReceiveDependentPointers(void* p1, void* p2, void* p3, double v1, double v2, double v3) abstract;
	};
}
