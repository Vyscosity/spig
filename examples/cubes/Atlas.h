#pragma once

#include <bigg.hpp>

//#include "Texture.h"
#include <vector>

namespace Texture
{
    class Atlas
    {
    public:
        Atlas(const std::string& textureName, GLuint size, GLuint textureSize)
            : m_size(size), m_textureSize(textureSize)
        {
            texture = bigg::loadTexture(textureName); //this can go into a handy dandy texture class and extend. class Atlas : void Texture or something.
        }

        std::vector<GLfloat> getTextureCoords(const Vector2& location)
        {
            static auto texturesPerRow = m_size / m_textureSize;
            static auto unitSize = 1.0f / (float)texturesPerRow;

            auto xMin = location.x * unitSize;
            auto yMin = location.y * unitSize;

            auto xMax = xMin + unitSize;
            auto yMax = yMin + unitSize;

            return
            {
                xMax, yMax,
                xMin, yMax,
                xMin, yMin,
                xMax, yMin
            };
        }

        bgfx::TextureHandle getTexture() {
            return texture;
        }

    private:
        const GLuint m_size;
        const GLuint m_textureSize;
        bgfx::TextureHandle texture;
    };
}

