#pragma once
#include <vector>

/// <summary>
/// Component System & Event System�� ������ ������, Data�� Engine/Graphics�� ������ �� �ִ� ������ �ʿ���.
/// Ư��, Graphics���� 1��1 �����ϴ� ������ ������Ʈ�� ���� ��, �Ź� ��ȭ�� �����ϴ� ���� �ɰ��� ���Ϸ� �̾���.
/// 
/// �ϴ���, ����/������ Component���� ����� �����ϱ� ���ؼ� �������.
/// Process�� ���ؼ� ���� ����� ��, �Ͻ��� �����̳��� ���ҿ��� ���ľ� ��.
/// �ϳ��� �ν��Ͻ����� singleton-cpp�� ���� �����Ǿ�� ��.
/// </summary>

namespace Pg::Data
{
	class Component;
	class BaseRenderer;
}
namespace Pg::Data
{
	class RendererChangeList
	{
	public:
		RendererChangeList();

		// [Usage: ParagonData] AddComponent ���ο��� Renderer �迭�� ȣ��Ǿ�� �Ѵ�!
		void InsertOnAddedRendererList(BaseRenderer* renderer);
		void InsertOnDeletedRendererList(BaseRenderer* renderer);
	
		// [Usage: ParagonGraphics] Data�� ����� ����Ʈ�� ��ȯ���� �� �ִ�.
		std::vector<BaseRenderer*>& GetAddedRendererList();
		std::vector<BaseRenderer*>& GetDeletedRendererList();

		//[Usage: ParagonProcess] ������Ʈ�� �ʿ䰡 �ִ��� ����.
		bool IfShouldUpdate();

		//[Usage: ParagonProcess] ���� Iteration�� ���� Clear.
		void ClearRendererChangeLists();

	private:
		std::vector<BaseRenderer*> _onAddedRendererList;
		std::vector<BaseRenderer*> _onDeletedRendererList;
	};
}


