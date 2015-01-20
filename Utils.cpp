#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "App.h"
#include "Game.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Colour.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // Drawing utility funcs
    //////////////////////////////////////////////////////////////////////////

    void DrawTexturedRect(const std::string& texture, float x, float y, float width, float height)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        TEXTURES.Bind(texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(x, y + height);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(x + width, y + height);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(x + width, y);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(x, y);
        glEnd();
    }

    void DrawTexturedRect(const std::string& texture, const ColourRGBA& col, float x, float y, float width, float height)
    {
        glColor4f(col.GetRed(), col.GetGreen(), col.GetBlue(), col.GetAlpha());
        TEXTURES.Bind(texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(x, y + height);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(x + width, y + height);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(x + width, y);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(x, y);
        glEnd();
    }

    void DrawRect(ColourRGBA col, float x, float y, float width, float height)
    {
        glColor4f(col.GetRed(), col.GetGreen(), col.GetBlue(), col.GetAlpha());
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glVertex2f(x, y + height);
            glVertex2f(x + width, y + height);
            glVertex2f(x + width, y);
            glVertex2f(x, y);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    void DrawLine(const ColourRGBA& col, float startX, float startY, float startZ, float endX, float endY, float endZ)
    {
        glColor4f(col.GetRed(), col.GetGreen(), col.GetBlue(), col.GetAlpha());
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_LINES);
            glVertex3f(startX, startY, startZ);
            glVertex3f(endX, endY, endZ);
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

    void DrawLine(const ColourRGBA& col, const juzutil::Vector3& start, const juzutil::Vector3& end)
    {
        DrawLine(col, start[0], start[1], start[2], end[0], end[1], end[2]);
    }

    void DrawLine(const ColourRGBA& col, float startX, float startY, float endX, float endY)
    {
        DrawLine(col, startX, startY, 0.0f, endX, endY, 0.0f);
    }

    void DrawLine(const ColourRGBA& col, const juzutil::Vector2& start, const juzutil::Vector2& end)
    {
        DrawLine(col, start[0], start[1], 0.0f, end[0], end[1], 0.0f);
    }

    //////////////////////////////////////////////////////////////////////////
    // Entity spawn utility funcs
    //////////////////////////////////////////////////////////////////////////

    // Selects a random spawn point for an enemy at the edge of the screen.
    const juzutil::Vector2 SelectEdgeSpawnPoint(std::mt19937& random)
    {
        enum screenSide { SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM };
        const float SIDE_OFFSET = 30.0f;

        std::uniform_int_distribution<> sides(SCREEN_LEFT, SCREEN_BOTTOM);

        screenSide side = static_cast<screenSide>(sides(random));

        std::uniform_real_distribution<> distGen(0, 1);
        float dist = static_cast<float>(distGen(random));

        switch (side)
        {
        case SCREEN_LEFT:
            return juzutil::Vector2(0.0f + SIDE_OFFSET, dist * APP.GetScreenHeight());

        case SCREEN_TOP:
            return juzutil::Vector2(dist * APP.GetScreenWidth(), 0.0f + SIDE_OFFSET);

        case SCREEN_RIGHT:
            return juzutil::Vector2(APP.GetScreenWidth() - SIDE_OFFSET, dist * APP.GetScreenHeight());

        case SCREEN_BOTTOM:
            return juzutil::Vector2(dist * APP.GetScreenWidth(), APP.GetScreenHeight() - SIDE_OFFSET);
        }

        return juzutil::Vector2();
    }

    // Selects a random spawn point for an entity in the middle of the screen.
    const juzutil::Vector2 SelectMidSpawnPoint(std::mt19937& random)
    {
        const float EDGE_DEAD_ZONE = 50.0f;
        const float MID_DEAD_ZONE  = 50.0f;

        std::uniform_real_distribution<> xGen(
                    EDGE_DEAD_ZONE,
                    APP.GetScreenWidth() - MID_DEAD_ZONE - EDGE_DEAD_ZONE);
        float x = static_cast<float>(xGen(random));

        if (x + EDGE_DEAD_ZONE * 2.0f > (APP.GetScreenWidth() - MID_DEAD_ZONE) / 2.0f)
        {
            x += MID_DEAD_ZONE;
        }

        std::uniform_real_distribution<> yGen(
                    EDGE_DEAD_ZONE,
                    APP.GetScreenHeight() - MID_DEAD_ZONE - EDGE_DEAD_ZONE);
        float y = static_cast<float>(yGen(random));

        if (y + EDGE_DEAD_ZONE * 2.0f > (APP.GetScreenHeight() - MID_DEAD_ZONE) / 2.0f)
        {
            y += MID_DEAD_ZONE;
        }

        return juzutil::Vector2(x, y);
    }

    // Selects a direction for an enemy to travel, such that at its closest point to the player,
    // it is a distance 'dist' away from the player.
    const juzutil::Vector2 SelectEnemyDirection(const juzutil::Vector3& spawnPoint, float dist)
    {
        // The direction from the player to the spawn point
        juzutil::Vector2 dir(spawnPoint - GAME.GetPlayerOrigin());
        dir.Normalize();
        dir *= dist;

        // There are two candidate points, one either side of the player
        juzutil::Vector2 a(dir.GetY(), dir.GetX());
        juzutil::Vector2 dirA = a + GAME.GetPlayerOrigin() - spawnPoint;

        juzutil::Vector2 b(-dir.GetY(), dir.GetX());
        juzutil::Vector2 dirB = b + GAME.GetPlayerOrigin() - spawnPoint;

        // Pick the closer of the two candidate points.
        if (dirB.SquareSize() >= dirA.SquareSize())
        {
            dirA.Normalize();
            return dirA;
        }
        else
        {
            dirB.Normalize();
            return dirB;
        }
    }
}
