#include <boost/format.hpp>
#include "App.h"
#include "MenuMain.h"
#include "MenuHighScores.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "Game.h"
#include "Utils.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // QuitGameConfirmMenu
    //////////////////////////////////////////////////////////////////////////

    const std::string QuitConfirmMenu::MENU_NAME("QuitConfirmMenu");
    const std::string QuitConfirmMenu::FONT("fonts/menufont.fnt");
    const std::string QuitConfirmMenu::BACKGROUND("textures/menu/background.tga");
    const float       QuitConfirmMenu::ITEM_HEIGHT  = 48.0f;
    const float       QuitConfirmMenu::ITEM_SPACING = 2.0f;
    const float       QuitConfirmMenu::TITLE_HEIGHT = 48.0f;

    void QuitConfirmMenu::Init()
    {
        const float TITLE_STARTY =
            APP.GetScreenHeight() - 2.0f * TITLE_HEIGHT - 7.0f * ITEM_SPACING;

        TEXTURES.Add(BACKGROUND);
        FONTS.Add(FONT);
        MenuItem::Init();

        // Add menu items
        // Hack. Magic numbers at the moment to get the menu items spaced correctly.
        m_titleOrigin.Set(APP.GetScreenWidth() / 2.0f, TITLE_STARTY);

        float y = TITLE_STARTY + ITEM_SPACING + TITLE_HEIGHT;
        float x = static_cast<float>(APP.GetScreenWidth()) / 2.0f - 50.0f;
        AddMenuItem(MenuItemPtr(
            new MenuItem(juzutil::Vector2(x, y), "Yes", 
                         MENUITEM_YES, ITEM_HEIGHT, true)));
        x += 100.0f;

        AddMenuItem(MenuItemPtr(
            new MenuItem(juzutil::Vector2(x, y), "No",
                         MENUITEM_NO, ITEM_HEIGHT, false)));
    }

    void QuitConfirmMenu::Draw()
    {
        const float TITLE_STARTY =
            APP.GetScreenHeight() - 2.0f * TITLE_HEIGHT - 7.0f * ITEM_SPACING;

        DrawTexturedRect(BACKGROUND, 0.0f, 0.0f, APP.GetScreenWidth(), APP.GetScreenHeight());

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, TITLE_STARTY - ITEM_SPACING,
            APP.GetScreenWidth(), APP.GetScreenHeight() - TITLE_STARTY + ITEM_SPACING);

        FONTS.Print(FONT, m_titleOrigin.GetX(), m_titleOrigin.GetY(), TITLE_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER, "RSI Already?");

        // Draw the menu items
        MenuScreen::Draw();
    }

    MenuScreen::NextAction QuitConfirmMenu::OnMenuItemChoose(unsigned int id)
    {
        switch(id)
        {
        case MENUITEM_YES:
            APP.Quit();
            return ACTION_NONE;

        case MENUITEM_NO:
            return ACTION_PREV;

        default:
            return ACTION_NONE;
        }
    }

    const std::string QuitConfirmMenu::NextMenu() const
    {
        return std::string("");
    }


    //////////////////////////////////////////////////////////////////////////
    // MainMenu
    //////////////////////////////////////////////////////////////////////////

    const std::string MainMenu::MENU_NAME("MainMenu");
    const std::string MainMenu::VERSION_FONT("fonts/menufont.fnt");
    const std::string MainMenu::BACKGROUND("textures/menu/background.tga");
    const float       MainMenu::ITEM_HEIGHT  = 32.0f;
    const float       MainMenu::ITEM_SPACING = 2.0f;

    void MainMenu::Init()
    {
        FONTS.Add(VERSION_FONT);
        TEXTURES.Add(BACKGROUND);

        MenuItem::Init();

        // Add menu items
        float x = APP.GetScreenWidth() / 2.0f;
        float y =
            APP.GetScreenHeight() - 3.0f * ITEM_HEIGHT - 7.0f * ITEM_SPACING;

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y),
                                             "Start Game", 
                                             MENUITEM_START,
                                             ITEM_HEIGHT,
                                             true)));
        y += ITEM_HEIGHT + ITEM_SPACING;

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y),
                                             "High Scores",
                                             MENUITEM_SCORES,
                                             ITEM_HEIGHT,
                                             false)));
        y += ITEM_HEIGHT + ITEM_SPACING;

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y),
                                             "Quit",
                                             MENUITEM_QUIT,
                                             ITEM_HEIGHT,
                                             false)));
    }

    void MainMenu::Draw()
    {
        const float y =
            APP.GetScreenHeight() - 3.0f * ITEM_HEIGHT - 7.0f * ITEM_SPACING;

        DrawTexturedRect(BACKGROUND, 0.0f, 0.0f, APP.GetScreenWidth(),
                        APP.GetScreenHeight());

        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, y - ITEM_SPACING,
            APP.GetScreenWidth(), APP.GetScreenHeight() - y + ITEM_SPACING);

        // Draw the version number
        using namespace boost;
        FONTS.Print(VERSION_FONT,
                    APP.GetScreenWidth(),
                    APP.GetScreenHeight() - 12.0f,
                    12.0f,
                    ColourRGBA::White(),
                    Font::ALIGN_RIGHT,
                    str(format("v%1%.%2%%3% - %4%")
                        % App::MAJOR_VERSION
                        % App::MINOR_VERSION
                        % App::PRE_RELEASE_STRING
                        % __DATE__));

        // Draw the menu items
        MenuScreen::Draw();
    }

    MenuScreen::NextAction MainMenu::OnMenuItemChoose(unsigned int id)
    {
        switch (id)
        {
        case MENUITEM_START:
            GAME.StartNewGame();
            return ACTION_GAME;

        case MENUITEM_SCORES:
            m_nextMenu = HighScoresMenu::MENU_NAME;
            return ACTION_NEXT;

        case MENUITEM_QUIT:
            m_nextMenu = QuitConfirmMenu::MENU_NAME;
            return ACTION_NEXT;

        default:
            return ACTION_NONE;
        }
    }

    const std::string MainMenu::NextMenu() const
    {
        return m_nextMenu;
    }
}
