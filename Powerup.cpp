#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Utils.h"
#include "Powerup.h"
#include "Game.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "Shape.h"
#include "Random.h"
#include "Award.h"
#include "Utils.h"

namespace typing
{
    static const float      POWERUP_LIFETIME = 6.0f;
    static const float      POWERUP_ROTATE_SPEED = 0.5f;
    static const float      POWERUP_BLINK_TIME = 2.0f;
    static const float      POWERUP_BLINK_SPEED = 4.0f;
    static const ColourRGBA POWERUP_SPHERE_COLOUR(1.0f, 1.0f, 1.0f, 0.1f);

    //////////////////////////////////////////////////////////////////////////
    // PowerupActivateEffect
    //////////////////////////////////////////////////////////////////////////

    static const std::string POWERUPACTIVATEEFFECT_SOUND(
                                                 "sounds/powerup.wav");
    static const std::string POWERUPACTIVATEEFFECT_FLARE_TEXTURE(
                                                 "textures/game/flare.tga");
    static const float POWERUPACTIVATEEFFECT_LIFETIME = 0.4f; 
    static const float POWERUPACTIVATEEFFECT_FLARE_START_SIZE = 10.0f;
    static const float POWERUPACTIVATEEFFECT_FLARE_EXPAND_SPEED = 1500.0f;
    static const float POWERUPACTIVATEEFFECT_FLARE_START_ALPHA = 1.0f;
    static const float POWERUPACTIVATEEFFECT_FLARE_ALPHA_FADE = 6.0f;

    void PowerupActivateEffect::Init()
    {
        SOUNDS.Add(POWERUPACTIVATEEFFECT_SOUND);
        TEXTURES.Add(POWERUPACTIVATEEFFECT_FLARE_TEXTURE);
    }

    void PowerupActivateEffect::OnSpawn()
    {
        SOUNDS.Play(POWERUPACTIVATEEFFECT_SOUND);
    }

    void PowerupActivateEffect::Update()
    {
        m_age += GAME.GetFrameTime();
    }

    bool PowerupActivateEffect::Unlink()
    {
        return m_age >= POWERUPACTIVATEEFFECT_LIFETIME;
    }

    void PowerupActivateEffect::Draw()
    {
        const float flareSize = POWERUPACTIVATEEFFECT_FLARE_START_SIZE +
            m_age * POWERUPACTIVATEEFFECT_FLARE_EXPAND_SPEED;
        const float flareAlpha = POWERUPACTIVATEEFFECT_FLARE_START_ALPHA -
            m_age * POWERUPACTIVATEEFFECT_FLARE_ALPHA_FADE;

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glScalef(flareSize, flareSize, flareSize);
            TEXTURES.Bind(POWERUPACTIVATEEFFECT_FLARE_TEXTURE);
            glColor4f(0.6f, 1.0f, 0.6f, flareAlpha);

            glBegin(GL_QUADS);
                juzutil::Vector3 vertex =
                    (-GAME.GetCam().GetRight() - GAME.GetCam().GetUp()) / 2.0f;
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
    }


    //////////////////////////////////////////////////////////////////////////
    // PowerupFactory
    //////////////////////////////////////////////////////////////////////////

    PowerupPtr PowerupFactory::Create(const juzutil::Vector3& origin)
    {
        if (m_creators.empty()) {
            throw std::runtime_error("Empty Powerup Factory");
        } else {
            unsigned int index = RAND.Range(
                    static_cast<unsigned int>(0),
                    static_cast<unsigned int>(m_creators.size() - 1));
            PowerupCreator create = m_creators.at(index);
            return create(GAME.GetPhrase(PhraseBook::PL_LONG), origin);
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // ExtraLife
    //////////////////////////////////////////////////////////////////////////
        
    void ExtraLife::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());

        if (!*hit) {
            ExplosionPtr explosion(new Explosion(m_origin,
                                                 ColourRGBA::Red()));
            GAME.AddEffect(explosion);
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            m_unlink = true;
        } else {
            *phraseFinished = m_phrase.Finished();

            if (*phraseFinished) {
                GAME.MakeCharAvail(m_phrase.GetStartChar());
                GAME.AddExtraLife();
                GAME.AddEffect2d(
                    AwardPtr(new Award(
                                GAME.GetCam().PerspectiveProject(m_origin),
                                AWARD_EXTRALIFE,
                                GAME.GetTime())));

                PowerupActivateEffectPtr activate(
                                        new PowerupActivateEffect(m_origin));
                GAME.AddEffect(activate);

                m_unlink = true;
            }
        }
    }

    void ExtraLife::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void ExtraLife::Draw3D()
    {
        ColourRGBA sphereColour(POWERUP_SPHERE_COLOUR);
        const float blinkTime = (GAME.GetTime() - m_spawnTime) -
                                    (POWERUP_LIFETIME - POWERUP_BLINK_TIME);
        if (blinkTime > 0.0f) {
            sphereColour[ColourRGBA::COLOUR_ALPHA] =
                POWERUP_SPHERE_COLOUR.GetAlpha() *
                    fabs(cosf((blinkTime / POWERUP_BLINK_TIME) *
                              static_cast<float>(M_PI) * POWERUP_BLINK_SPEED));
        }

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);

            glPushMatrix();
                float angle =
                    RadToDeg(acosf(fmod(POWERUP_ROTATE_SPEED * 
                                        GAME.GetTime(), 2.0f) - 1.0f));
                glRotatef(angle, 0.0f, 1.0f, 0.0f);

                glPushMatrix();
                    glScalef(7.0f, 18.0f, 7.0f);
                    DrawCube(ColourRGBA::Red());
                glPopMatrix();
                glPushMatrix();
                    glScalef(18.0f, 7.0f, 7.0f);
                    DrawCube(ColourRGBA::Red());
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glScalef(20.0f, 20.0f, 20.0f);
                DrawSphere(ColourRGBA(sphereColour));
            glPopMatrix();
        glPopMatrix();
    }

    void ExtraLife::OnSpawn()
    {
        m_spawnTime = GAME.GetTime();
    }

    void ExtraLife::Update()
    {
        if (GAME.GetTime() - m_spawnTime >= POWERUP_LIFETIME) {
            GAME.MakeCharAvail(GetStartChar());
            m_unlink = true;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // ShortenPhrases
    //////////////////////////////////////////////////////////////////////////
    void ShortenPhrases::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());

        if (!*hit) {
            ExplosionPtr explosion(new Explosion(m_origin,
                                                 ColourRGBA::Blue()));
            GAME.AddEffect(explosion);
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            m_unlink = true;
        } else {
            *phraseFinished = m_phrase.Finished();

            if (*phraseFinished) {
                GAME.MakeCharAvail(m_phrase.GetStartChar());
                GAME.StartShortenPhrases();
                GAME.AddEffect2d(
                    AwardPtr(new Award(
                                GAME.GetCam().PerspectiveProject(m_origin),
                                AWARD_SHORTEN_PHRASES,
                                GAME.GetTime())));

                PowerupActivateEffectPtr activate(
                                    new PowerupActivateEffect(m_origin));
                GAME.AddEffect(activate);

                m_unlink = true;
            }
        }
    }

    void ShortenPhrases::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void ShortenPhrases::Draw3D()
    {
        ColourRGBA sphereColour(POWERUP_SPHERE_COLOUR);
        const float blinkTime = (GAME.GetTime() - m_spawnTime) -
                                    (POWERUP_LIFETIME - POWERUP_BLINK_TIME);
        if (blinkTime > 0.0f) {
            sphereColour[ColourRGBA::COLOUR_ALPHA] =
                POWERUP_SPHERE_COLOUR.GetAlpha() *
                    fabs(cosf((blinkTime / POWERUP_BLINK_TIME) *
                              static_cast<float>(M_PI) * POWERUP_BLINK_SPEED));
        }

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);

            glPushMatrix();
                float angle =
                    RadToDeg(acosf(fmod(POWERUP_ROTATE_SPEED * 
                                        GAME.GetTime(), 2.0f) - 1.0f));
                glRotatef(angle, 0.0f, 1.0f, 0.0f);

                glPushMatrix();
                    glTranslatef(-0.0f, -5.0f, -0.0f);
                    glScalef(10.0f, 10.0f, 10.0f);
                    DrawPyramid(ColourRGBA::Blue());
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glScalef(20.0f, 20.0f, 20.0f);
                DrawSphere(ColourRGBA(sphereColour));
            glPopMatrix();
        glPopMatrix();
    }

    void ShortenPhrases::OnSpawn()
    {
        m_spawnTime = GAME.GetTime();
    }

    void ShortenPhrases::Update()
    {
        if (GAME.GetTime() - m_spawnTime >= POWERUP_LIFETIME) {
            GAME.MakeCharAvail(GetStartChar());
            m_unlink = true;
        }
    }
}
