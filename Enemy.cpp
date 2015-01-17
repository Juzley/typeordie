#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Enemy.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Explosion.h"
#include "Laser.h"
#include "PhraseBook.h"
#include "Game.h"
#include "Shape.h"
#include "Colour.h"
#include "Random.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // BasicEnemy
    //////////////////////////////////////////////////////////////////////////

    const BBox        BasicEnemy::BASICENEMY_BOUNDS; /* ( todo ); */
    const ColourRGBA  BasicEnemy::BASICENEMY_COLOUR(1.0f, 0.85f, 0.0f, 0.4f);
    const ColourRGBA  BasicEnemy::BASICENEMY_OUTLINECOLOUR(1.0f, 0.9f, 0.8f, 1.0f);

    void BasicEnemy::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void BasicEnemy::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
            glScalef(10.0f, 40.0f, 10.0f);
            DrawPyramid(BASICENEMY_COLOUR, BASICENEMY_OUTLINECOLOUR);
        glPopMatrix();
    }

    void BasicEnemy::OnSpawn()
    {
        m_dir = GAME.GetPlayerOrigin() - m_origin;
        m_dir.Normalize();
        m_angle = (atan2(m_dir[0], -m_dir[1]) / static_cast<float>(M_PI) * 180.0f);
    }

    void BasicEnemy::Update()
    {
        // Move towards the player
        m_origin += m_dir * (m_speed * GAME.GetFrameTime());
    }

    void BasicEnemy::OnCollide()
    {
        GAME.MakeCharAvail(m_phrase.GetStartChar());
        GAME.Damage();
        m_unlink = true;
    }

    void BasicEnemy::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, BASICENEMY_COLOUR)));
            m_unlink = true;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // AccelEnemy
    //////////////////////////////////////////////////////////////////////////

    const BBox        AccelEnemy::ACCELENEMY_BOUNDS; /* ( todo ); */
    const ColourRGBA  AccelEnemy::ACCELENEMY_COLOUR(0.1f, 0.1f, 0.1f, 0.4f);
    const ColourRGBA  AccelEnemy::ACCELENEMY_OUTLINECOLOUR(0.8f, 0.8f, 0.8f, 1.0f);
    const float       AccelEnemy::ACCELENEMY_ACCEL = 2.0f;

    void AccelEnemy::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void AccelEnemy::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
            glPushMatrix();
                glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
                glScalef(7.5f, 40.0f, 7.5f);
                DrawPyramid(ACCELENEMY_COLOUR, ACCELENEMY_OUTLINECOLOUR);
            glPopMatrix();
        glPopMatrix();
    }

    void AccelEnemy::OnSpawn()
    {
        m_dir = GAME.GetPlayerOrigin() - m_origin;
        m_dir.Normalize();
        m_angle = (atan2(m_dir[0], -m_dir[1]) / static_cast<float>(M_PI) * 180.0f);
    }

    void AccelEnemy::Update()
    {
        // Move towards the player
        m_origin += m_dir * (m_speed * GAME.GetFrameTime());
    }

    void AccelEnemy::OnCollide()
    {
        GAME.MakeCharAvail(m_phrase.GetStartChar());
        GAME.Damage();
        m_unlink = true;
    }

    void AccelEnemy::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (!*hit) {
            m_speed *= ACCELENEMY_ACCEL;
        }

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, ACCELENEMY_COLOUR)));
            m_unlink = true;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // Missile
    //////////////////////////////////////////////////////////////////////////

    const float      Missile::MISSILE_SPEED = 150.0f;
    const ColourRGBA Missile::MISSILE_COLOUR(1.0f, 0.0f, 0.0f, 0.4f);
    const ColourRGBA Missile::MISSILE_OUTLINECOLOUR(1.0f, 0.8f, 0.8f, 1.0f);

    void Missile::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void Missile::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
            glScalef(3.0f, 16.0f, 3.0f);
            DrawPyramid(MISSILE_COLOUR, MISSILE_OUTLINECOLOUR);
        glPopMatrix();
    }

    void Missile::OnSpawn()
    {
        m_dir = GAME.GetPlayerOrigin() - m_origin;
        m_dir.Normalize();
        m_angle = (atan2(m_dir[0], -m_dir[1]) / static_cast<float>(M_PI) * 180.0f);
    }

    void Missile::Update()
    {
        m_origin += m_dir * (MISSILE_SPEED * GAME.GetFrameTime());
    }

    void Missile::OnCollide()
    {
        GAME.MakeCharAvail(m_phrase.GetStartChar());
        GAME.Damage();
        m_unlink = true;
    }

    void Missile::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, MISSILE_COLOUR)));
            m_unlink = true;
        }
    }


    ////////////////////////////////////////////////////////////////////////////
    // MissileEnemy
    ////////////////////////////////////////////////////////////////////////////

    const float            MissileEnemy::MISSILEENEMY_SPEED     = 40.0f;
    const float            MissileEnemy::MISSILEENEMY_FIREPAUSE = 3.0f;
    const ColourRGBA       MissileEnemy::MISSILEENEMY_COLOUR(1.0f, 0.20f, 0.0f, 0.4f);
    const ColourRGBA       MissileEnemy::MISSILEENEMY_OUTLINECOLOUR(1.0f, 0.9f, 0.8f, 1.0f);

    void MissileEnemy::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void MissileEnemy::Draw3D()
    {
        const juzutil::Vector3 dirToPlayer = GAME.GetPlayerOrigin() - m_origin;
        const float turretAngle = (atan2(dirToPlayer[0], -dirToPlayer[1]) / static_cast<float>(M_PI) * 180.0f);

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glPushMatrix();
                glRotatef(turretAngle, 0.0f, 0.0f, 1.0f);
                glScalef(8.0f, 30.0f, 8.f);
                DrawPyramid(MISSILEENEMY_COLOUR, MISSILEENEMY_OUTLINECOLOUR);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -8.0f);
                glScalef(30.0f, 20.0f, 20.0f);
                DrawCube(MISSILEENEMY_COLOUR, MISSILEENEMY_OUTLINECOLOUR);
            glPopMatrix();
        glPopMatrix();
    }

    void MissileEnemy::OnSpawn()
    {
        m_angle = (atan2(m_dir[0], -m_dir[1]) / static_cast<float>(M_PI) * 180.0f);
        m_lastFireTime = GAME.GetTime() - MISSILEENEMY_FIREPAUSE / 2.0f;
    }

    void MissileEnemy::Update()
    {
        m_origin += m_dir * (MISSILEENEMY_SPEED * GAME.GetFrameTime());

        if ((m_dir[0] > 0 && m_origin[0] > Game::GAME_SCREEN_RIGHT) ||
            (m_dir[0] < 0 && m_origin[0] < Game::GAME_SCREEN_LEFT) ||
            (m_dir[1] > 0 && m_origin[1] > Game::GAME_SCREEN_TOP) ||
            (m_dir[1] < 0 && m_origin[1] < Game::GAME_SCREEN_BOTTOM)) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            m_unlink = true;
        }

        if (GAME.GetTime() - m_lastFireTime > MISSILEENEMY_FIREPAUSE)
        {
            GAME.AddEntity(MissilePtr(new Missile(GAME.GetPhrase(PhraseBook::PL_SINGLE), m_origin)));
            m_lastFireTime = GAME.GetTime();
        }
    }


    void MissileEnemy::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, MISSILEENEMY_COLOUR)));
            m_unlink = true;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // BombEnemy
    //////////////////////////////////////////////////////////////////////////

    const ColourRGBA   BombEnemy::BOMB_COLOUR(0.3f, 0.0f, 0.5f, 0.9f);
    const ColourRGBA   BombEnemy::BOMB_OUTLINECOLOUR(0.5f, 0.0f, 1.0f, 1.0f);
    const float        BombEnemy::BOMB_MAX_ROTATE_SPEED = 15.0f;
    const float        BombEnemy::BOMB_DETONATE_TIME    = 10.0f;
    const float        BombEnemy::BOMB_BLINK_TIME       = 3.0f;
    const float        BombEnemy::BOMB_BLINK_SPEED      = 8.0f;

    void BombEnemy::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void BombEnemy::Draw3D()
    {
        ColourRGBA outlineColour(BOMB_OUTLINECOLOUR);
        const float blinkTime = (GAME.GetTime() - m_spawnTime) - (BOMB_DETONATE_TIME - BOMB_BLINK_TIME);
        if (blinkTime > 0.0f) {
            outlineColour[ColourRGBA::COLOUR_ALPHA] = BOMB_OUTLINECOLOUR.GetAlpha() *
                abs(cosf((blinkTime / BOMB_BLINK_TIME) * static_cast<float>(M_PI) * BOMB_BLINK_SPEED));
        }

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glRotatef(m_angles[0], 0.0f, 0.0f, 1.0f);
            glRotatef(m_angles[1], 0.0f, 1.0f, 0.0f);
            glRotatef(m_angles[2], 1.0f, 0.0f, 0.0f);
            glScalef(10.0f, 10.0f, 10.0f);
            DrawDodecahedron(BOMB_COLOUR, outlineColour);
        glPopMatrix();
    }

    void BombEnemy::OnSpawn()
    {
        m_angleSpeed.Set(RAND.Range(-BOMB_MAX_ROTATE_SPEED, BOMB_MAX_ROTATE_SPEED),
                         RAND.Range(-BOMB_MAX_ROTATE_SPEED, BOMB_MAX_ROTATE_SPEED),
                         RAND.Range(-BOMB_MAX_ROTATE_SPEED, BOMB_MAX_ROTATE_SPEED));
        m_spawnTime = GAME.GetTime();
    }


    void BombEnemy::Update()
    {
        m_angles += m_angleSpeed * GAME.GetFrameTime();

        // TODO: Add a warning sound when the bomb is about to explode

        if (GAME.GetTime() - m_spawnTime >= BOMB_DETONATE_TIME) {
            Detonate();
        }
    }

    void BombEnemy::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (!*hit) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            Detonate();
            *phraseFinished = true;
        } else if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, BOMB_COLOUR)));
            m_unlink = true;
        }
    }

    void BombEnemy::Detonate()
    {
        GAME.Damage();

        ExplosionPtr explosion(new Explosion(m_origin, BOMB_COLOUR));
        GAME.AddEffect(explosion);

        LaserPtr laser(new Laser(m_origin, GAME.GetPlayerOrigin(), BOMB_COLOUR.ToRGB()));
        GAME.AddEffect(laser);

        m_unlink = true;
    }


    //////////////////////////////////////////////////////////////////////////
    // SeekerEnemy
    //////////////////////////////////////////////////////////////////////////

    const ColourRGBA   SeekerEnemy::SEEKER_COLOUR(0.3f, 0.0f, 0.5f, 0.9f);
    const ColourRGBA   SeekerEnemy::SEEKER_OUTLINECOLOUR(0.5f, 0.0f, 1.0f, 1.0f);
    const float        SeekerEnemy::SEEKER_SEEK_TIME = 3.0f;
    const float        SeekerEnemy::SEEKER_TURN_SPEED = 175.0f;
    const float        SeekerEnemy::SEEKER_SEEK_MOVE_SPEED = 60.0f;
    const float        SeekerEnemy::SEEKER_ATTACK_MOVE_SPEED = 250.0f;

    void SeekerEnemy::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void SeekerEnemy::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
            glScalef(10.0f, 40.0f, 10.0f);
            DrawPyramid(SEEKER_COLOUR, SEEKER_OUTLINECOLOUR);
        glPopMatrix();
    }

    void SeekerEnemy::OnSpawn()
    {
        m_seeking = true;
        m_turning = false;
        m_angle = 0.0f;
        m_spawnTime = GAME.GetTime();
        m_dir.Set(0.0f, -1.0f, 0.0f);
    }


    void SeekerEnemy::Update()
    {
        if (m_seeking) {
            // Spent long enough seeking, lock onto the player and attack.
            if (GAME.GetTime() - m_spawnTime >= SEEKER_SEEK_TIME) {
                StartAttack();
            } else {
                m_origin += m_dir * SEEKER_SEEK_MOVE_SPEED * GAME.GetFrameTime();
            }
        } else if (m_turning) {
            float newAngle = m_angle;
            if (m_destAngle > m_angle) {
                newAngle = m_angle + GAME.GetFrameTime() * SEEKER_TURN_SPEED;
                
                if (newAngle >= m_destAngle) {
                    m_turning = false;
                    newAngle = m_destAngle;
                }
            } else if (m_destAngle < m_angle) {
                newAngle = m_angle - GAME.GetFrameTime() * SEEKER_TURN_SPEED;

                if (newAngle <= m_destAngle) {
                    m_turning = false;
                    newAngle = m_destAngle;
                }
            } else {
                m_turning = false;
            }

            m_angle = newAngle;
        } else {
            m_origin += m_dir * SEEKER_ATTACK_MOVE_SPEED * GAME.GetFrameTime();
        }
    }

    void SeekerEnemy::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            GAME.AddEffect(ExplosionPtr(new Explosion(m_origin, SEEKER_COLOUR)));
            m_unlink = true;
        } else if (m_seeking) {
            StartAttack();
        }
    }

    void SeekerEnemy::OnCollide()
    {
        GAME.MakeCharAvail(m_phrase.GetStartChar());
        GAME.Damage();
        m_unlink = true;
    }

    void SeekerEnemy::StartAttack()
    {
        if (m_seeking) {
            m_turning = true;
            m_seeking = false;

            m_dir = GAME.GetPlayerOrigin() - m_origin;
            m_dir.Normalize();
            m_destAngle = (atan2(m_dir[0], -m_dir[1]) / static_cast<float>(M_PI) * 180.0f);
        }
    }
}
