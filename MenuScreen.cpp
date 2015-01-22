#include <algorithm>
#include <functional>
#include "MenuScreen.h"

namespace typing
{
    void MenuScreen::Draw()
    {
        for_each(m_menuItems.begin(),
                 m_menuItems.end(),
                 std::mem_fn(&MenuItem::Draw));
    }

    MenuScreen::NextAction MenuScreen::OnKeyDown(SDL_Keycode keycode)
    {
        if (keycode == SDLK_RETURN)
        {
            MenuItems::iterator iter = find_if(
                                        m_menuItems.begin(),
                                        m_menuItems.end(),
                                        std::mem_fn(&MenuItem::IsSelected));

            if (iter != m_menuItems.end())
            {
                return OnMenuItemChoose((*iter)->GetID());
            }
        }
        else if (keycode == SDLK_DOWN || keycode == SDLK_RIGHT)
        {
            MenuItems::iterator iter = find_if(
                                        m_menuItems.begin(),
                                        m_menuItems.end(),
                                        std::mem_fn(&MenuItem::IsSelected));

            if (iter != m_menuItems.end())
            {
                (*iter)->Select(false);

                if (++iter == m_menuItems.end())
                {
                    iter = m_menuItems.begin();
                }

                (*iter)->Select(true);
            }
        }
        else if (keycode == SDLK_UP || keycode == SDLK_LEFT)
        {
            MenuItems::iterator iter = find_if(
                                        m_menuItems.begin(),
                                        m_menuItems.end(),
                                        std::mem_fn(&MenuItem::IsSelected));

            if (iter != m_menuItems.end())
            {
                (*iter)->Select(false);

                if (iter == m_menuItems.begin())
                {
                    iter = m_menuItems.end();
                }

                --iter;

                (*iter)->Select(true);
            }
        }

        return ACTION_NONE;
    }

}
