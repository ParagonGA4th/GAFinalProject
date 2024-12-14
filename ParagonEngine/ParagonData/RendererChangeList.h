#pragma once
#include <vector>

/// <summary>
/// Component System & Event System이 나오기 전까지, Data와 Engine/Graphics랑 연동할 수 있는 수단이 필요함.
/// 특히, Graphics에서 1대1 대응하는 렌더될 오브젝트를 만들 때, 매번 변화를 감지하는 것은 심각한 부하로 이어짐.
/// 
/// 일단은, 생성/삭제된 Component들의 목록을 관리하기 위해서 만들어짐.
/// Process에 의해서 로직 제어될 것, 일시적 컨테이너의 역할에서 그쳐야 함.
/// 하나의 인스턴스만이 singleton-cpp에 의해 유지되어야 함.
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

		// [Usage: ParagonData] AddComponent 내부에서 Renderer 계열시 호출되어야 한다!
		void InsertOnAddedRendererList(BaseRenderer* renderer);
		void InsertOnDeletedRendererList(BaseRenderer* renderer);
	
		// [Usage: ParagonGraphics] Data의 변경된 리스트를 반환받을 수 있다.
		std::vector<BaseRenderer*>& GetAddedRendererList();
		std::vector<BaseRenderer*>& GetDeletedRendererList();

		//[Usage: ParagonProcess] 업데이트될 필요가 있는지 리턴.
		bool IfShouldUpdate();

		//[Usage: ParagonProcess] 다음 Iteration을 위해 Clear.
		void ClearRendererChangeLists();

	private:
		std::vector<BaseRenderer*> _onAddedRendererList;
		std::vector<BaseRenderer*> _onDeletedRendererList;
	};
}


