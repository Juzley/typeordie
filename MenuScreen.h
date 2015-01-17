#ifndef _MENU_SCREEN_H_
#define _MENU_SCREEN_H_

#include <string>
#include <vector>
#include <memory>
#include <SDL/SDL.h>
#include "MenuItem.h"

namespace typing
{
    class MenuScreen
    {
    public:
        // Ctors/Dtors
        virtual ~MenuScreen()
        {
        }

        // Constants
        // NextAction enum contains values indicating what to do after this menu has updated,
        // returned by MenuScreen::Update
        // - ACTION_NONE        : No action, update this menu again
        // - ACTION_NEXT        : Switch to the menu identified by this MenuScreen's NextMenu
        // - ACTION_NEXT_AND_POP: Switch to the menu identified by this Menuscreen's NextMenu and pop this menu off the stack.
        // - ACTION_PREV        : Switch to the menu displayed before this one
        // - ACTION_GAME        : (Re)start the game
        enum NextAction { ACTION_NONE, ACTION_NEXT, ACTION_NEXT_AND_POP, ACTION_PREV, ACTION_GAME };

        // Methods
        virtual        void                Init()               = 0;
        virtual        const std::string   NextMenu() const     = 0;

        virtual        NextAction          OnType(const SDL_keysym sym);
        virtual        void                Draw();

        virtual NextAction Update()
        {
            return ACTION_NONE;
        }

        virtual NextAction OnMenuItemChoose(unsigned int)
        {
            return ACTION_NONE;
        }

        void AddMenuItem(const MenuItemPtr& item)
        {
            m_menuItems.push_back(item);
        }

    private:
        // Typedefs
        typedef std::vector<MenuItemPtr> MenuItems;

        // Members
        MenuItems m_menuItems;
    };
    typedef std::shared_ptr<MenuScreen> MenuScreenPtr;
}

#endif // _MENU_SCREEN_H_
