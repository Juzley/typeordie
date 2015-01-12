#ifndef _MENU_PAUSE_H_
#define _MENU_PAUSE_H_

#include <string>
#include <SDL/SDL.h>
#include "MenuScreen.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // EndGameConfirmMenu
    //////////////////////////////////////////////////////////////////////////

    class EndGameConfirmMenu : public MenuScreen
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
        static const float       ITEM_HEIGHT;
        static const float       ITEM_SPACING;
        static const float       TITLE_HEIGHT;
        static const float       TITLE_OFFSET;
        enum PauseMenuItems { MENUITEM_YES, MENUITEM_NO, MENUITEM_COUNT };

        // Members
        juzutil::Vector2 m_titleOrigin;
    };


    //////////////////////////////////////////////////////////////////////////
    // PauseMenu
    //////////////////////////////////////////////////////////////////////////

    class PauseMenu : public MenuScreen
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
        static const float       ITEM_HEIGHT;
        static const float       ITEM_SPACING;
        static const float       TITLE_HEIGHT;
        static const float       TITLE_OFFSET;
        enum PauseMenuItems { MENUITEM_RESUME, MENUITEM_QUIT, MENUITEM_COUNT };

        // Members
        juzutil::Vector2 m_titleOrigin;
    };
}

#endif // _MENU_PAUSE_H_
