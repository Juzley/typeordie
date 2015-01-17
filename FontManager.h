#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include <string>
#include <map>
#include <memory>
#include "Colour.h"

namespace typing
{
    struct CharInfo
    {
        unsigned int x;
        unsigned int y;
        unsigned int width;
    };

    class Font
    {
    public:
        // Ctors/Dtors
        Font()
            : m_texture(""), m_imageWidth(0), m_imageHeight(0), m_charHeight(0), m_charMap()
        {
        }

        // Enums
        enum Align { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

        // Methods
        void  Load(const std::string& fileName);
        float GetLineWidth(float h, const std::string& text) const;
        void  Print(float x, float y, float h, ColourRGBA col, Align align, const std::string& text) const;

    private:
        // Typedefs
        typedef std::map<char, CharInfo> CharMap;

        // Members
        std::string  m_texture;
        unsigned int m_imageWidth;
        unsigned int m_imageHeight;
        unsigned int m_charHeight;
        CharMap      m_charMap;
    };
    typedef std::shared_ptr<Font> FontPtr;

    class FontManager
    {
    public:
        // Singleton implementation
        static FontManager& GetFontManager();

        // Methods
        void        Add(const std::string& fontName);
        const Font& Get(const std::string& fontName) const;
        float       GetLineWidth(const std::string& fontName, float h, const std::string& text) const;
        void        Print(const std::string& fontName, float x, float y, float h, ColourRGBA col, Font::Align align, const std::string& text) const;

    private:
        // Ctors/Dtors
        FontManager()
        {
        }

        // Typedefs
        typedef std::map <std::string, FontPtr> FontMap;

        // Members
        FontMap m_fontMap;

        // Singleton implementation
        static std::auto_ptr<FontManager> m_singleton;
    };
    #define FONTS FontManager::GetFontManager()
}

#endif // _FONT_MANAGER_H_
