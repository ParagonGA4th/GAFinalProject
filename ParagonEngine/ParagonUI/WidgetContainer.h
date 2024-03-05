#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI
{
	class WidgetContainer
	{
	public:
		void Update();

		std::vector<Pg::UI::IWidget*> GetColumnWidgets();
		std::vector<Pg::UI::IWidget*> GetCollapsWidgets();
		std::vector<Pg::UI::IWidget*> GetTreeNodeWidgets();
		std::vector<Pg::UI::IWidget*> GetChildWindowWidgets();

		void SetTempWidgets(std::vector<Pg::UI::IWidget*> tempWidget);
		std::vector<Pg::UI::IWidget*> GetTempWidgets();

		void SameLine(bool isSameLine);

		void ClearWidget(int index = 0);
		void ClearColumnWidget();
		void ClearCollapsWidget();
		void ClearTreeNodeWidget(int index = -1);
		void ClearTempWidgets();

		template<typename T, typename ... Args>
		T& CreateWidget(Args&&... args)
		{
			_widgets.emplace_back(new T(args...));
			T& ins = *reinterpret_cast<T*>(_widgets.back());
			return ins;
		}

		template<typename T, typename ... Args>
		T& CreateColumnsWidget(Args&&... args)
		{
			_columnWidgets.emplace_back(new T(args...));
			T& ins = *reinterpret_cast<T*>(_columnWidgets.back());
			return ins;
		}

		template<typename T, typename ... Args>
		T& CreateCollapsWidget(Args&&... args)
		{
			_collapsWidgets.emplace_back(new T(args...));
			T& ins = *reinterpret_cast<T*>(_collapsWidgets.back());
			return ins;
		}

		template<typename T, typename ... Args>
		T& CreateTreeNodeWidget(Args&&... args)
		{
			_treeNodeWidgets.emplace_back(new T(args...));
			T& ins = *reinterpret_cast<T*>(_treeNodeWidgets.back());
			return ins;
		}	
		
		template<typename T, typename ... Args>
			T& CreateChildWindowWidget(Args&&... args)
		{
				_childWindowWidgets.emplace_back(new T(args...));
			T& ins = *reinterpret_cast<T*>(_childWindowWidgets.back());
			return ins;
		}

	private:
		std::vector<Pg::UI::IWidget*> _widgets;
		std::vector<Pg::UI::IWidget*> _columnWidgets;
		std::vector<Pg::UI::IWidget*> _collapsWidgets;
		std::vector<Pg::UI::IWidget*> _treeNodeWidgets;
		std::vector<Pg::UI::IWidget*> _childWindowWidgets;
		std::vector<Pg::UI::IWidget*> _tempWidgets;

		bool _isSameLine;
	};
}

