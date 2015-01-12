#ifndef _MENU_MAIN_H_
#define _MENU_MAIN_H_

#include <string>
#include <SDL/SDL.h>
#include "MenuScreen.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // QuitConfirmMenu
    //////////////////////////////////////////////////////////////////////////

    class QuitConfirmMenu : public MenuScreen
    {
    public:
        // Overrides
        void                Init();
        void                Draw();
        NextAction          OnMenuItemChoose(unsigned int id);
        const std::string   NextMenu() const;

        // Constants
        static const std::string MENU_NAME;

    private:
        // Constants
        static const std::string FONT;
        static const std::string BACKGROUND;
        static const float       ITEM_HEIGHT;
        static const float       ITEM_SPACING;
        static const float       TITLE_HEIGHT;
        static const float       TITLE_STARTY;
        enum PauseMenuItems { MENUITEM_YES, MENUITEM_NO, MENUITEM_COUNT };

        // Members
        juzutil::Vector2 m_titleOrigin;
    };

    //////////////////////////////////////////////////////////////////////////
    // MainMenu
    //////////////////////////////////////////////////////////////////////////

    class MainMenu : public MenuScreen
    {
    public:
        // Overrides
        void                Init();
        void                Draw();
        NextAction          OnMenuItemChoose(unsigned int id);
        const std::string   NextMenu() const;

        // Constants
        static const std::string MENU_NAME;

    private:
        // Constants
        static const std::string VERSION_FONT;
        static const std::string BACKGROUND;
        static const float       ITEM_HEIGHT;
        static const float       ITEM_SPACING;
        static const float       ITEM_STARTY;
        enum MainMenuItems { MENUITEM_START, MENUITEM_SCORES, MENUITEM_QUIT, MENUITEM_COUNT };

        // Members
        std::string m_nextMenu;
    };
}

#endif // _MENU_MAIN_H_
