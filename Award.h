#ifndef _AWARD_H_
#define _AWARD_H_

#include <string>
#include <memory>
#include "Effect.h"
#include "Vector.h"
#include "Colour.h"

namespace typing
{
    enum AwardType {
        AWARD_EXCELLENT,
        AWARD_GOOD,            
        AWARD_OK,
        AWARD_POOR,
        AWARD_BAD,
        AWARD_EXTRALIFE,
    };

    class Award : public Effect
    {
    public:
        Award(const juzutil::Vector2& origin, AwardType type, float startTime)
            : m_origin(origin), m_type(type), m_startTime(startTime)
        {
        }

        static void Init();

        void Draw();
        bool Unlink();

    private:
        static const std::string AWARD_FONT;
        static const float       AWARD_RISE_TIME;
        static const float       AWARD_FADE_TIME;
        static const float       AWARD_DISPLAY_TIME;
        static const float       AWARD_START_ALPHA;
        static const float       AWARD_PEAK_ALPHA;
        static const float       AWARD_END_ALPHA;
        static const float       AWARD_FONT_HEIGHT;

        static const ColourRGB   AWARD_EXCELLENT_COLOUR;
        static const ColourRGB   AWARD_GOOD_COLOUR;
        static const ColourRGB   AWARD_OK_COLOUR;
        static const ColourRGB   AWARD_POOR_COLOUR;
        static const ColourRGB   AWARD_BAD_COLOUR;
        static const ColourRGB   AWARD_EXTRALIFE_COLOUR;

        static const std::string AWARD_EXCELLENT_STRING;
        static const std::string AWARD_GOOD_STRING;
        static const std::string AWARD_OK_STRING;
        static const std::string AWARD_POOR_STRING;
        static const std::string AWARD_BAD_STRING;
        static const std::string AWARD_EXTRALIFE_STRING;

        juzutil::Vector2 m_origin;
        AwardType        m_type;
        float            m_startTime;
    };
    typedef std::shared_ptr<Award> AwardPtr;
}

#endif // _AWARD_H_
