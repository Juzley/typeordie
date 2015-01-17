#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Explosion.h"
#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Utils.h"
#include "Shape.h"
#include "Random.h"

namespace typing
{
    const std::string Explosion::EXPLOSION_SOUND("sounds/explosion.wav");
    const std::string Explosion::FLARE_TEXTURE("textures/game/flare.tga");
    const float       Explosion::START_SPEED        = 400.0f;
    const float       Explosion::ACCEL              = -200.0f;
    const float       Explosion::START_ALPHA        = 0.8f;
    const float       Explosion::FRAGMENT_SIZE      = 5.0f;
    const float       Explosion::LIFETIME           = 0.3f;
    const float       Explosion::FLARE_START_SIZE   = 10.0f;
    const float       Explosion::FLARE_EXPAND_SPEED = 2000.0f;
    const float       Explosion::FLARE_START_ALPHA  = 1.0f;
    const float       Explosion::FLARE_ALPHA_FADE   = 7.0f;

    void Explosion::Init()
    {
        SOUNDS.Add(EXPLOSION_SOUND);
        TEXTURES.Add(FLARE_TEXTURE);
    }


    Explosion::Explosion(const juzutil::Vector3& origin, const ColourRGBA& colour)
        : m_origin(origin), m_colour(colour), m_age(0.0f), m_fadeSpeed(colour.GetAlpha() / LIFETIME)
    {
        for(unsigned int i = 0; i < FRAGMENTS; ++i)
        {
            m_fragments[i].m_origin = origin;
            m_fragments[i].m_dir[0] = RAND.Range(-0.5f, 0.5f);
            m_fragments[i].m_dir[1] = RAND.Range(-0.5f, 0.5f);
            m_fragments[i].m_dir[2] = RAND.Range(-0.5f, 0.5f);
            m_fragments[i].m_dir.Normalize();

            m_fragments[i].m_speed = START_SPEED;
            m_fragments[i].m_accel = ACCEL;
            m_fragments[i].m_alpha = START_ALPHA;
        }
    }

    bool Explosion::Unlink()
    {
        if (m_age >= LIFETIME)
        {
            return true;
        }

        return false;
    }

    void Explosion::Update()
    {
        m_age += GAME.GetFrameTime();

        for(unsigned int i = 0; i < FRAGMENTS; ++i)
        {
            m_fragments[i].m_origin += m_fragments[i].m_dir * m_fragments[i].m_speed * GAME.GetFrameTime();
            m_fragments[i].m_speed  += m_fragments[i].m_accel * GAME.GetFrameTime();
            m_fragments[i].m_alpha  -= m_fadeSpeed * GAME.GetFrameTime();
        }
    }


    void Explosion::Draw()
    {
        for(unsigned int i = 0; i < FRAGMENTS; ++i)
        {
            glPushMatrix();
                glTranslatef(m_fragments[i].m_origin[0], m_fragments[i].m_origin[1], m_fragments[i].m_origin[2]);
                glScalef(FRAGMENT_SIZE, FRAGMENT_SIZE, FRAGMENT_SIZE);
                DrawCube(ColourRGBA(m_colour.ToRGB(), m_fragments[i].m_alpha));
            glPopMatrix();
        }

        const float flareSize = FLARE_START_SIZE + m_age * FLARE_EXPAND_SPEED;
        const float alpha = FLARE_START_ALPHA - m_age * FLARE_ALPHA_FADE;

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glScalef(flareSize, flareSize, flareSize);

            TEXTURES.Bind(FLARE_TEXTURE);
            glColor4f(1.0f, 1.0f, 1.0f, alpha);

            glBegin(GL_QUADS);
                juzutil::Vector3 vertex = (-GAME.GetCam().GetRight() - GAME.GetCam().GetUp()) / 2.0f;
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(vertex[0], vertex[1], vertex[2]);

                vertex += GAME.GetCam().GetRight();
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(vertex[0], vertex[1], vertex[2]);

                vertex += GAME.GetCam().GetUp();
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(vertex[0], vertex[1], vertex[2]);

                vertex -= GAME.GetCam().GetRight();
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(vertex[0], vertex[1], vertex[2]);
            glEnd();
        glPopMatrix();
    }


    void Explosion::OnSpawn()
    {
        SOUNDS.Play(EXPLOSION_SOUND);
    }
}
