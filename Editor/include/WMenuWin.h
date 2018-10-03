#ifndef _WMENU_WINDOW_H_
#define _WMENU_WINDOW_H_

namespace maj::editor
{
	class WMainWindow;

	class WMenuWin
	{
	public:
		WMenuWin(WMainWindow* parent = nullptr);
		~WMenuWin();

	private:
		void createMenus();
		void createFilesMenu();
		void createAssetMenu();
		void createGameObjectMenu();
		void createWindowMenu();
		void createHelpMenu();
		void createAboutDialog();
		
		WMainWindow* mParent;
	};
}
#endif