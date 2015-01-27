#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <boost/algorithm/string/find.hpp>
#include "FontManager.h"
#include "Exceptions.h"
#include "TextureManager.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // Font
    //////////////////////////////////////////////////////////////////////////

    void Font::Load (const std::string& fileName)
    {
        FILE* fontFile = fopen(fileName.c_str(), "rb");

        if(!fontFile)
        {
            throw FileNotFoundException(fileName);
        }

        char header[4];
        if((fread(header, 1, 4, fontFile) != 4) || memcmp (header, "FONT", 4))
        {
            fclose(fontFile);
            throw FileCorruptException(fileName + ": Invalid header");
        }

        int fileNameLen;
        if((fread(&fileNameLen, sizeof(int), 1, fontFile) != 1) || fileNameLen < 1)
        {
            fclose(fontFile);
            throw FileCorruptException(fileName + ": Invalid file (file name length)");
        }

        std::string textureName;
        for (int i = 0; i < fileNameLen; i++)
        {
            char c = fgetc(fontFile);
            if (c == EOF)
            {
                fclose(fontFile);
                throw FileCorruptException(fileName + ": Invalid font file (file name)");
            }
            else
            {
                textureName += c;
            }
        }


        if((fread(&m_imageWidth, sizeof(int), 1, fontFile) != 1) || m_imageWidth < 1)
        {
            fclose(fontFile);
            throw FileCorruptException(fileName + ": Invalid font file (texture width)");
        }

        if((fread(&m_imageHeight, sizeof(int), 1, fontFile) != 1) || m_imageHeight < 1)
        {
            fclose(fontFile);
            throw FileCorruptException(fileName + ": Invalid font file (texture height)");
        }

        if((fread(&m_charHeight, sizeof(int), 1, fontFile) != 1) || m_charHeight < 1)
        {
            fclose(fontFile);
            throw FileCorruptException(fileName + ": Invalid font file (char height)");
        }

        m_charMap.clear();
        char c;
        while(fread(&c, 1, 1, fontFile) == 1)
        {
            CharInfo cInfo;
            if(fread(&cInfo, sizeof(CharInfo), 1, fontFile) != 1)
            {
                fclose(fontFile);
                throw FileCorruptException(fileName + ": Invalid font file (char height)");
            }

            m_charMap.insert(CharMap::value_type(c, cInfo));
        }

        std::string dir;
        boost::iterator_range<std::string::const_iterator> res = boost::find_last(fileName, "/");
        if (res.end() != fileName.end())
        {
            dir.assign(fileName.begin(), res.end());
        }

        m_texture = dir + textureName;
        TEXTURES.Add(m_texture);
    }

    float Font::GetLineWidth(float h, const std::string& text) const
    {
        float textWidth = 0.0f;

        for(std::string::const_iterator iter = text.begin(); iter != text.end(); ++iter)
        {
            CharMap::const_iterator cInfoIter = m_charMap.find(*iter);
            if (cInfoIter == m_charMap.end())
            {
                // We didn't find the char in the char map, miss it out and go onto the next letter.
                continue;
            }

            const CharInfo& cInfo = cInfoIter->second;
            textWidth += h * static_cast<float>(cInfo.width) / static_cast<float>(m_charHeight);
        }

        return textWidth;
    }

    void Font::Print(float x, float y, float h, ColourRGBA col, Align align, const std::string& text) const
    {
        const float th = static_cast<float>(m_charHeight) / static_cast<float>(m_imageHeight);

        if(align == ALIGN_CENTER)
        {
            x -= GetLineWidth(h, text) / 2.0f;
        }
        else if(align == ALIGN_RIGHT)
        {
            x -= GetLineWidth(h, text);
        }

        TEXTURES.Bind(m_texture);
        glColor4f(col.GetRed(), col.GetGreen(), col.GetBlue(), col.GetAlpha());

        for(std::string::const_iterator iter = text.begin(); iter != text.end(); ++iter)
        {
            CharMap::const_iterator cInfoIter = m_charMap.find(*iter);
            if (cInfoIter == m_charMap.end())
            {
                // We didn't find the char in the char map, miss it out and go onto the next letter.
                continue;
            }

            const CharInfo& cInfo = cInfoIter->second;
            const float tl        = static_cast<float>(cInfo.x) / static_cast<float>(m_imageWidth);
            const float tt        = static_cast<float>(cInfo.y) / static_cast<float>(m_imageHeight);
            const float tw        = static_cast<float>(cInfo.width) / static_cast<float>(m_imageWidth);
            const float w         = h * static_cast<float>(cInfo.width) / static_cast<float>(m_charHeight);

            glBegin(GL_QUADS);
            glTexCoord2f(tl, 1.0f - tt - th); // bottom left
            glVertex2f(x, y + h);

            glTexCoord2f(tl + tw, 1.0f - tt - th); // bottom right
            glVertex2f(x + w, y + h);

            glTexCoord2f(tl + tw, 1.0f - tt); // top right
            glVertex2f(x + w, y);

            glTexCoord2f(tl, 1.0f - tt); // top left
            glVertex2f(x, y);
            glEnd();
            x += w;
        }
    }

    bool Font::HasChar(char c)
    {
        return m_charMap.find(c) != m_charMap.end();
    }


    //////////////////////////////////////////////////////////////////////////
    // FontManager
    //////////////////////////////////////////////////////////////////////////

    std::auto_ptr<FontManager> FontManager::m_singleton(new FontManager);
    FontManager& FontManager::GetFontManager()
    {
        return *(m_singleton.get());
    }


    void FontManager::Add(const std::string& fontName)
    {
        if (m_fontMap.find(fontName) == m_fontMap.end())
        {
            FontPtr font(new Font());
            font->Load(fontName);
            m_fontMap[fontName] = font;
        }
    }


    const Font& FontManager::Get(const std::string& fontName) const
    {
        FontMap::const_iterator iter = m_fontMap.find(fontName);
        if (iter == m_fontMap.end())
        {
            throw MediaNotLoadedException(fontName);
        }
        else
        {
            return *(iter->second);
        }
    }


    float FontManager::GetLineWidth(const std::string& fontName, float h, const std::string& text) const
    {
        return Get(fontName).GetLineWidth(h, text);
    }


    void FontManager::Print(const std::string& fontName, float x, float y, float h, ColourRGBA col, Font::Align align, const std::string& text) const
    {
        Get(fontName).Print(x, y, h, col, align, text);
    }
}
