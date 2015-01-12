#include <boost/scoped_array.hpp>
#include <stdio.h>
#include "TextureManager.h"
#include "Exceptions.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // Texture
    //////////////////////////////////////////////////////////////////////////

    void Texture::Load(const std::string& textureName)
    {
        FILE *textureFile = fopen(textureName.c_str(), "rb");

        if (!textureFile)
        {
            throw FileNotFoundException(textureName);
        }

        unsigned char expectedHeader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
        unsigned char header[12];
        if (fread(header, sizeof(unsigned char), sizeof(header), textureFile) != sizeof(header) ||
            memcmp(header, expectedHeader, sizeof(expectedHeader)))
        {
            fclose(textureFile);
            throw FileCorruptException(textureName);
        }

        unsigned char info[6];
        if (fread(info, sizeof(unsigned char), sizeof(info), textureFile) != sizeof(info))
        {
            fclose(textureFile);
            throw FileCorruptException(textureName);
        }

        unsigned int width  = info[1] * 256 + info[0];
        unsigned int height = info[3] * 256 + info[2];
        unsigned int bpp    = info[4];

        if (width == 0 || height == 0 || (bpp != 24 && bpp != 32))
        {
            fclose(textureFile);
            throw FileCorruptException(textureName);
        }
        unsigned int size = width * height * (bpp / 8);

        boost::scoped_array<unsigned char> data(new unsigned char[size]);
        if (!data.get())
        {
            fclose(textureFile);
            throw MemoryAllocationError(__FUNCTION__);
        }

        if (fread(data.get(), 1, size, textureFile) != size)
        {
            fclose(textureFile);
            throw FileCorruptException(textureName);
        }

        unsigned char temp;
        for (unsigned int i = 0; i < size; i += (bpp / 8))
        {
            temp        = data[i];
            data[i]     = data[i + 2];
            data[i + 2] = temp;
        }

        fclose(textureFile);

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (bpp == 24)
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        }
        else
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
        }
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }


    //////////////////////////////////////////////////////////////////////////
    // TextureManager
    //////////////////////////////////////////////////////////////////////////

    std::auto_ptr<TextureManager> TextureManager::m_singleton(new TextureManager);
    TextureManager& TextureManager::GetTextureManager()
    {
        return *(m_singleton.get());
    }

    void TextureManager::Add(const std::string& textureName)
    {
        if (m_textureMap.find(textureName) == m_textureMap.end())
        {
            TexturePtr texture(new Texture());
            texture->Load(textureName);
            m_textureMap[textureName] = texture;
        }
    }

    const Texture& TextureManager::Get(const std::string& textureName) const
    {
        TextureMap::const_iterator iter = m_textureMap.find(textureName);
        if (iter == m_textureMap.end())
        {
            throw MediaNotLoadedException(textureName);
        }
        else
        {
            return *(iter->second);
        }
    }

    void TextureManager::Bind(const std::string& textureName) const
    {
        Get(textureName).Bind();
    }
}
