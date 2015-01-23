#include "Boss.h"
#include "Enemy.h"
#include "Game.h"
#include "PhraseBook.h"
#include "Explosion.h"
#include "Shape.h"
#include "Laser.h"


namespace typing
{
    // The bosses start from the top of the screen and move in before
    // starting their pattern.
    const juzutil::Vector3 BOSS_START_ORIGIN(0.0f, 1400.0f, 0.0f);
    const juzutil::Vector3 BOSS_DEST_ORIGIN(0.0f, 800.0f, 0.0f);
    const float            BOSS_DEST_EPSILON = 5.0f;
    const float            BOSS_ENTRY_SPEED = 100.0f;

    /*************************************************************************
     * Knockback Boss                                                        *
     *************************************************************************/

    const float KNOCKBACKBOSS_ADVANCE_SPEED = 120.0f; 
    const float KNOCKBACKBOSS_KNOCKBACK_DISTANCE = 200.0f;
    const unsigned int KNOCKBACKBOSS_WORD_COUNT = 2;
    const enum PhraseBook::PhraseLength KNOCKBACKBOSS_PHRASE_LENGTH =
                                                        PhraseBook::PL_SHORT;
    const ColourRGBA KNOCKBACKBOSS_COLOUR(0.6f, 0.6f, 1.0f, 0.4f);
    const ColourRGBA KNOCKBACKBOSS_LINE_COLOUR(0.6f, 0.6f, 1.0f, 1.0f);

    void KnockbackBoss::OnSpawn()
    {
        m_phrase.Reset("");
        m_health = KNOCKBACKBOSS_HEALTH;

        m_origin = BOSS_START_ORIGIN;
        m_moving = !m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON);
    }

    void KnockbackBoss::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void KnockbackBoss::Draw3D()
    {
        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);

            glPushMatrix();
                glTranslatef(0.0f, 100.0f, 0.0f);
                glScalef(30.0f, 100.0f, 30.0f);
                DrawPyramid(KNOCKBACKBOSS_COLOUR,
                            KNOCKBACKBOSS_LINE_COLOUR);
            glPopMatrix();
        glPopMatrix();
    }

    void KnockbackBoss::Update()
    {
        if (m_moving) {
            juzutil::Vector3 dir = (BOSS_DEST_ORIGIN - m_origin);
            dir.Normalize();
            m_origin += dir * (BOSS_ENTRY_SPEED * GAME.GetFrameTime());

            if (m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON)) {
                m_phrase.Reset(GAME.GetComboPhrase(
                    KNOCKBACKBOSS_WORD_COUNT,
                    KNOCKBACKBOSS_PHRASE_LENGTH));
                m_moving = false;
            }
        } else {
            juzutil::Vector3 dir = GAME.GetPlayerOrigin() - m_origin;
            dir.Normalize();
            m_origin += dir * (KNOCKBACKBOSS_ADVANCE_SPEED *
                                                        GAME.GetFrameTime());
        }
    }

    void KnockbackBoss::OnType(char c, bool *hit, bool *phraseFinished)
    {
        if (!m_moving) {
            *hit = m_phrase.OnType(c, GAME.GetTime());
            *phraseFinished = m_phrase.Finished();

            if (*phraseFinished) {
                GAME.MakeCharAvail(m_phrase.GetStartChar());
                m_health--;
                if (m_health > 0) {
                    m_phrase.Reset(GAME.GetComboPhrase(
                        KNOCKBACKBOSS_WORD_COUNT,
                        KNOCKBACKBOSS_PHRASE_LENGTH));

                    // Knock the boss back
                    juzutil::Vector3 dir = m_origin - GAME.GetPlayerOrigin();
                    dir.Normalize();
                    m_origin += dir * KNOCKBACKBOSS_KNOCKBACK_DISTANCE;
                } else {
                    ExplosionPtr explosion(new Explosion(
                                            m_origin, KNOCKBACKBOSS_COLOUR));
                    GAME.AddEffect(explosion);
                }
            }
        }
    }

    void KnockbackBoss::OnCollide()
    {
        GAME.MakeCharAvail(m_phrase.GetStartChar());
        GAME.Damage();
        ExplosionPtr explosion(new Explosion(m_origin, KNOCKBACKBOSS_COLOUR));
        GAME.AddEffect(explosion);
        // When the boss hits the player, move it back to the start and reset
        // the phrase.
        m_phrase.Reset(GAME.GetComboPhrase(KNOCKBACKBOSS_WORD_COUNT,
                                           KNOCKBACKBOSS_PHRASE_LENGTH));
        m_origin = BOSS_DEST_ORIGIN;
    }

    
    /*************************************************************************
     * Charge Boss                                                           *
     *************************************************************************/

    // Charge time is calculated as base + scale * health, so the charge gets
    // quicker as the boss gets closer to death.
    const float ChargeBoss::CHARGEBOSS_BASE_CHARGE_TIME  = 2.0f;
    const float ChargeBoss::CHARGEBOSS_CHARGE_TIME_SCALE = 0.5f;

    void ChargeBoss::OnSpawn()
    {
        m_phrase.Reset("");
        m_health = CHARGEBOSS_HEALTH;
        
        m_origin = BOSS_START_ORIGIN;
        m_moving = !m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON);

        m_colour = ColourRGBA::White();
        m_colour[ColourRGBA::COLOUR_ALPHA] = 0.4f;
    }

    void ChargeBoss::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void ChargeBoss::Draw3D()
    {
        // This boss starts white and gets ready the closer it gets to
        // charging.
        m_colour[ColourRGBA::COLOUR_GREEN] =
        m_colour[ColourRGBA::COLOUR_BLUE] =
                                    (m_nextChargeFinishTime - GAME.GetTime()) /
                                    CHARGEBOSS_BASE_CHARGE_TIME;

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);

            glPushMatrix();
                glTranslatef(0.0f, 100.0f, 0.0f);
                glScalef(30.0f, 100.0f, 30.0f);
                DrawPyramid(m_colour);
            glPopMatrix();
        glPopMatrix();
    }

    void ChargeBoss::Update()
    {
        if (m_moving) {
            juzutil::Vector3 dir = (BOSS_DEST_ORIGIN - m_origin);
            dir.Normalize();
            m_origin += dir * (BOSS_ENTRY_SPEED * GAME.GetFrameTime());

            m_moving = !m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON);
            if (!m_moving) {
                m_phrase.Reset(GAME.GetComboPhrase(3, PhraseBook::PL_LONG));
                CalcNextChargeTime();
            }
        } else {
            if (m_nextChargeFinishTime <= GAME.GetTime()) {
                GAME.Damage();
                LaserPtr laser(new Laser(m_origin,
                                         GAME.GetPlayerOrigin(),
                                         ColourRGB::Red()));
                GAME.AddEffect(laser);
                CalcNextChargeTime();
            }
        }
    }

    void ChargeBoss::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());
        *phraseFinished = m_phrase.Finished();

        if (*phraseFinished) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            --m_health;
            if (m_health > 0) {
                m_phrase.Reset(GAME.GetComboPhrase(3, PhraseBook::PL_LONG));
                CalcNextChargeTime();
            } else {
                ExplosionPtr explosion(new Explosion(m_origin, m_colour));
                GAME.AddEffect(explosion);
            }
        }
    }

    /*************************************************************************
     * Missile Boss                                                          *
     *************************************************************************/

    const float            MissileBoss::MISSILEBOSS_WAVE_GAP     = 4.0f;
    const float            MissileBoss::MISSILEBOSS_MISSILE_GAP  = 0.5f;   

    void MissileBoss::OnSpawn()
    {
        m_phrase.Reset("");
        m_health = MISSILEBOSS_HEALTH;
        m_currentWaveMissilesFired = 0;

        m_origin = BOSS_START_ORIGIN;
        m_moving = !m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON);
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
        if (m_moving) {
            juzutil::Vector3 dir = (BOSS_DEST_ORIGIN - m_origin);
            dir.Normalize();
            m_origin += dir * (BOSS_ENTRY_SPEED * GAME.GetFrameTime());

            m_moving = !m_origin.Equals(BOSS_DEST_ORIGIN, BOSS_DEST_EPSILON);
            if (!m_moving) {
                m_phrase.Reset(GAME.GetComboPhrase(3, PhraseBook::PL_MEDIUM));

                m_nextMissileFireTime = GAME.GetTime();
            }
        } else { 
            if (m_nextMissileFireTime <= GAME.GetTime()) {
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
                m_phrase.Reset(GAME.GetComboPhrase(3, PhraseBook::PL_MEDIUM));
            } else {
                ExplosionPtr explosion(new Explosion(m_origin, ColourRGBA::White()));
                GAME.AddEffect(explosion);
            }
        }
    }
}
