#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <string>
#include <memory>
#include <map>
#include <memory>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

namespace typing
{
    class Texture
    {
    public:
        // Ctors/Dtors
        Texture() : m_id(0)
        {
        }

        // Methods
        void Load(const std::string& textureName);
        void Bind() const;

    private:
        GLuint m_id;
    };
    typedef std::shared_ptr<Texture> TexturePtr;

    class TextureManager
    {
    public:
        // Singleton Implementation
        static TextureManager& GetTextureManager();

        // Methods
        void           Add(const std::string& textureName);
        const Texture& Get(const std::string& textureName)  const;
        void           Bind(const std::string& textureName) const;

    private:
        // Ctors/Dtors
        TextureManager()
        {
        }

        // Typedefs
        typedef std::map<std::string, TexturePtr> TextureMap;

        // Members
        TextureMap m_textureMap;

        // Singleton Implementation
        static std::auto_ptr<TextureManager> m_singleton;
    };
    #define TEXTURES TextureManager::GetTextureManager()
}

#endif // _TEXTURE_MANAGER_H_
