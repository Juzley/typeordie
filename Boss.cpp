#include "Boss.h"
#include "Enemy.h"
#include "Game.h"
#include "PhraseBook.h"
#include "Explosion.h"
#include "Shape.h"

namespace typing
{
    const juzutil::Vector3 MissileBoss::MISSILEBOSS_START_ORIGIN(0.0f, 700.0f, 0.0f);
    const juzutil::Vector3 MissileBoss::MISSILEBOSS_DEST_ORIGIN(0.0f, 600.0f, 0.0f);
    const float            MissileBoss::MISSILEBOSS_DEST_EPSILON = 5.0f;
    const float            MissileBoss::MISSILEBOSS_WAVE_GAP     = 4.0f;
    const float            MissileBoss::MISSILEBOSS_MISSILE_GAP  = 0.5f;   

    void MissileBoss::OnSpawn()
    {
        m_phrase.Reset("");
        m_health = MISSILEBOSS_HEALTH;
        m_currentWaveMissilesFired = 0;

        m_origin = MISSILEBOSS_START_ORIGIN;
        m_moving = !m_origin.Equals(MISSILEBOSS_DEST_ORIGIN, MISSILEBOSS_DEST_EPSILON);
    }

    void MissileBoss::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void MissileBoss::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);

            glPushMatrix();
            glTranslatef(0.0f, 100.0f, 0.0f);
            glScalef(30.0f, 100.0f, 30.0f);
            DrawPyramid(ColourRGBA::White());
            glPopMatrix();
        glPopMatrix();
    }

    void MissileBoss::Update()
    {
        static const float MOVE_SPEED = 100.0f;

        if (m_moving) {
            juzutil::Vector3 dir = (MISSILEBOSS_DEST_ORIGIN - m_origin);
            dir.Normalize();
            m_origin += dir * (MOVE_SPEED * GAME.GetFrameTime());

            m_moving = !m_origin.Equals(MISSILEBOSS_DEST_ORIGIN, MISSILEBOSS_DEST_EPSILON);
            if (!m_moving) {
                m_phrase.Reset(GAME.GetPhrase(PhraseBook::PL_MEDIUM));

                m_nextMissileFireTime = GAME.GetTime();
            }
        } else { 
            if (m_nextMissileFireTime <= GAME.GetTime())
            {
                MissilePtr mis(new Missile(GAME.GetPhrase(PhraseBook::PL_SINGLE), m_origin));
                GAME.AddEntity(mis);

                if (++m_currentWaveMissilesFired >= MISSILEBOSS_WAVE_MISSILE_COUNT) {
                    m_nextMissileFireTime = GAME.GetTime() + MISSILEBOSS_WAVE_GAP;
                    m_currentWaveMissilesFired = 0;
                } else {
                    m_nextMissileFireTime = GAME.GetTime() + MISSILEBOSS_MISSILE_GAP;
                }
            }
        }
    }

    void MissileBoss::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            --m_health;
            if (m_health > 0) {
                m_phrase.Reset(GAME.GetPhrase(PhraseBook::PL_MEDIUM));
            } else {
                ExplosionPtr explosion(new Explosion(m_origin, ColourRGBA::White()));
                GAME.AddEffect(explosion);
            }
        }
    }

    void MissileBossEnemyWave::Spawn()
    {
        if (!m_spawned) {
            m_ent.reset(new MissileBoss);
            GAME.AddEntity(m_ent);
            m_spawned = true;
        }
    }
}
