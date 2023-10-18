#pragma once

namespace Pg::UI
{
	class Panel
	{
	public:
		Panel();
		~Panel();

		void SetPanelShow(bool show);
		bool GetPanelShow();

	private:
		bool _isShow;
	};
}


