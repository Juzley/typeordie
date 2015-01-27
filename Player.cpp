#include "Player.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Game.h"
#include "Shape.h"

namespace typing
{
    const float            Player::FIRE_FADE_TIME          = 1.0f;
    const float            Player::DAMAGE_FADE_TIME        = 1.0f;
    const float            Player::PLAYER_SIDE             = 32.0f;
    const juzutil::Vector3 Player::PLAYER_ORIGIN(0.0f, 0.0f, 0.0f);
    const BBox             Player::PLAYER_BOUNDS(
                -Player::PLAYER_SIDE / 2.0f, -Player::PLAYER_SIDE / 2.0f,
                Player::PLAYER_SIDE / 2.0f, Player::PLAYER_SIDE / 2.0f);

    void Player::Init()
    {
    }

    void Player::Reset(unsigned int lives)
    {
        m_lives = lives;
        m_lastFireTime = 0.0f;
        m_damageTime = 0.0f;
    }

    void Player::Draw()
    {
        // Alpha fades between 0.1 and 0.5
        const float alpha =
            m_lives > 0 ? 
                0.1f + (1.0f + sinf(GAME.GetTime())) / 5.0f : 0.0f;

        glPushMatrix();
            glTranslatef(PLAYER_ORIGIN.GetX(),
                         PLAYER_ORIGIN.GetY(),
                         PLAYER_ORIGIN.GetZ());
            glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
            glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
            glScalef(38.0f, 38.0f, 38.0f);

            DrawCube(ColourRGBA(m_lives > 1 ? 0.7f : 1.0f,
                                m_lives > 1 ? 1.0f : 0.0f,
                                0.0f, alpha),
                     ColourRGBA(0.8f, 1.0f, 0.8f, 1.0f));

            glPushMatrix();
                glScalef(1.1f, 1.1f, 1.1f);
                if (m_lastFireTime != 0.0f && GAME.GetTime() - m_lastFireTime < FIRE_FADE_TIME)
                {
                    const float fireAlpha = 1.0f - (GAME.GetTime() - m_lastFireTime) / FIRE_FADE_TIME;
                    DrawCube(ColourRGBA(1.0f, 1.0f, 1.0f, fireAlpha));
                }

                if (m_damageTime != 0.0f && GAME.GetTime() - m_damageTime < DAMAGE_FADE_TIME)
                {
                    const float damageAlpha = 1.0f - (GAME.GetTime() - m_damageTime) / DAMAGE_FADE_TIME;
                    DrawCube(ColourRGBA(1.0f, 0.0f, 0.0f, damageAlpha));
                }
            glPopMatrix();
        glPopMatrix();
    }

    void Player::Update()
    {
    }

    void Player::Fire()
    {
        m_lastFireTime = GAME.GetTime();
    }

    void Player::Damage()
    {
        m_lives--;
        m_damageTime = GAME.GetTime();
    }
}
