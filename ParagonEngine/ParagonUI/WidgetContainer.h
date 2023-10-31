#pragma once
#include "IWidget.h"
#include "InputText.h"

#include <vector>

namespace Pg::UI
{
	class WidgetContainer
	{
	public:
		void Update();
		std::vector<Pg::UI::IWidget*> GetColumnWidgets();

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

	private:
		std::vector<Pg::UI::IWidget*> _widgets;
		std::vector<Pg::UI::IWidget*> _columnWidgets;
	};

}

