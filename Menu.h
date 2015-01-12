#ifndef _MENU_H_
#define _MENU_H_

#include <stack>
#include <map>
#include <string>
#include <SDL/SDL.h>
#include "MenuScreen.h"

namespace typing
{
    typedef MenuScreenPtr (*MenuCreator)();

    template<typename menuType>
    MenuScreenPtr CreateMenu()
    {
        return MenuScreenPtr(new menuType);
    }

    class Menu
    {
    public:
        // Singleton implementation
        static Menu& GetMenu();

        // Methods
        bool Init();
        void Update();
        void Draw();
        void OnType(const SDL_keysym sym);
        void Activate(const std::string& menuName);

        bool IsActive()
        {
            return m_active;
        }

    private:
        // Typedefs
        typedef std::stack<MenuScreenPtr>                    MenuStack;
        typedef std::map<std::string, MenuCreator>           MenuFactory;
        typedef std::map<std::string, MenuCreator>::iterator MenuFactoryIter;

        // Ctors/Dtors
        Menu()
        {
        }

        // Methods
        template<typename menuType> void RegisterMenu(const std::string& menuName);
                                    void NextMenu(const std::string& menuName);
                                    void ClearMenuStack();
                                    void HandleAction(MenuScreen::NextAction act);


        // Singleton implementation
        static std::auto_ptr<Menu> m_singleton;

        // Members
        MenuStack   m_menuStack;
        MenuFactory m_menuFactory;
        bool        m_active;
    };

    #define MENU Menu::GetMenu()
}

#endif // _MENU_H_
