#include "MenuPause.h"
#include "App.h"
#include "FontManager.h"
#include "Utils.h"
#include "Game.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // EndGameConfirmMenu
    //////////////////////////////////////////////////////////////////////////

    const std::string EndGameConfirmMenu::MENU_NAME("EndGameConfirmMenu");
    const std::string EndGameConfirmMenu::FONT("fonts/menufont.fnt");
    const float       EndGameConfirmMenu::ITEM_HEIGHT  = 32.0f;
    const float       EndGameConfirmMenu::ITEM_SPACING = 8.0f;
    const float       EndGameConfirmMenu::TITLE_HEIGHT = 48.0f;
    const float       EndGameConfirmMenu::TITLE_OFFSET = 16.0f;

    void EndGameConfirmMenu::Init()
    {
        FONTS.Add(FONT);
        MenuItem::Init();

        // Add menu items
        float x = APP.GetScreenWidth() / 2.0f;
        float y = (MENUITEM_COUNT * ITEM_HEIGHT + (MENUITEM_COUNT - 1) * ITEM_SPACING) / 2.0f;
        y = (APP.GetScreenHeight() / 2.0f) - y;

        m_titleOrigin.Set(x, y - TITLE_OFFSET - TITLE_HEIGHT);

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y), "Yes",  MENUITEM_YES,  ITEM_HEIGHT, true)));
        y += ITEM_HEIGHT + ITEM_SPACING;

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y), "No",   MENUITEM_NO,   ITEM_HEIGHT, false)));
    }

    void EndGameConfirmMenu::Draw()
    {
        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, 0.0f,
            APP.GetScreenWidth(), APP.GetScreenHeight());

        FONTS.Print(FONT, m_titleOrigin.GetX(), m_titleOrigin.GetY(), TITLE_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER, "Are you sure?");

        // Draw the menu items
        MenuScreen::Draw();
    }

    MenuScreen::NextAction EndGameConfirmMenu::OnMenuItemChoose(unsigned int id)
    {
        switch(id)
        {
        case MENUITEM_YES:
            GAME.Pause(false);
            GAME.EndGame();
            return ACTION_GAME;

        case MENUITEM_NO:
            return ACTION_PREV;

        default:
            return ACTION_NONE;
        }
    }

    const std::string EndGameConfirmMenu::NextMenu() const
    {
        return std::string("");
    }


    //////////////////////////////////////////////////////////////////////////
    // PauseMenu
    //////////////////////////////////////////////////////////////////////////

    const std::string PauseMenu::MENU_NAME("PauseMenu");
    const std::string PauseMenu::FONT("fonts/menufont.fnt");
    const float       PauseMenu::ITEM_HEIGHT  = 32.0f;
    const float       PauseMenu::ITEM_SPACING = 8.0f;
    const float       PauseMenu::TITLE_HEIGHT = 48.0f;
    const float       PauseMenu::TITLE_OFFSET = 16.0f;

    void PauseMenu::Init()
    {
        FONTS.Add(FONT);
        MenuItem::Init();

        // Add menu items
        float x = APP.GetScreenWidth() / 2.0f;
        float y = (MENUITEM_COUNT * ITEM_HEIGHT + (MENUITEM_COUNT - 1) * ITEM_SPACING) / 2.0f;
        y = (APP.GetScreenHeight() / 2.0f) - y;

        m_titleOrigin.Set(x, y - TITLE_OFFSET - TITLE_HEIGHT);

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y), "Resume Game",  MENUITEM_RESUME,  ITEM_HEIGHT, true)));
        y += ITEM_HEIGHT + ITEM_SPACING;

        AddMenuItem(MenuItemPtr(new MenuItem(juzutil::Vector2(x, y), "End Game",     MENUITEM_QUIT, ITEM_HEIGHT, false)));
    }

    void PauseMenu::Draw()
    {
        DrawRect(ColourRGBA(0.0f, 0.0f, 0.0f, 0.5f), 0.0f, 0.0f,
            APP.GetScreenWidth(), APP.GetScreenHeight());

        FONTS.Print(FONT, m_titleOrigin.GetX(), m_titleOrigin.GetY(), TITLE_HEIGHT, ColourRGBA::White(), Font::ALIGN_CENTER, "Game Paused");

        // Draw the menu items
        MenuScreen::Draw();
    }

    MenuScreen::NextAction PauseMenu::OnMenuItemChoose(unsigned int id)
    {
        switch (id)
        {
        case MENUITEM_RESUME:
            GAME.Pause(false);
            return ACTION_GAME;

        case MENUITEM_QUIT:
            return ACTION_NEXT;

        default:
            return ACTION_NONE;
        }
    }

    const std::string PauseMenu::NextMenu() const
    {
        return EndGameConfirmMenu::MENU_NAME;
    }
}
