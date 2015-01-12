#include "Menu.h"
#include "MenuMain.h"
#include "MenuPause.h"
#include "MenuHighScores.h"
#include "MenuNewHighScore.h"

namespace typing
{
    // Singleton implementation
    std::auto_ptr<Menu> Menu::m_singleton(new Menu);
    Menu& Menu::GetMenu ()
    {
        return *(m_singleton.get());
    }

    // Main menu stuff
    bool Menu::Init ()
    {
        // Register menus with the menu manager
        RegisterMenu<MainMenu>(MainMenu::MENU_NAME);
        RegisterMenu<PauseMenu>(PauseMenu::MENU_NAME);
        RegisterMenu<EndGameConfirmMenu>(EndGameConfirmMenu::MENU_NAME);
        RegisterMenu<QuitConfirmMenu>(QuitConfirmMenu::MENU_NAME);
        RegisterMenu<HighScoresMenu>(HighScoresMenu::MENU_NAME);
        RegisterMenu<NewHighScoreMenu>(NewHighScoreMenu::MENU_NAME);

        // Push the first menu onto the stack
        NextMenu(MainMenu::MENU_NAME);

        m_active = true;

        return true;
    }

    void Menu::ClearMenuStack()
    {
        while (!m_menuStack.empty())
        {
            m_menuStack.pop();
        }
    }

    void Menu::Update ()
    {
        if (IsActive() && !m_menuStack.empty())
        {
            MenuScreenPtr&          menu = m_menuStack.top();
            MenuScreen::NextAction  act  = menu->Update();

            HandleAction(act);
        }
    }

    void Menu::Draw ()
    {
        if (IsActive() && !m_menuStack.empty())
        {
            MenuScreenPtr& menu = m_menuStack.top();
            if (menu)
            {
                menu->Draw();
            }
        }

    }

    template<typename menuType> void Menu::RegisterMenu (const std::string& menuName)
    {
        if (m_menuFactory.find(menuName) == m_menuFactory.end())
        {
            m_menuFactory[menuName] = CreateMenu<menuType>;
        }
    }

    void Menu::OnType(const SDL_keysym sym)
    {
        if (IsActive())
        {
            MenuScreenPtr&          menu = m_menuStack.top();
            MenuScreen::NextAction  act  = menu->OnType(sym);

            HandleAction(act);
        }
    }


    void Menu::HandleAction(MenuScreen::NextAction act)
    {
        switch (act)
        {
        case MenuScreen::ACTION_PREV:
            if (m_menuStack.size() > 1)
            {
                m_menuStack.pop();
            }
            break;

        case MenuScreen::ACTION_NEXT:
            if (!m_menuStack.empty())
            {
                NextMenu(m_menuStack.top()->NextMenu());
            }
            break;

        case MenuScreen::ACTION_NEXT_AND_POP:
            if (!m_menuStack.empty())
            {
                // Move to the next menu, but don't keep the current menu on the stack.
                const std::string nextMenu = m_menuStack.top()->NextMenu();
                m_menuStack.pop();
                NextMenu(nextMenu);
            }
            break;

        case MenuScreen::ACTION_GAME:
            // We've gone to the game now, so make the menu inactive
            ClearMenuStack();
            m_active = false;
            break;

        case MenuScreen::ACTION_NONE:
        default:
            break;
        }
    }

    void Menu::NextMenu (const std::string& menuName)
    {
        MenuFactoryIter iterMenu = m_menuFactory.find(menuName);

        if (iterMenu != m_menuFactory.end())
        {
            MenuCreator creator = iterMenu->second;
            MenuScreenPtr menu = creator();

            menu->Init();
            m_menuStack.push(menu);
        }
    }

    void Menu::Activate(const std::string& menuName)
    {
        ClearMenuStack();
        NextMenu(menuName);
        m_active = true;
    }
}
